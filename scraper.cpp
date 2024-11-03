#include <cpr/cpr.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#define BASE_URL "https://www.ah.nl/shop/prijsfavorieten"
#define PREFIX "https://www.ah.nl"

#define PRODUCT_MASTER_DIV "product-card-portrait_root__ZiRpZ"
#define NAME_PATH ".//a/@title"
#define PRICE_PATH ".//span[@class='sr-only']/@aria-label"
#define LINK_PATH ".//a/@href"

// Main div: product-card-portrait_root__ZiRpZ

// URL: link_root__EqRHd in href

std::ifstream InitializeFile(void) {
    std::string fileName = "AHdata.txt";
    std::ifstream file(fileName);
    if (file.is_open()) {
        std::cerr << "Failed to open file" << fileName << std::endl;
        return std::ifstream();
    }
    return file;
}

// Retrieves a DOM structure of the Acquired HTML file requested via GET
htmlDocPtr RetrieveHTMLPage(void) {
    // Initalize the header so this won't look like a BOT Request.
    // Might eventually rotate to random users if bot detection problem occur.
    cpr::Header header = {
        {"User-Agent",
         "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like "
         "Gecko) Chrome/117.0.0.0 Safari/537.36"}};

    // Send the GET request and stores it in HTMLdoc
    cpr::Response HTMLdoc = cpr::Get(cpr::Url(BASE_URL), header);

    if (HTMLdoc.status_code != 200) {
        std::cerr << "Status code isn't OK" << std::endl;
    }

    // std::cout << HTMLdoc.text << std::endl;
    // We try to optimize here, but might leave errors and external resources
    // undetected or unusable.
    int options = HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR |
                  HTML_PARSE_NOWARNING | HTML_PARSE_NONET;

    // Restructure the HTMLdoc into a DOM model so we can use XML parsing on it.
    htmlDocPtr ParsedDoc = htmlReadMemory(
        HTMLdoc.text.c_str(), HTMLdoc.text.length(), nullptr, nullptr, options);

    if (ParsedDoc == NULL) {
        std::cerr << "ParsedDoc is somehow a NULL ptr" << std::endl;
    }

    return ParsedDoc;
}

std::string getNodeContent(xmlNodePtr node) {
    if (node == NULL) {
        return "";
    }
    xmlChar* content = NULL;
    if (node->type == XML_ATTRIBUTE_NODE) {
        content = xmlNodeListGetString(node->doc, node->children, 1);
    } else {
        content = xmlNodeGetContent(node);
    }
    if (content == NULL) {
        return "";
    }
    std::string result(reinterpret_cast<char*>(content));
    xmlFree(content);
    return result;
}

/* Debug function to inspect the structure.
 */
void PrintNodeAttributes(xmlNodePtr node, int depth = 0) {
    if (node == NULL) {
        return;
    }

    // Indentation for readability
    std::string indent(depth * 2, ' ');

    // Print node name
    std::cout << indent << "Node Name: " << node->name << std::endl;

    // Print node attributes
    for (xmlAttrPtr attr = node->properties; attr != NULL; attr = attr->next) {
        xmlChar* value = xmlNodeListGetString(node->doc, attr->children, 1);
        std::cout << indent << "  Attribute: " << attr->name << " = " << value
                  << std::endl;
        xmlFree(value);
    }

    // Optionally, print node content if it's a text node
    if (node->type == XML_TEXT_NODE) {
        xmlChar* content = xmlNodeGetContent(node);
        if (content != NULL && xmlStrlen(content) > 0) {
            std::cout << indent << "  Content: " << content << std::endl;
            xmlFree(content);
        }
    }

    // Recursively print child nodes
    for (xmlNodePtr child = node->children; child != NULL;
         child = child->next) {
        PrintNodeAttributes(child, depth + 1);
    }
}

void ProcessIndividual_Node(xmlNodePtr node, htmlDocPtr doc) {
    // We create a new context to query the node that matched our first search
    // in XMLParsed.

    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == NULL) {
        std::cerr << "Couldn't create a context" << std::endl;
        return;
    }

    // context node is current node.
    context->node = node;

    // XPath to extract the product name.
    std::string nameXPath = NAME_PATH;

    // XPath to extract the price.
    std::string priceXPath = PRICE_PATH;

    // XPatht o extract link.
    std::string linkXPath = LINK_PATH;

    // Evaluate the XPath expression.
    xmlXPathObjectPtr nameNodes = xmlXPathEvalExpression(
        reinterpret_cast<const xmlChar*>(nameXPath.c_str()), context);

    std::string ProductName = "N/A";
    if (nameNodes != NULL && nameNodes->nodesetval->nodeNr > 0) {
        xmlNodePtr nameNode = nameNodes->nodesetval->nodeTab[0];
        ProductName = getNodeContent(nameNode);

        // Remove the 'Bekijk'
        const std::string prefix = "Bekijk ";
        if (ProductName.compare(0, prefix.size(), prefix) == 0) {
            ProductName = ProductName.substr(prefix.size());
        }
    }
    xmlXPathFreeObject(nameNodes);

    // Evaluate the price XPath expression
    xmlXPathObjectPtr priceNodes = xmlXPathEvalExpression(
        reinterpret_cast<const xmlChar*>(priceXPath.c_str()), context);

    std::string productPrice = "N/A";
    if (priceNodes != NULL && priceNodes->nodesetval->nodeNr > 0) {
        xmlNodePtr priceAttrNode = priceNodes->nodesetval->nodeTab[0];
        productPrice = getNodeContent(priceAttrNode);
        // Remove 'Prijs: ' prefix
        const std::string prefix = "Prijs: ";
        if (productPrice.compare(0, prefix.size(), prefix) == 0) {
            productPrice = productPrice.substr(prefix.size());
        }
    }
    xmlXPathFreeObject(priceNodes);

    xmlXPathObjectPtr linkNodes = xmlXPathEvalExpression(
        reinterpret_cast<const xmlChar*>(linkXPath.c_str()), context);
    std::string link = "N/A";
    if (linkNodes != NULL && linkNodes->nodesetval->nodeNr > 0) {
        xmlNodePtr linkAttrNode = linkNodes->nodesetval->nodeTab[0];
        link = getNodeContent(linkAttrNode);
    }
    // Clean up the XPath context
    xmlXPathFreeContext(context);

    std::cout << ProductName << "   " << productPrice << "      "
              << PREFIX + link << std::endl;
}

void ProcessNodes(xmlXPathObjectPtr Objects, htmlDocPtr doc) {
    // A pointer to the set of corresponding nodes.
    xmlNodeSetPtr CorrespondingNodes = Objects->nodesetval;

    // Check if ANY corresponding nodes were found.
    if (CorrespondingNodes != NULL) {
        std::cout << "Found " << CorrespondingNodes->nodeNr << std::endl;
    }
    for (int i = 0; i < CorrespondingNodes->nodeNr; i++) {
        xmlNodePtr node = CorrespondingNodes->nodeTab[i];
        if (node != NULL) {
            ProcessIndividual_Node(
                node, doc);  // If i don't end up here then the node's
                             // memory won't get freed. Potential leak.
        }
    }
}

int XMLParsed(htmlDocPtr doc) {
    // Create a context for our doc. This is done so we can query XML into this
    // specific file.
    xmlXPathContextPtr context = xmlXPathNewContext(doc);

    if (context == NULL) {
        std::cerr << "Couldn't create a context" << std::endl;
        xmlFreeDoc(doc);
        return -1;
    }

    // We use std::string as we want the safety benefits of this i.e const char*
    // std::string xpathExpr =
    //     "//*[contains(@class, 'line-clamp_root__7DevG "
    //     "line-clamp_active__5Qc2L "
    //     "title_lineclamp__kjrFA')]";

    std::string xpathExpr = "//*[contains(@class, '" PRODUCT_MASTER_DIV "')]";

    // xpathObj contains the node that match the query
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(
        reinterpret_cast<const xmlChar*>(xpathExpr.c_str()), context);

    if (xpathObj == NULL) {
        fprintf(stderr, "Could not evaluate XPath expression\n");
        xmlXPathFreeContext(context);
        xmlFreeDoc(doc);
        return -1;
    }

    ProcessNodes(xpathObj, doc);

    // free the Xpath object after processing
    xmlXPathFreeObject(xpathObj);

    // Free the XPath context
    xmlXPathFreeContext(context);

    // Free the document
    xmlFreeDoc(doc);
    return 0;
}

void Retrieve_And_Initalize_Data(void) {
    std::ifstream file = InitializeFile();
    htmlDocPtr doc = RetrieveHTMLPage();
    XMLParsed(doc);
}

// Name : line-clamp_root__7DevG line-clamp_active__5Qc2L
// title_lineclamp__kjrFA
//  Price : price-amount_root__Sa88q price-amount_highlight__ekL92
//  price_amount__s-QN4
// price_highlight__RucvZ in span sr-only

// Image; lazy-image_image__o9P+M as src

/*
    Plan is to eventually write this all to a well formatted JSON file for
   ease. For now we will use a txt file as it is the easiest.
*/
int main() {
    Retrieve_And_Initalize_Data();
    return 0;
}
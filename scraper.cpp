#include <cpr/cpr.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>

#define BASE_URL "https://www.ah.nl/shop/prijsfavorieten"

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
    xmlChar* content = xmlNodeGetContent(node);
    if (content == NULL) {
        return "";
    }
    std::string result(reinterpret_cast<char*>(content));
    xmlFree(content);
    return result;
}

void ProcessNodes(xmlXPathObjectPtr Objects) {
    // A pointer to the set of corresponding nodes.
    xmlNodeSetPtr CorrespondingNodes = Objects->nodesetval;

    // Check if ANY corresponding nodes were found.
    if (CorrespondingNodes != NULL) {
        std::cout << "Found " << CorrespondingNodes->nodeNr << std::endl;
    }
    for (int i = 0; i < CorrespondingNodes->nodeNr; i++) {
        xmlNodePtr node = CorrespondingNodes->nodeTab[i];
        if (node != NULL) {
            std::cout << getNodeContent(node)
                      << std::endl;  // If i don't end up here then the node's
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
    std::string xpathExpr =
        "//*[contains(@class, 'line-clamp_root__7DevG "
        "line-clamp_active__5Qc2L "
        "title_lineclamp__kjrFA')]";

    // xpathObj contains the node that match the query
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(
        reinterpret_cast<const xmlChar*>(xpathExpr.c_str()), context);

    if (xpathObj == NULL) {
        fprintf(stderr, "Could not evaluate XPath expression\n");
        xmlXPathFreeContext(context);
        xmlFreeDoc(doc);
        return -1;
    }
    ProcessNodes(xpathObj);

    // free the Xpath object after processing
    xmlXPathFreeObject(xpathObj);

    // Free the XPath context
    xmlXPathFreeContext(context);

    // Free the document
    xmlFreeDoc(doc);
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

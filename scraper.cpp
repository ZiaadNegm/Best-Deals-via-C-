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
#define PICTURE_LINK_PATH ".//img/@src"

struct ProductData {
    std::string name;
    std::string price;
    std::string link;
    std::string pictureLink;
};

// Struct to hold compiled XPath expressions
struct CompiledXPathExpressions {
    xmlXPathCompExprPtr nameExpr;
    xmlXPathCompExprPtr priceExpr;
    xmlXPathCompExprPtr linkExpr;
    xmlXPathCompExprPtr pictureLinkExpr;
};

// Function to retrieve the HTML page and parse it into a DOM structure
htmlDocPtr RetrieveHTMLPage() {
    // Initialize the header to mimic a regular browser request
    cpr::Header header = {
        {"User-Agent",
         "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like "
         "Gecko) Chrome/117.0.0.0 Safari/537.36"}};

    // Send the GET request
    cpr::Response HTMLdoc = cpr::Get(cpr::Url(BASE_URL), header);

    if (HTMLdoc.status_code != 200) {
        std::cerr << "Failed to retrieve the webpage. Status code: "
                  << HTMLdoc.status_code << std::endl;
        return nullptr;
    }

    // Parse options
    int options = HTML_PARSE_NOBLANKS | HTML_PARSE_NOERROR |
                  HTML_PARSE_NOWARNING | HTML_PARSE_NONET | HTML_PARSE_RECOVER;

    // Parse the HTML content into a DOM structure
    htmlDocPtr doc = htmlReadMemory(HTMLdoc.text.c_str(), HTMLdoc.text.length(),
                                    nullptr, nullptr, options);

    if (doc == nullptr) {
        std::cerr << "Failed to parse HTML content." << std::endl;
    }

    return doc;
}

// Helper function to evaluate a compiled XPath expression and get content
std::string GetCompiledXPathContent(xmlXPathContextPtr context,
                                    xmlXPathCompExprPtr compiledExpr) {
    xmlXPathObjectPtr xpathObj = xmlXPathCompiledEval(compiledExpr, context);
    if (xpathObj == nullptr) {
        std::cerr << "Could not evaluate compiled XPath expression."
                  << std::endl;
        return "N/A";
    }

    std::string content = "N/A";
    if (xpathObj->nodesetval && xpathObj->nodesetval->nodeNr > 0) {
        xmlNodePtr node = xpathObj->nodesetval->nodeTab[0];
        if (node != nullptr) {
            if (node->type == XML_ATTRIBUTE_NODE) {
                content = reinterpret_cast<char*>(xmlNodeGetContent(node));
            } else {
                xmlChar* nodeContent = xmlNodeGetContent(node);
                if (nodeContent != nullptr) {
                    content = reinterpret_cast<char*>(nodeContent);
                    xmlFree(nodeContent);
                }
            }
        }
    }

    xmlXPathFreeObject(xpathObj);
    return content;
}

// Process individual node
void ProcessIndividual_Node(xmlNodePtr node, xmlXPathContextPtr context,
                            CompiledXPathExpressions& compiledExprs) {
    // Set the context node
    context->node = node;

    // Extract product data
    ProductData product;

    // Extract Product Name
    product.name = GetCompiledXPathContent(context, compiledExprs.nameExpr);
    // Remove the 'Bekijk ' prefix if present
    const std::string prefix = "Bekijk ";
    if (product.name.compare(0, prefix.size(), prefix) == 0) {
        product.name = product.name.substr(prefix.size());
    }

    // Extract Product Price
    product.price = GetCompiledXPathContent(context, compiledExprs.priceExpr);
    // Remove 'Prijs: ' prefix if present
    const std::string pricePrefix = "Prijs: ";
    if (product.price.compare(0, pricePrefix.size(), pricePrefix) == 0) {
        product.price = product.price.substr(pricePrefix.size());
    }

    // Extract Product Link
    product.link = GetCompiledXPathContent(context, compiledExprs.linkExpr);
    if (product.link != "N/A") {
        product.link = PREFIX + product.link;
    }

    // Extract Picture Link
    product.pictureLink =
        GetCompiledXPathContent(context, compiledExprs.pictureLinkExpr);

    // Output the extracted information
    std::cout << "Name: " << product.name << std::endl;
    std::cout << "Price: " << product.price << std::endl;
    std::cout << "Link: " << product.link << std::endl;
    std::cout << "Picture Link: " << product.pictureLink << std::endl;
    std::cout << "-------------------------------------" << std::endl;
}

// Process the nodes returned by the XPath expression
void ProcessNodes(xmlXPathObjectPtr xpathObj, xmlXPathContextPtr context,
                  CompiledXPathExpressions& compiledExprs) {
    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    if (nodes != nullptr) {
        std::cout << "Found " << nodes->nodeNr << " products." << std::endl;
        for (int i = 0; i < nodes->nodeNr; ++i) {
            xmlNodePtr node = nodes->nodeTab[i];
            if (node != nullptr) {
                ProcessIndividual_Node(node, context, compiledExprs);
            }
        }
    } else {
        std::cout << "No matching nodes found." << std::endl;
    }
}

// Main function to parse the HTML document and extract data
int XMLParsed(htmlDocPtr doc) {
    // Create a single XPath context
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == nullptr) {
        std::cerr << "Couldn't create an XPath context." << std::endl;
        xmlFreeDoc(doc);
        return -1;
    }

    // XPath expression to select product cards
    std::string xpathExpr = "//*[contains(@class, '" PRODUCT_MASTER_DIV "')]";

    // Evaluate the XPath expression
    xmlXPathObjectPtr xpathObj = xmlXPathEvalExpression(
        reinterpret_cast<const xmlChar*>(xpathExpr.c_str()), context);

    if (xpathObj == nullptr) {
        std::cerr << "Could not evaluate XPath expression: " << xpathExpr
                  << std::endl;
        xmlXPathFreeContext(context);
        xmlFreeDoc(doc);
        return -1;
    }

    // Compile XPath expressions for product details
    CompiledXPathExpressions compiledExprs;
    compiledExprs.nameExpr = xmlXPathCompile(BAD_CAST NAME_PATH);
    compiledExprs.priceExpr = xmlXPathCompile(BAD_CAST PRICE_PATH);
    compiledExprs.linkExpr = xmlXPathCompile(BAD_CAST LINK_PATH);
    compiledExprs.pictureLinkExpr = xmlXPathCompile(BAD_CAST PICTURE_LINK_PATH);

    if (!compiledExprs.nameExpr || !compiledExprs.priceExpr ||
        !compiledExprs.linkExpr || !compiledExprs.pictureLinkExpr) {
        std::cerr << "Failed to compile one or more XPath expressions."
                  << std::endl;
        // Free any compiled expressions that were successfully created
        if (compiledExprs.nameExpr)
            xmlXPathFreeCompExpr(compiledExprs.nameExpr);
        if (compiledExprs.priceExpr)
            xmlXPathFreeCompExpr(compiledExprs.priceExpr);
        if (compiledExprs.linkExpr)
            xmlXPathFreeCompExpr(compiledExprs.linkExpr);
        if (compiledExprs.pictureLinkExpr)
            xmlXPathFreeCompExpr(compiledExprs.pictureLinkExpr);

        xmlXPathFreeObject(xpathObj);
        xmlXPathFreeContext(context);
        xmlFreeDoc(doc);
        return -1;
    }

    // Process the nodes
    ProcessNodes(xpathObj, context, compiledExprs);

    // Free compiled XPath expressions
    xmlXPathFreeCompExpr(compiledExprs.nameExpr);
    xmlXPathFreeCompExpr(compiledExprs.priceExpr);
    xmlXPathFreeCompExpr(compiledExprs.linkExpr);
    xmlXPathFreeCompExpr(compiledExprs.pictureLinkExpr);

    // Free the XPath object and context
    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(context);

    // Free the document
    xmlFreeDoc(doc);

    return 0;
}

// Function to retrieve and initialize data
void Retrieve_And_Initialize_Data() {
    htmlDocPtr doc = RetrieveHTMLPage();
    if (doc != nullptr) {
        XMLParsed(doc);
    }
}

int main() {
    Retrieve_And_Initialize_Data();
    return 0;
}

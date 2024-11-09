#include <cpr/cpr.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <vector>

#define BASE_URL "https://www.ah.nl/shop/prijsfavorieten"
#define PREFIX "https://www.ah.nl"

#define PRODUCT_MASTER_DIV "product-card-portrait_root__ZiRpZ"
#define NAME_PATH ".//a/@title"
#define PRICE_PATH ".//span[@class='sr-only']/@aria-label"
#define LINK_PATH ".//a/@href"
#define PICTURE_LINK_PATH ".//img/@src"

using json = nlohmann::json;

struct ProductData {
    std::string name;
    std::string price;
    std::string link;
    std::string pictureLink;
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(ProductData, name, price, link, pictureLink)
};

// Correct placement of the macro outside the struct

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
                            CompiledXPathExpressions& compiledExprs,
                            std::vector<ProductData>& TotalProduct_Data) {
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

    // Add the product to the list
    TotalProduct_Data.push_back(product);
}

// Process the nodes returned by the XPath expression
json ProcessNodes(xmlXPathObjectPtr xpathObj, xmlXPathContextPtr context,
                  CompiledXPathExpressions& compiledExprs) {
    xmlNodeSetPtr nodes = xpathObj->nodesetval;

    std::vector<ProductData> TotalProduct_Data;
    if (nodes != nullptr) {
        std::cout << "Found " << nodes->nodeNr << " products." << std::endl;
        for (int i = 0; i < nodes->nodeNr; ++i) {
            xmlNodePtr node = nodes->nodeTab[i];
            if (node != nullptr) {
                ProcessIndividual_Node(node, context, compiledExprs,
                                       TotalProduct_Data);
            }
        }
    } else {
        std::cout << "No matching nodes found." << std::endl;
    }
    json j_products = TotalProduct_Data;
    return j_products;
}

// Main function to parse the HTML document and extract data
json XMLParsed(htmlDocPtr doc) {
    // Create a single XPath context
    xmlXPathContextPtr context = xmlXPathNewContext(doc);
    if (context == nullptr) {
        std::cerr << "Couldn't create an XPath context." << std::endl;
        xmlFreeDoc(doc);
        json errorResult;
        errorResult["error"] = "Couldn't create an XPath context.";
        return errorResult;
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
        json errorResult;
        errorResult["error"] = "Could not evaluate XPath expression.";
        return errorResult;
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
        json errorResult;
        errorResult["error"] = "Failed to compile XPath expressions.";
        return errorResult;
    }

    // Process the nodes
    json JSON_Products = ProcessNodes(xpathObj, context, compiledExprs);

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

    return JSON_Products;
}

// Function to retrieve and initialize data
void Retrieve_And_Initialize_Data() {
    htmlDocPtr doc = RetrieveHTMLPage();
    if (doc != nullptr) {
        json result = XMLParsed(doc);
        if (result.contains("error")) {
            std::cerr << "Error: " << result["error"] << std::endl;
        } else {
            // Output the JSON to console
            std::cout << result.dump(4) << std::endl;

            // Write JSON to file
            std::ofstream outFile("products.json");
            if (outFile.is_open()) {
                outFile << result.dump(4);
                outFile.close();
            } else {
                std::cerr << "Failed to open output file for writing."
                          << std::endl;
            }
        }
    }
}

int main() {
    Retrieve_And_Initialize_Data();
    return 0;
}

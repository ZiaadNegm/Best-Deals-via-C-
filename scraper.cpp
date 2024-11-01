// scraper.cpp

#include <cpr/cpr.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#include <iostream>
#include <vector>

// Define a struct to store the scraped data
struct IndustryCard {
    std::string image;
    std::string url;
    std::string name;
};

int main() {
    cpr::Response response =
        cpr::Get(cpr::Url{"https://www.ah.nl/producten/snoep-chocolade-koek"},
                 cpr::Header{{"User-Agent",
                              "Mozilla/5.0 (Windows NT 10.0; Win64; x64) "
                              "AppleWebKit/537.36 (KHTML, like Gecko) "
                              "Chrome/92.0.4515.159 Safari/537.36"},
                             {"Accept",
                              "text/html,application/xhtml+xml,application/xml;"
                              "q=0.9,image/avif,image/webp,image/apng,*/*;"
                              "q=0.8,application/signed-exchange;v=b3;q=0.9"},
                             {"Accept-Language", "en-US,en;q=0.9"}});

    if (response.status_code == 200) {
        std::cout << "Successfully retrieved the webpage." << std::endl;
        std::cout << response.text << std::endl;
    } else {
        std::cout << "Failed to retrieve the webpage. Status code: "
                  << response.status_code << std::endl;
        std::cout << response.text << std::endl;
    }
    return 0;
}

// File: oauth_redirect_server.cpp

#include "httplib.h"
#include <iostream>
#include <string>

// Function to extract query parameters
std::string get_query_param(const std::string& query, const std::string& key) {
    size_t start = query.find(key + "=");
    if (start == std::string::npos) return "";
    start += key.length() + 1;
    size_t end = query.find("&", start);
    if (end == std::string::npos) end = query.length();
    return query.substr(start, end - start);
}

int main() {
    using namespace httplib;

    // Create a server instance
    Server svr;

    // Define the callback for the /callback path
    svr.Get("/callback", [&](const Request& req, Response& res) {
        std::cout << "Received a callback request!" << std::endl;

        // Extract 'code' and 'state' from query parameters
        std::string code = req.get_param_value("code");
        std::string state = req.get_param_value("state");

        if (code.empty()) {
            res.set_content("Authorization code not found.", "text/plain");
            std::cerr << "Authorization code not found in the request." << std::endl;
            return;
        }

        std::cout << "Authorization Code: " << code << std::endl;
        if (!state.empty()) {
            std::cout << "State: " << state << std::endl;
        }

        // Respond to the browser
        res.set_content("Authorization successful! You can close this window.", "text/plain");

        // TODO: Proceed to exchange the authorization code for an access token
        // This can be done by signaling another part of your application or triggering a callback
    });

    // Listen on localhost:8080
    std::cout << "Starting server at http://localhost:8080/callback" << std::endl;
    svr.listen("localhost", 8080);

    return 0;
}

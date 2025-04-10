#include "response.hpp"

#include <sys/socket.h>
#include <vector>

#include "../utils/console.hpp"
#include "../utils/utils.hpp"

using std::string;
using std::to_string;

const string VERSION = "1.1";

//                             Pointer to this occurence's code in memory
Response::Response(int code) { this->code = code; }
Response::Response(int code, string message) {
    this->code = code;
    contentType = "text/plain";
    this->message = message;
}

string Response::headers() {
    std::vector<string> headers_vector;
    headers_vector.push_back("HTTP/" + VERSION + " " + to_string(code) + " " + status());
    if (!message.empty()) headers_vector.push_back("Content-Length: " + to_string(this->message.size()));
    if (!contentType.empty()) headers_vector.push_back("Content-Type: " + contentType);

    // Header-body separator
    headers_vector.push_back("");
    headers_vector.push_back("");
    return vec::join(headers_vector, "\r\n");
}

int Response::getCode() { return code; }
string Response::getMessage() { return message; }

// PRIVATE
// Use map instead?
string Response::status() {
    switch (code) {
        case 200:
            return "OK";
        case 404:
            return "Not Found";
        case 503:
            return "Service Unavailable";
        default:
            return to_string(code);
    }
}

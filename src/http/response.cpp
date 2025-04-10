#include "response.hpp"

#include <sys/socket.h>

// #define VERSION "1.1"
constexpr const char *VERSION = "1.1";

using std::string;
using std::to_string;

//                             Pointer to this occurence's code in memory
Response::Response(int code) { this->code = code; }

string Response::headers() {
    string headers = "HTTP/" + string(VERSION) + " " + to_string(code) + " " + status() + "\r\n\r\n";
    return headers;
}

int Response::getCode() { return code; }

// PRIVATE
// Use map instead?
string Response::status() {
    switch (code) {
        case 200:
            return "OK";
        case 404:
            return "Not Found";
        default:
            return to_string(code);
    }
}

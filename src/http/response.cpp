#include "response.hpp"

#include <sys/socket.h>

#include "../utils/console.hpp"

using std::string;
using std::to_string;

const string VERSION = "1.1";
const string CRLF = "\r\n";

//                             Pointer to this occurence's code in memory
Response::Response(int code) { this->code = code; }
Response::Response(int code, string message) {
    this->code = code;
    contentType = "text/plain";
    this->message = message;
}

string Response::headers() {
    string headers = "HTTP/" + VERSION + " " + to_string(code) + " " + status() + CRLF;
    //                                                                            in bytes
    if (!message.empty()) headers += "Content-Length: " + to_string(this->message.size()) + CRLF;
    if (!contentType.empty()) headers += "Content-Type: " + contentType + CRLF;
    headers += CRLF;
    // log(headers);
    return headers;
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
        default:
            return to_string(code);
    }
}

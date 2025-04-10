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
    content_type = "text/plain";
    this->message = message;
    file = nullptr;
}
Response::Response(int code, std::ifstream *file) {
    this->code = code;
    content_type = "application/octet-stream";
    this->file = file;
}

string Response::headers() {
    std::vector<string> headers_vector;
    headers_vector.push_back("HTTP/" + VERSION + " " + to_string(code) + " " + status());
    if (!content_type.empty()) headers_vector.push_back("Content-Type: " + content_type);
    if (!message.empty()) headers_vector.push_back("Content-Length: " + to_string(this->message.size()));
    if (file != nullptr && file->is_open()) {
        file->seekg(0, std::ios::end); // Move the file pointer to the end
        std::streampos fileSize = file->tellg();
        headers_vector.push_back("Content-Length: " + to_string(fileSize));
        file->seekg(0, std::ios::beg); // Move back to the beginning to read it
    }

    // Header-body separator
    headers_vector.push_back("");
    headers_vector.push_back("");

    return vec::join(headers_vector, "\r\n");
}

int Response::get_code() { return code; }
string Response::msg() { return message; }

// PRIVATE
// Use map instead?
string Response::status() {
    switch (code) {
        case 200:
            return "OK";
        case 400:
            return "Bad Request";
        case 404:
            return "Not Found";
        case 503:
            return "Service Unavailable";
        default:
            return to_string(code);
    }
}

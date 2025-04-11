#include "response.hpp"

#include <sys/socket.h>
#include <vector>

#include "../encode.hpp"
#include "../utils/console.hpp"
#include "../utils/utils.hpp"

using std::string;
using std::to_string;

const string VERSION = "1.1";

//                           default ""      default []
Response::Response(int code, string message, vector<string> encodings) {
    // Pointer to this occurence's code in memory
    this->code = code;
    if (!message.empty()) {
        content_type = "text/plain";
        this->message = message;
    }
    file = nullptr; // Initialize a null pointer
    encode(encodings);
}
//                                                default ""
Response::Response(int code, std::ifstream *file, vector<string> encodings) {
    this->code = code;
    content_type = "application/octet-stream";
    this->file = file;
    encode(encodings);
}

string Response::headers() {
    std::vector<string> headers_vector;
    headers_vector.push_back("HTTP/" + VERSION + " " + to_string(code) + " " + status());
    if (!content_type.empty()) headers_vector.push_back("Content-Type: " + content_type);
    if (!message.empty()) headers_vector.push_back("Content-Length: " + to_string(encoded_msg.size()));
    if (file != nullptr && file->is_open()) {
        file->seekg(0, std::ios::end); // Move the file pointer to the end
        std::streampos fileSize = file->tellg();
        headers_vector.push_back("Content-Length: " + to_string(fileSize));
        file->seekg(0, std::ios::beg); // Move back to the beginning to read it
    }
    if (!encoding.empty()) headers_vector.push_back("Content-Encoding: " + encoding);

    // Header-body separator
    headers_vector.push_back("");
    headers_vector.push_back("");

    log(vec::join(headers_vector, "\r\n")); // 
    return vec::join(headers_vector, "\r\n");
}

int Response::get_code() { return code; }
string Response::msg() { return this->encoded_msg; }


// PRIVATE
// Use map instead?
string Response::status() {
    switch (code) {
        case 200:
            return "OK";
        case 201:
            return "Created";
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

void Response::encode(vector<string> encodings) {
    // Encode
    int flag = 1;
    log("encoding: " + vec::join(encodings, ", "));
    do {
        try {
            encoding = selectEncoding(encodings);
            //            global-scoped (from ../encode.hpp)
            encoded_msg = ::encode(message, encoding);
            flag = 0;
        } catch (const std::runtime_error &e) {
            if (!encoding.empty()) {
                error("Error encoding response with " + encoding);
                error(e.what());
                encodings = vec::filter(encodings, encoding);
            } else throw std::runtime_error("Error without encoding");
        }
    } while (flag);
}

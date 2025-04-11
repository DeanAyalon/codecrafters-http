#include "request.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>
#include <fstream>

#include "../utils/console.hpp"
#include "../utils/utils.hpp"
#include "../encode.hpp"

#define BUFFER_SIZE 1024
#define ENCODINGS "Accept-Encoding"

using std::string;
using std::vector;

Request::Request() {}

string Request::ip() {
    //            num to addr                   .s_addr = IP as binary (32b)
    return string(inet_ntoa(client_addr.sin_addr));
}

void Request::accept(int client, sockaddr_in *address) {
    client_fd = client;
    this->client_addr = *address;

    // Read contents
    char buffer[BUFFER_SIZE] = {0}; // 1MB
    if (recv(client_fd, buffer, BUFFER_SIZE, 0) < 0)
        throw "Message receival failed";
    contents = string(buffer);

    // Separate request content
    using str::split;
    vector<string> parts = split(contents, "\r\n\r\n", 1);
    const string header = parts[0];
    body = parts[1];
    
    // Analyze header
    vector<string> headerLines = split(header, "\r\n");
    vector<string> title = split(headerLines[0], " ");
    method = title[0];
    path = title[1];
    path_components = vec::filter(split(path, "/"), "");
    for (int i = 1; i < headerLines.size(); i++) {
        const vector<string> line = split(headerLines[i], ": ");
        if (line.size() < 2) continue; // skip malformed lines

        const string key = line[0];
        const vector<string> values = { line[1] };

        for (const auto &value : values) headers[key].push_back(value);
    }
    // Encodings
    vector<string> encodings = split(vec::join(headers[ENCODINGS], ","), ",");  // Merge rows
    for (auto &&encoding : encodings) encoding = str::filter(encoding, " ");    // Remove spaces
    encodings = vec::filter(encodings, "");                                     // Remove empty values
    headers[ENCODINGS] = encodings;

    // Log
    log(ip() + " -> " + method + " " + path);
}

//                              default ""            default 0
void Request::respond(int code, const string message, const int options) { 
    Response *response = new Response(code, message, headers[ENCODINGS]);
    string headers = response->headers();
    string msg = response->msg();
    log(std::to_string(response->get_code()) + " -> " + ip() + (msg.empty() ? "" : ":"));
    if (!msg.empty()) log(msg);
    //              Returns a char* from the str
    send(client_fd, headers.c_str(), headers.size(), options);
    send(client_fd, msg.c_str(), msg.size(), options);
}
void Request::respond(int code, std::ifstream *file, const int options) {
    // Header
    Response *response = new Response(code, file, headers[ENCODINGS]);
    string headers = response->headers();
    
    log(std::to_string(response->get_code()) + " -> " + ip() + " (file)");
    send(client_fd, headers.c_str(), headers.size(), 0);

    // Body
    char buffer[BUFFER_SIZE];
    //     false when reached EOF with unfilled buffer
    while (file->read(buffer, BUFFER_SIZE) || file->gcount() > 0) {
        ssize_t bytes_read = file->gcount();
        ssize_t bytes_sent = 0;

        // Send the data through the client socket
        while (bytes_sent < bytes_read) {
            ssize_t result = send(client_fd, buffer + bytes_sent, bytes_read - bytes_sent, 0);
            if (result == -1) { throw "Error writing to socket."; }
            bytes_sent += result;
        }
    }
}

vector<string> Request::get_path() { return path_components; }
string Request::full_path() { return path; }
vector<string> Request::header(string key) { return headers[key]; }
string Request::get_method() { return method; }
string Request::get_body() { return body; }

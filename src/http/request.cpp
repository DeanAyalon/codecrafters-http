#include "request.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>

#include "../utils/console.hpp"
#include "../utils/str.hpp"

#define BUFFER_SIZE 1024

using std::string;
using std::vector;

Request::Request() {}

string Request::ip() {
    //            num to addr                   .s_addr = IP as binary (32b)
    return string(inet_ntoa(client_addr.sin_addr));
}

void Request::accept(int server_fd) {
    socklen_t len = sizeof(client_addr); // Must be declared in memory for there to be a pointer to give ::accept()
    //          global-scoped (from socket.h)
    client_fd = ::accept(server_fd, (struct sockaddr *)&client_addr, &len);

    // Read contents
    char buffer[BUFFER_SIZE] = {0}; // 1MB
    if (recv(client_fd, buffer, BUFFER_SIZE, 0) < 0)
        throw "Message receival failed";
    contents = string(buffer);

    // Analyze header
    string header = split(contents, "\r\n\r\n")[0];
    vector<string> headerLines = split(header, "\r\n");
    vector<string> title = split(headerLines[0], " ");
    method = title[0];
    path = title[1];
    pathComponents = filter(split(path, "/"), "");
    for (int i = 1; i < headerLines.size(); i++) {
        const vector<string> line = split(headerLines[i], ": ");
        if (line.size() < 2) continue; // skip malformed lines

        const string key = line[0];
        const vector<string> values = { line[1] };

        for (const auto &value : values) headers[key].push_back(value);
    }

    // Log
    log(ip() + " -> " + method + " " + path);
}

void Request::respond(int code, string message) { respond(new Response(code, message), 0); }
void Request::respond(Response *response, int options) {
    string headers = response->headers();
    string msg = response->getMessage();
    log(std::to_string(response->getCode()) + " -> " + ip() + (msg.empty() ? "" : ":"));
    if (!msg.empty())
        log(msg);
    //              Returns a char* from the str
    send(client_fd, headers.c_str(), headers.size(), options);
    send(client_fd, msg.c_str(), msg.size(), options);
}

vector<string> Request::getPath() { return pathComponents; }
string Request::fullPath() { return path; }

vector<string> Request::getHeader(string key) { return headers[key]; }
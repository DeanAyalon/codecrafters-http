#include "request.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>

#include "../utils/str.hpp"
#include "../utils/console.hpp"

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
    if (recv(client_fd, buffer, BUFFER_SIZE, 0) < 0) throw "Message receival failed";
    contents = string(buffer);

    // Analyze header
    string header = split(contents, "\r\n\r\n")[0];
    vector<string> headerLines = split(header, "\r\n");
    vector<string> title = split(headerLines[0], " ");
    method = title[0];
    path = title[1];
    pathComponents = filter(split(path, "/"), "");

    // Log
    log(ip() + " -> " + method + " " + path);
}

void Request::respond(Response *response, int options) {
    string headers = response->headers();
    log(std::to_string(response->getCode()) + " -> " + ip());
    //              Returns a char* from the str
    send(client_fd, headers.c_str(), headers.size(), options);
}

vector<string> Request::getPath() { return pathComponents; }
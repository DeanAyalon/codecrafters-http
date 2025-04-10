#include "request.hpp"

#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>

using std::string;

Request::Request() {
    // struct sockaddr_in client_address;
    // client_addr = &client_address;
    // length
}

string Request::ip() {
    //            num to addr                 .s_addr = IP as binary (32b)
    return string(inet_ntoa(client_addr.sin_addr));
}

void Request::accept(int server_fd) {
    socklen_t len = sizeof(client_addr);
    //          global-scoped (from socket.h)
    client_fd = ::accept(server_fd, (struct sockaddr *)&client_addr, &len);
}

void Request::respond(Response *response, int options) {
    string headers = response->headers();
    std::cout << "-> " + std::to_string(response->getCode());
    //              Returns a char* from the str
    send(client_fd, headers.c_str(), headers.size(), options);
}

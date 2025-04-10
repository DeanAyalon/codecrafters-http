#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "http/request.hpp"
#include "http/response.hpp"

using namespace std; // No need for `std::`

int crash(string msg) {
    cerr << msg;
    return 1;
}

int main(int argc, char **argv) {
    // Flush after every cout / cerr
    cout << unitbuf;
    cerr << unitbuf;

    //  file descriptor    IPv4     TCP          Default protocol
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        return crash("Failed to create server socket\n");

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors
    int reuse = 1; // int (4B)
    //                        Socket-wide               pointer=1B  Usually, along with a pointer, the size of the object in memory is transported
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
        return crash("Socket options failed\n");

    // New occurence of struct (like type) sockaddr_in
    //  Properties are not mandatory
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;         // TCP
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    server_addr.sin_port = htons(4221);       // int16

    //  globally-scoped   Not primitive - instanciate
    if (::bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
        return crash("Failed to bind to port 4221\n");

    int connection_backlog = 5; // Max pending connections
    if (listen(server_fd, connection_backlog) != 0)
        return crash("Listen failed\n");

    Request *request = new Request();
    cout << "Waiting for a client to connect...\n";

    request->accept(server_fd);
    cout << "Client connected - " + request->ip() + "\n";

    //               new returns a pointer
    request->respond(new Response(200), 0);

    close(server_fd);
    return 0;
}

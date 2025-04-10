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
#include "utils/console.hpp"
#include "utils/str.hpp"

int main(int argc, char **argv) {
    // Flush after every cout / cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    //  file descriptor    IPv4     TCP          Default protocol
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
        return error("Failed to create server socket");

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors
    int reuse = 1; // int (4B)
    //                        Socket-wide               pointer=1B  Usually, along with a pointer, the size of the object in memory is transported
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0)
        return error("Socket options failed");

    // New occurence of struct (like type) sockaddr_in
    //  Properties are not mandatory
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;         // TCP
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    server_addr.sin_port = htons(4221);       // int16

    //  globally-scoped   Not primitive - instanciate
    if (::bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
        return error("Failed to bind to port 4221");

    int connection_backlog = 5; // Max pending connections
    if (listen(server_fd, connection_backlog) != 0)
        return error("Listen failed");

    Request *request = new Request();
    log("Waiting for a client to connect...");

    request->accept(server_fd);

    // Routing
    vector<string> path = request->getPath();
    if (path.size() == 0) request->respond(200, "Welcome");
    else if (path[0] == "echo") request->respond(200, path[1]);
    else if (path[0] == "user-agent") request->respond(200, request->getHeader("User-Agent")[0]);
    else request->respond(404, request->fullPath() + " not found");

    close(server_fd);
    return 0;
}

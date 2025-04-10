#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <thread>

#include "http/request.hpp"
#include "utils/console.hpp"
#include "utils/utils.hpp"
#include "files.hpp"

struct sockaddr_in accepted_addr;
socklen_t length = sizeof(accepted_addr); // Must be declared in memory for there to be a pointer to give ::accept()
string files_dir;

void handle_request(int client) {
    Request *request = new Request();
    request->accept(client, &accepted_addr);

    // Routing
    vector<string> path = request->get_path();
    if (path.size() == 0) request->respond(200, "Welcome");
    else if (path[0] == "echo") request->respond(200, path[1]);
    else if (path[0] == "user-agent") request->respond(200, request->header("User-Agent")[0]);
    else if (path[0] == "files") files(request, files_dir);
    else request->respond(404, request->full_path() + " not found");

    close(client);
}

int main(int argc, char **argv) {
    // Flush after every cout / cerr
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    const int dir_pos = arr::position(argv, argc, "--directory");
    if (dir_pos > -1 && argc > dir_pos + 1) files_dir = argv[dir_pos + 1];
    else {
        files_dir = "./files";
        log("--directory not specified, using ./files as default file directory");
    }

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

    //  Globally-scoped    Type casting
    if (::bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0)
        return error("Failed to bind to port 4221");
    log("Listening on port 4221");

    int connection_backlog = 5; // Max pending connections
    if (listen(server_fd, connection_backlog) != 0)
        return error("Listen failed");

    while (true) {
        const int client = accept(server_fd, (struct sockaddr *)&accepted_addr, &length);
        //            () => handle_request(client)
        std::thread t([client]() { handle_request(client); });
        t.detach();
    }
    
    close(server_fd);
    return 0;
}

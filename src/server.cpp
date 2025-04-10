#include <arpa/inet.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;    // No need for `std::`

int main(int argc, char **argv) {
    // Flush after every cout / cerr
    cout << unitbuf;
    cerr << unitbuf;

    //  file descriptor    IPv4     TCP          Default protocol
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        cerr << "Failed to create server socket\n";
        return 1;
    }

    // Since the tester restarts your program quite often, setting SO_REUSEADDR
    // ensures that we don't run into 'Address already in use' errors
    int reuse = 1; // int (4B)
    //                        Socket-wide               pointer=1B
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        cerr << "Socket options failed\n";
        return 1;
    }

    // New occurence of struct (like type) sockaddr_in
    //  Properties are not mandatory
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;         // TCP
    server_addr.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    server_addr.sin_port = htons(4221);       // int16

    //  globally-scoped   Not primitive - instanciate
    if (::bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        cerr << "Failed to bind to port 4221\n";
        return 1;
    }

    int connection_backlog = 5; // Max pending connections
    if (listen(server_fd, connection_backlog) != 0) {
        cerr << "listen failed\n";
        return 1;
    }

    struct sockaddr_in client_addr;
    int client_addr_len = sizeof(client_addr);
    cout << "Waiting for a client to connect...\n";

    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t *)&client_addr_len); 
    //                                  .s_addr = IP as binary (32b)
    char *ipv4 = inet_ntoa(client_addr.sin_addr);
    cout << "Client connected - " + string(ipv4) + "\n";
    
    string response = "HTTP/1.1 200 OK\r\n\r\n";
    //              Returns a char* from the str       options
    send(client_fd, response.c_str(), response.size(), 0);
    cout << "-> " + string(ipv4) + " 200\n";

    // close(client_fd);
    close(server_fd);
    return 0;
}

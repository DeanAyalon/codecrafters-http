#pragma once // Put in header files (.hpp) so they do not compile more than once

#include <arpa/inet.h>
#include <vector>
#include <map>

#include "response.hpp"

using std::string;
using std::vector;

class Request {
  public:
    Request();
    string ip();
    void accept(int server_fd);
    void respond(int code, string message);
    void respond(Response *response, int options);
    vector<string> getPath();
    string fullPath();
    vector<string> getHeader(string key);

  private:
    sockaddr_in client_addr;
    int client_fd;
    string contents;
    string method;
    string path;
    vector<string> pathComponents;
    std::map<string, vector<string>> headers;
};
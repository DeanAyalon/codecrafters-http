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
    void accept(int client, sockaddr_in *address);
    void respond(int code, string message);
    void respond(Response *response, int options);
    vector<string> get_path();
    string full_path();
    vector<string> header(string key);

  private:
    sockaddr_in client_addr;
    int client_fd;
    string contents;
    string method;
    string path;
    vector<string> path_components;
    std::map<string, vector<string>> headers;
};
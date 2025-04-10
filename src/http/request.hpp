#pragma once // Put in header files (.hpp) so they do not compile more than once

#include <iostream>
#include <arpa/inet.h>

#include "response.hpp"

class Request {
  public:
    Request();
    std::string ip();
    void accept(int server_fd);
    void respond(Response *response, int options);

  private:
    sockaddr_in client_addr;
    int client_fd;
};
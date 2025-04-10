#pragma once

#include <iostream>

class Response {
  public:
    Response(int code);
    std::string headers();
    int getCode();

  private:
    int code;
    std::string status();
};
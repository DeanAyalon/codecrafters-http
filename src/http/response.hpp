#pragma once

#include <string>

using std::string;

class Response {
  public:
    Response(int code);
    string headers();
    int getCode();

  private:
    int code;
    string status();
};
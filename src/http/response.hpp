#pragma once

#include <string>

using std::string;

class Response {
  public:
    Response(int code);
    Response(int code, string message);
    string headers();
    int getCode();
    string getMessage();

  private:
    int code;
    string message;
    string status();
    string contentType;
};
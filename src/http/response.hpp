#pragma once

#include <string>
#include <fstream>

using std::string;
using std::ifstream;

class Response {
  public:
    Response(int code);
    Response(int code, string message);
    Response(int code, ifstream *file);
    string headers();
    int get_code();
    string msg();

  private:
    int code;
    string message;
    ifstream *file;

    string status();
    string content_type;
};
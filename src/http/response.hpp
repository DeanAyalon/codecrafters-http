#pragma once

#include <string>
#include <fstream>

using std::string;
using std::ifstream;

class Response {
  public:
    Response(int code, string message = "", string encoding = "");
    Response(int code, ifstream *file, string encoding = "");
    string headers();
    int get_code();
    string msg();

  private:
    int code;
    string message;
    ifstream *file;

    string status();
    string content_type;
    string encoding;
};
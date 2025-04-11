#pragma once

#include <string>
#include <vector>
#include <fstream>

using std::string;
using std::vector;
using std::ifstream;

class Response {
  public:
    Response(int code, string message = "", vector<string> encodings = {});
    Response(int code, ifstream *file, vector<string> encodings = {});
    string headers();
    int get_code();
    string msg();
    
    private:
    int code;
    string message;
    string encoded_msg;
    ifstream *file;
    
    string status();
    string content_type;
    string encoding;
    void encode(vector<string> encodings);
};
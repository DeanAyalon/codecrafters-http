#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

string selectEncoding(vector<string> encodings);
string encode(const string &data, const string &encoding);
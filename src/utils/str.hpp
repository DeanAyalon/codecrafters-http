#pragma once

#include <string>

using std::string;
using std::vector;

vector<string> split(const string &input, const string &delimiter);

string join(const vector<string> &parts, const string &delimiter);
vector<string> filter(const vector<string> &v, const string &element);

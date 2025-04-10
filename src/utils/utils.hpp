#pragma once

#include <string>

using std::string;
using std::vector;

namespace str {
vector<string> split(const string &input, const string &delimiter);
} // namespace str

namespace vec {
string join(const vector<string> &parts, const string &delimiter);
vector<string> filter(const vector<string> &v, const string &element);
} // namespace vec

namespace arr {
int position(char **arr, int size, const char *value);
} // namespace arr
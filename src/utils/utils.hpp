#pragma once

#include <string>
#include <vector>

using std::string;
using std::vector;

namespace str {
// Splits a string (input) into a vector using (delimiter). Optionally limit splits
vector<string> split(const string &input, const string &delimiter, const int limit = -1);

// Removes all occurrences of (filter) from string (original)
string filter(const string &original, const string &filter);
} // namespace str

namespace vec {
// Joins a string vector (parts) into a single string, stitching with delimiter. ex: (["a", "b"], "-") -> "a-b"
string join(const vector<string> &parts, const string &delimiter);

// Filters a string vector (v) from all elements matching given string (element)
vector<string> filter(const vector<string> &v, const string &element);
} // namespace vec

namespace arr {
// Find the index of a given string (value) within a char* (arr) of given size
int position(char **arr, int size, const char *value);
} // namespace arr
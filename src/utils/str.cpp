#include <string>
#include <vector>

using std::string;
using std::vector;

// Array of unknown length
vector<string> split(const string &input, const string &delimiter) {
    vector<string> parts;
    size_t start = 0, end;
    //                                                     Not found
    while ((end = input.find(delimiter, start)) != string::npos) {
        //                                  length (not end position)
        parts.push_back(input.substr(start, end - start));
        start = end + delimiter.length();
    }
    parts.push_back(input.substr(start)); // add the last part
    return parts;
}

string join(const vector<string> &parts, const string &delimiter) {
    if (parts.empty()) return "";

    string result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) result += delimiter + parts[i];

    return result;
}

vector<string> filter(const vector<string> &v, const string &element) {
    vector<string> result;
    for (int i = 0; i < v.size(); i++) 
        if (v[i] != element) result.push_back(v[i]);
    return result;
}

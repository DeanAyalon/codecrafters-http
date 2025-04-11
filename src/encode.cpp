#include "encode.hpp"

#include <string>
#include <vector>
#include <algorithm>

#include "utils/console.hpp"
#include "utils/utils.hpp"

using std::string;
using std::vector;

string selectEncoding(vector<string> encodings) {
    // Supported encodings
    if (std::find(encodings.begin(), encodings.end(), "gzip") != encodings.end()) return "gzip";

    return "";
}
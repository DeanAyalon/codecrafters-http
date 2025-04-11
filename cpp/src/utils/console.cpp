#include "console.hpp"

#include <iostream>

using std::string;

void log(string message) { std::cout << message + "\n"; }
int error(string message) { std::cerr << message + "\n"; return 1; }
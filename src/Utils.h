#pragma once

#include <vector>
#include <string>

using std::string;
using std::vector;

class Utils {
public:
    /**
     * Splits the string provided to a several strings (tokens) based on the separator provided
     */
    static vector<std::string> tokenize(const string& s, char separator);
};
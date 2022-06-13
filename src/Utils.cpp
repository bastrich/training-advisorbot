#include <iostream>
#include <fstream>
#include "Utils.h"

using std::string;
using std::stod;
using std::vector;
using std::cout;
using std::endl;
using std::exception;

vector<string> Utils::tokenize(const string& s, char separator) {
    vector<string> tokens;
    signed int start, end;
    string token;
    start = s.find_first_not_of(separator, 0);
    do {
        end = s.find_first_of(separator, start);
        if (start == s.length() || start == end) break;
        if (end >= 0) token = s.substr(start, end - start);
        else token = s.substr(start, s.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);

    return tokens;
}
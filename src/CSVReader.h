#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>

using std::string;
using std::vector;

class CSVReader {
public:
    CSVReader();
    static vector<OrderBookEntry> readCSV(string csvFile);
private:
    static vector<std::string> tokenise(string csvLine, char separator);
    static OrderBookEntry stringsToOBE(vector<string> strings);
};
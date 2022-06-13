#pragma once

#include <vector>
#include <string>
#include "OrderBookEntry.h"

using std::string;
using std::vector;

class CSVReader {
public:
    CSVReader();

    /**
     * Read and parse CSV from the file provided
     */
    static vector<OrderBookEntry> readCSV(const string &csvFile);

private:
    static OrderBookEntry stringsToOBE(const vector<string>& strings);
};
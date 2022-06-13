#include <iostream>
#include <fstream>
#include "CSVReader.h"
#include "Utils.h"

using std::string;
using std::stod;
using std::vector;
using std::cout;
using std::endl;
using std::exception;

CSVReader::CSVReader() = default;

vector<OrderBookEntry> CSVReader::readCSV(const string &csvFilename) {
    vector<OrderBookEntry> entries;

    std::ifstream csvFile{csvFilename};
    string line;
    if (csvFile.is_open()) {
        while (std::getline(csvFile, line)) {
            try {
                OrderBookEntry obe = stringsToOBE(Utils::tokenize(line, ','));
                entries.push_back(obe);
            } catch (const exception &e) {
                cout << "CSVReader::readCSV bad data" << endl;
            }
        } // end of while
    }

    cout << "CSVReader::readCSV read " << entries.size() << " entries" << endl;
    return entries;
}

/**
 * Parse string tokens and create OrderBookEntry
 */
OrderBookEntry CSVReader::stringsToOBE(const vector<string>& tokens) {
    double price, amount;

    if (tokens.size() != 5) // bad
    {
        cout << "Bad line " << endl;
        throw exception{};
    }
    // we have 5 tokens
    try {
        price = stod(tokens[3]);
        amount = stod(tokens[4]);
    } catch (const exception &e) {
        cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << endl;
        cout << "CSVReader::stringsToOBE Bad float! " << tokens[4] << endl;
        throw;
    }

    OrderBookEntry obe{price,
                       amount,
                       tokens[0],
                       tokens[1],
                       OrderBookEntry::stringToOrderBookType(tokens[2])};

    return obe;
}
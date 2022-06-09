#include <iostream>
#include <fstream>
#include "CSVReader.h"

using std::string;
using std::stod;
using std::vector;
using std::cout;
using std::endl;
using std::exception;

CSVReader::CSVReader() {

}

vector<OrderBookEntry> CSVReader::readCSV(string csvFilename) {
    vector<OrderBookEntry> entries;

    std::ifstream csvFile{csvFilename};
    string line;
    if (csvFile.is_open()) {
        while (std::getline(csvFile, line)) {
            try {
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe);
            } catch (const exception &e) {
                cout << "CSVReader::readCSV bad data" << endl;
            }
        }// end of while
    }

    cout << "CSVReader::readCSV read " << entries.size() << " entries" << endl;
    return entries;
}

vector<string> CSVReader::tokenise(string csvLine, char separator) {
    vector<string> tokens;
    signed int start, end;
    string token;
    start = csvLine.find_first_not_of(separator, 0);
    do {
        end = csvLine.find_first_of(separator, start);
        if (start == csvLine.length() || start == end) break;
        if (end >= 0) token = csvLine.substr(start, end - start);
        else token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);

    return tokens;
}

OrderBookEntry CSVReader::stringsToOBE(vector<string> tokens) {
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
#pragma once

#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"

using std::string;
using std::vector;

class OrderBook {
public:
    /** construct, reading a csv data file */
    OrderBook(string filename);

    /** return vector of all know products in the dataset*/
    vector<string> getKnownProducts();

    /** return vector of Orders according to the sent filters*/
    vector<double> getOrderPrices(OrderBookType type, const string& product, const string& timestamp);
    vector<double> getOrderMinPricesBefore(OrderBookType type, const string& product, const string& timestamp);
    vector<double> getOrderMaxPricesBefore(OrderBookType type, const string& product, const string& timestamp);
    vector<double> getOrderPricesForLastTimesteps(OrderBookType type, const string& product, const string& timestamp, unsigned int timesteps);

    /** returns the earliest time in the orderbook*/
    string getEarliestTime();

    /** returns the next time after the
     * sent time in the orderbook
     * If there is no next timestamp, wraps around to the start
     * */
    string getNextTime(string timestamp);

private:
    vector<OrderBookEntry> orders;


};

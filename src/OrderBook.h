#pragma once

#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"

using std::string;
using std::vector;

class OrderBook {
public:
    /**
     * Construct, reading a csv data file
     */
    OrderBook(const string& filename);

    /**
     * Return vector of all know products in the dataset
     */
    vector<string> getKnownProducts();

    /**
     * Return vector of prices of orders according to the sent filters
     */
    vector<double> getOrderPrices(OrderBookType type, const string &product, const string &timestamp);

    /**
     * Return vector of min prices of orders for each timestamp according to the sent filters
     */
    vector<double> getOrderMinPricesBefore(OrderBookType type, const string &product, const string &timestamp);

    /**
     * Return vector of max prices of orders for each timestamp according to the sent filters
     */
    vector<double> getOrderMaxPricesBefore(OrderBookType type, const string &product, const string &timestamp);

    /**
     * Return vector of prices of orders for last @param timesteps timestamps
     * including @param timestamp according to the sent filters
     */
    vector<double> getOrderPricesForLastTimesteps(OrderBookType type, const string &product, const string &timestamp,
                                                  unsigned int timesteps);

    /**
     * Returns the earliest time in the orderbook
     */
    string getEarliestTime();

    /**
     * Returns the next time after the sent @param timestamp in the orderbook
     * If there is no next timestamp, wraps around to the start
     */
    string getNextTime(string timestamp);

private:
    vector<OrderBookEntry> orders;
};

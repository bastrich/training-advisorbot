#include <map>
#include <algorithm>
#include <set>
#include "OrderBook.h"
#include "CSVReader.h"

using std::string;
using std::set;
using std::vector;

/** construct, reading a csv data file */
OrderBook::OrderBook(string filename) {
    orders = CSVReader::readCSV(filename);
}

/** return vector of all know products in the dataset*/
vector<string> OrderBook::getKnownProducts() {
    vector<string> products;

    std::map<string, bool> prodMap;

    for (OrderBookEntry &e: orders) {
        prodMap[e.product] = true;
    }

    // now flatten the map to a vector of strings
    for (auto const &e: prodMap) {
        products.push_back(e.first);
    }

    return products;
}

/** return vector of Orders according to the sent filters*/
vector<double> OrderBook::getOrderPrices(OrderBookType type, const string& product, const string& timestamp) {
    vector<double> prices;
    for (OrderBookEntry &e: orders) {
        if (e.orderType == type &&
            e.product == product &&
            e.timestamp == timestamp) {
            prices.push_back(e.price);
        }
    }
    return prices;
}

/** return vector of Orders according to the sent filters*/
vector<double> OrderBook::getOrderPricesForLastTimesteps(OrderBookType type,
                                                            const string& product,
                                                            const string& timestamp,
                                                            unsigned int timesteps) {
    vector<double> prices;
    string lastPreviousTimestamp;
    int lastPreviousTimestampUpdateCount = 1;
    for (auto orderBookEntry = orders.rbegin(); orderBookEntry != orders.rend(); ++orderBookEntry) {

        if (orderBookEntry->timestamp == timestamp &&
            orderBookEntry->orderType == type &&
            orderBookEntry->product == product) {
            prices.push_back(orderBookEntry->price);
            continue;
        }

        if (orderBookEntry->timestamp < timestamp) {
            if (lastPreviousTimestamp != orderBookEntry->timestamp) {
                lastPreviousTimestamp = orderBookEntry->timestamp;
                lastPreviousTimestampUpdateCount++;
            }

            if (lastPreviousTimestampUpdateCount > timesteps) {
                break;
            }

            if (orderBookEntry->orderType == type &&
                orderBookEntry->product == product) {
                prices.push_back(orderBookEntry->price);
                continue;
            }
        }
    }

    return prices;
}

string OrderBook::getEarliestTime() {
    return orders[0].timestamp;
}

string OrderBook::getNextTime(string timestamp) {
    string next_timestamp = "";
    for (OrderBookEntry &e: orders) {
        if (e.timestamp > timestamp) {
            next_timestamp = e.timestamp;
            break;
        }
    }
    if (next_timestamp == "") {
        next_timestamp = orders[0].timestamp;
    }
    return next_timestamp;
}

vector<double> OrderBook::getOrderMinPricesBefore(OrderBookType type, const string& product, const string& timestamp) {
    vector<double> prices;

    string currentTimestamp;
    double minForCurrentTimestamp = 0;

    for (OrderBookEntry &e: orders) {
        if (e.orderType != type ||
            e.product != product) {
            continue;
        }

        if (currentTimestamp.empty()) {
            currentTimestamp = e.timestamp;
            minForCurrentTimestamp = e.price;
        }

        if (e.timestamp != currentTimestamp) {
            prices.push_back(minForCurrentTimestamp);
            currentTimestamp = e.timestamp;
            minForCurrentTimestamp = e.price;
        }

        if (minForCurrentTimestamp < e.price) {
            minForCurrentTimestamp = e.price;
        }
    }

    return prices;
}

vector<double> OrderBook::getOrderMaxPricesBefore(OrderBookType type, const string& product, const string& timestamp) {
    vector<double> prices;

    string currentTimestamp;
    double maxForCurrentTimestamp = 0;

    for (OrderBookEntry &e: orders) {
        if (e.orderType != type ||
            e.product != product) {
            continue;
        }

        if (currentTimestamp.empty()) {
            currentTimestamp = e.timestamp;
            maxForCurrentTimestamp = e.price;
        }

        if (e.timestamp != currentTimestamp) {
            prices.push_back(maxForCurrentTimestamp);
            currentTimestamp = e.timestamp;
            maxForCurrentTimestamp = e.price;
        }

        if (maxForCurrentTimestamp > e.price) {
            maxForCurrentTimestamp = e.price;
        }
    }

    return prices;
}

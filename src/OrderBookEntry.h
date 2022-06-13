#pragma once

#include <string>

using std::string;

enum class OrderBookType {
    bid, ask, unknown
};

class OrderBookEntry {
public:
    OrderBookEntry(double _price,
                   double _amount,
                   string _timestamp,
                   string _product,
                   OrderBookType _orderType,
                   string username = "dataset");
    /**
     * Create an object from the string provided
     */
    static OrderBookType stringToOrderBookType(const string& s);

    /**
     * Return string representation of @class OrderBookType
     */
    static string orderBookTypeToString(OrderBookType orderBookType);

    double price;
    double amount;
    string timestamp;
    string product;
    OrderBookType orderType;
    string username;
};

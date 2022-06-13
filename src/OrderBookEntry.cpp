#include "OrderBookEntry.h"

#include <utility>

using std::string;

OrderBookEntry::OrderBookEntry(double price,
                               double amount,
                               string timestamp,
                               string product,
                               OrderBookType orderType,
                               string username)
        : price(price),
          amount(amount),
          timestamp(std::move(timestamp)),
          product(std::move(product)),
          orderType(orderType),
          username(std::move(username)) {
}

OrderBookType OrderBookEntry::stringToOrderBookType(const string& s) {
    if (s == "ask") {
        return OrderBookType::ask;
    }
    if (s == "bid") {
        return OrderBookType::bid;
    }
    return OrderBookType::unknown;
}

string OrderBookEntry::orderBookTypeToString(OrderBookType orderBookType) {
    switch (orderBookType) {
        case OrderBookType::bid:
            return "bid";
        case OrderBookType::ask:
            return "ask";
        default:
            return "unknown";
    }
}

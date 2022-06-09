#include <iostream>
#include <vector>
#include <set>
#include "AdvisorbotMain.h"
#include "OrderBookEntry.h"
#include "Calculations.h"

using std::string;
using std::to_string;
using std::exception;
using std::stoi;

AdvisorbotMain::AdvisorbotMain() {

}

void AdvisorbotMain::init() {
    currentTime = orderBook.getEarliestTime();

    vector<string> knownProducts = orderBook.getKnownProducts();
    availableProducts.insert(knownProducts.begin(), knownProducts.end());

    string input;
    while (true) {
        printWelcomeMessage();
        input = readUserInput();
        processUserInput(input);
    }
}

/**
 * Prints welcome message
 */
void AdvisorbotMain::printWelcomeMessage() {
    printAdvisorbotOutput("Please enter a command, or 'help' for a list of commands");
}

/**
 * Reads and returns user input as string
 */
string AdvisorbotMain::readUserInput() {
    std::cout << "\033[1;34muser> \033[0m";
    string input;
    std::getline(std::cin, input);
    return input;
}

/**
 * Parses and processes user input
 */
void AdvisorbotMain::processUserInput(string input) {
    vector<string> tokens = tokeniseUserInput(input);

    if (tokens.empty()) {
        printAdvisorbotOutput("You typed nothing");
        return;
    }

    string command = tokens[0];
    command_processing_method commandProcessingMethod = commandProcessingMethodsMap[command];
    if (commandProcessingMethod == nullptr) {
        printAdvisorbotOutput("Command '" + command + "' doesn't exist, use 'help' to list available commands");
        return;
    }

    ((*this).*(commandProcessingMethod))(tokens);
}

vector<string> AdvisorbotMain::tokeniseUserInput(string input) {
    char separator = ' ';
    vector<string> tokens;
    signed int start, end;
    string token;
    start = input.find_first_not_of(separator, 0);
    do {
        end = input.find_first_of(separator, start);
        if (start == input.length() || start == end) break;
        if (end >= 0) token = input.substr(start, end - start);
        else token = input.substr(start, input.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while (end > 0);

    return tokens;
}

void AdvisorbotMain::processCommand_help(const vector<string>& tokens) {
    if (tokens.size() == 1) {
        printAdvisorbotOutput("The available commands are "
                              "\n * help - list all available commands"
                              "\n * help <cmd> - output help for the specified command"
                              "\n * prod - list available products"
                              "\n * min - find minimum bid or ask for product in current time step"
                              "\n * max - find maximum bid or ask for product in current time step"
                              "\n * avg - compute average ask or bid for the sent product over the sent number of time steps"
                              "\n * predict - predict max/min product ask/bid"
                              "\n * time - state current time in dataset, i.e. which timeframe are we looking at"
                              "\n * step - move to next time step"
                              "\n * percentile - calculate nth percentile of ask or bid for the sent product over the sent number of time steps"
                              "\n * exit - finish execution"
        );
        return;
    }
    if (tokens.size() == 2) {
        string command = tokens[1];
        string commandHelp = commandHelpMap[command];
        if (commandHelp.empty()) {
            printAdvisorbotOutput("Help for command '" + command + "' isn't defined");
        } else {
            printAdvisorbotOutput(commandHelp);
        }
        return;
    }
    printAdvisorbotOutput("Invalid syntax for 'help' command, check 'help help'");
}

void AdvisorbotMain::processCommand_prod(const vector<string>& tokens) {
    if (tokens.size() > 1) {
        printAdvisorbotOutput("Invalid syntax for 'prod' command, check 'help prod'");
    }

    string products;
    for (string const &product: availableProducts) {
        products += product + ",";
    }
    products.pop_back();
    printAdvisorbotOutput(products);
}

void AdvisorbotMain::processCommand_min(const vector<string>& tokens) {
    if (tokens.size() != 3) {
        printAdvisorbotOutput("Invalid syntax for 'min' command, check 'help min'");
    }

    string product = tokens[1];
    if (availableProducts.find(product) == availableProducts.end()) {
        printAdvisorbotOutput("Product '" + product + "' doesn't exist");
        return;
    }

    OrderBookType orderBookType = OrderBookEntry::stringToOrderBookType(tokens[2]);
    if (orderBookType == OrderBookType::unknown) {
        printAdvisorbotOutput("Order book type provided is unknown, should be 'ask' or 'bid'");
        return;
    }

    vector<double> prices = orderBook.getOrderPrices(orderBookType, product, currentTime);
    if (prices.empty()) {
        printAdvisorbotOutput("There are no orders for the given product and order type");
        return;
    }

    printAdvisorbotOutput("The min " + OrderBookEntry::orderBookTypeToString(orderBookType) + " for " + product +
                          " is " + to_string(Calculations::min(prices)));
}

void AdvisorbotMain::processCommand_max(const vector<string>& tokens) {
    if (tokens.size() != 3) {
        printAdvisorbotOutput("Invalid syntax for 'max' command, check 'help max'");
    }

    string product = tokens[1];
    if (availableProducts.find(product) == availableProducts.end()) {
        printAdvisorbotOutput("Product '" + product + "' doesn't exist");
        return;
    }

    OrderBookType orderBookType = OrderBookEntry::stringToOrderBookType(tokens[2]);
    if (orderBookType == OrderBookType::unknown) {
        printAdvisorbotOutput("Order book type provided is unknown, should be 'ask' or 'bid'");
        return;
    }

    vector<double> prices = orderBook.getOrderPrices(orderBookType, product, currentTime);
    if (prices.empty()) {
        printAdvisorbotOutput("There are no orders for the given product and order type");
        return;
    }

    printAdvisorbotOutput("The max " + OrderBookEntry::orderBookTypeToString(orderBookType) + " for " + product +
                          " is " + to_string(Calculations::max(prices)));
}

void AdvisorbotMain::processCommand_avg(const vector<string>& tokens) {
    if (tokens.size() != 4) {
        printAdvisorbotOutput("Invalid syntax for 'avg' command, check 'help avg'");
    }

    string product = tokens[1];
    if (availableProducts.find(product) == availableProducts.end()) {
        printAdvisorbotOutput("Product '" + product + "' doesn't exist");
        return;
    }

    OrderBookType orderBookType = OrderBookEntry::stringToOrderBookType(tokens[2]);
    if (orderBookType == OrderBookType::unknown) {
        printAdvisorbotOutput("Order book type provided is unknown, should be 'ask' or 'bid'");
        return;
    }

    unsigned int timesteps;
    try {
        timesteps = stoi(tokens[3]);
    } catch (const exception &e) {
        printAdvisorbotOutput("timesteps should positive integer number");
        return;
    }

    vector<double> prices = orderBook.getOrderPricesForLastTimesteps(orderBookType, product, currentTime,
                                                                        timesteps);
    if (prices.empty()) {
        printAdvisorbotOutput("There are no orders for the given product and order type");
        return;
    }

    printAdvisorbotOutput("The average " + product + " " + OrderBookEntry::orderBookTypeToString(orderBookType) +
                          " price over the last " +
                          to_string(timesteps) + " timesteps was " + to_string(Calculations::avg(prices)));
}

void AdvisorbotMain::processCommand_predict(const vector<string>& tokens) {
    if (tokens.size() != 4) {
        printAdvisorbotOutput("Invalid syntax for 'max' command, check 'help predict'");
    }

    string minOrMax = tokens[1];
    if (minOrMax != "min" && minOrMax != "max") {
        printAdvisorbotOutput("The 1st parameter should 'min' or 'max', check 'help predict'");
        return;
    }

    string product = tokens[2];
    if (availableProducts.find(product) == availableProducts.end()) {
        printAdvisorbotOutput("Product '" + product + "' doesn't exist");
        return;
    }

    OrderBookType orderBookType = OrderBookEntry::stringToOrderBookType(tokens[3]);
    if (orderBookType == OrderBookType::unknown) {
        printAdvisorbotOutput("Order book type provided is unknown, should be 'ask' or 'bid'");
        return;
    }

    vector<double> prices;
    if (minOrMax == "min") {
        prices = orderBook.getOrderMinPricesBefore(orderBookType, product, currentTime);
    } else {
        prices = orderBook.getOrderMaxPricesBefore(orderBookType, product, currentTime);
    }
    if (prices.empty()) {
        printAdvisorbotOutput("There are no orders for the given product and order type");
        return;
    }

    printAdvisorbotOutput("The max " + OrderBookEntry::orderBookTypeToString(orderBookType) + " for " + product +
                          " is " + to_string(Calculations::predict(prices)));
}


void AdvisorbotMain::processCommand_time(const vector<string>& tokens) {
    if (tokens.size() != 1) {
        printAdvisorbotOutput("Invalid syntax for 'time' command, check 'help time'");
    }

    printAdvisorbotOutput(currentTime);
}

void AdvisorbotMain::processCommand_step(const vector<string>& tokens) {
    if (tokens.size() != 1) {
        printAdvisorbotOutput("Invalid syntax for 'step' command, check 'help step'");
    }

    currentTime = orderBook.getNextTime(currentTime);

    printAdvisorbotOutput("now at " + currentTime);
}

void AdvisorbotMain::processCommand_percentile(const vector<string>& tokens) {
    if (tokens.size() != 5) {
        printAdvisorbotOutput("Invalid syntax for 'percentile' command, check 'help percentile'");
    }

    string product = tokens[1];
    if (availableProducts.find(product) == availableProducts.end()) {
        printAdvisorbotOutput("Product '" + product + "' doesn't exist");
        return;
    }

    OrderBookType orderBookType = OrderBookEntry::stringToOrderBookType(tokens[2]);
    if (orderBookType == OrderBookType::unknown) {
        printAdvisorbotOutput("Order book type provided is unknown, should be 'ask' or 'bid'");
        return;
    }

    unsigned int timesteps;
    try {
        timesteps = stoi(tokens[3]);
    } catch (const exception &e) {
        printAdvisorbotOutput("timesteps should positive integer number");
        return;
    }

    unsigned int percentile;
    try {
        percentile = stoi(tokens[4]);
        if (percentile < 1 || percentile > 100) {
            throw;
        }
    } catch (const exception &e) {
        printAdvisorbotOutput("percentile should positive integer number from 1 to 100");
        return;
    }

    vector<double> prices = orderBook.getOrderPricesForLastTimesteps(orderBookType, product, currentTime,
                                                                        timesteps);
    if (prices.empty()) {
        printAdvisorbotOutput("There are no orders for the given product and order type");
        return;
    }

    printAdvisorbotOutput(
            "The 50th percentile of  " + product + " " + OrderBookEntry::orderBookTypeToString(orderBookType) +
            " price over the last " +
            to_string(timesteps) + " timesteps was " + to_string(Calculations::nthPercentile(prices, percentile)));
}

void AdvisorbotMain::processCommand_exit(const vector<string>& tokens) {
    if (tokens.size() != 1) {
        printAdvisorbotOutput("Invalid syntax for 'exit' command, check 'help exit'");
    }

    printAdvisorbotOutput("Finishing program execution");
    std::exit(0);
}

void AdvisorbotMain::printAdvisorbotOutput(const string& message) {
    std::cout << "\033[1;31madvisorbot>\033[0m " << message << std::endl;
}
#include <iostream>
#include <vector>
#include <set>
#include "AdvisorbotMain.h"
#include "OrderBookEntry.h"
#include "Calculations.h"
#include "Utils.h"

using std::string;
using std::to_string;
using std::exception;
using std::stoi;

AdvisorbotMain::AdvisorbotMain() = default;

void AdvisorbotMain::init() {
    //init the 1st timestep
    currentTime = orderBook.getEarliestTime();

    //cache parsed products
    vector<string> knownProducts = orderBook.getKnownProducts();
    availableProducts.insert(knownProducts.begin(), knownProducts.end());

    //start input-output loop
    string input;
    while (true) {
        printWelcomeMessage();
        input = readUserInput();
        processUserInput(input);
    }
}

/**
 * Print welcome message
 */
void AdvisorbotMain::printWelcomeMessage() {
    printAdvisorbotOutput("Please enter a command, or 'help' for a list of commands");
}

/**
 * Read and return user input as a string
 */
string AdvisorbotMain::readUserInput() {
    //apply color style to user input cursor
    std::cout << "\033[1;34muser> \033[0m";
    string input;
    std::getline(std::cin, input);
    return input;
}

/**
 * Parse and process user input
 */
void AdvisorbotMain::processUserInput(const string &input) {
    //get abd verify command tokens
    vector<string> tokens = Utils::tokenize(input, ' ');

    if (tokens.empty()) {
        printAdvisorbotOutput("You typed nothing");
        return;
    }

    //find command processor
    string command = tokens[0];
    command_processing_method commandProcessingMethod = commandProcessingMethodsMap[command];
    if (commandProcessingMethod == nullptr) {
        printAdvisorbotOutput("Command '" + command + "' doesn't exist, use 'help' to list available commands");
        return;
    }

    //call command processor
    ((*this).*(commandProcessingMethod))(tokens);
}

/**
 * Process 'help' command
 */
void AdvisorbotMain::processCommand_help(const vector<string> &tokens) {
    //verify amount of tokens
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

/**
 * Process 'prod' command
 */
void AdvisorbotMain::processCommand_prod(const vector<string> &tokens) {
    //verify amount of tokens
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

/**
 * Process 'min' command
 */
void AdvisorbotMain::processCommand_min(const vector<string> &tokens) {
    //verify amount of tokens
    if (tokens.size() != 3) {
        printAdvisorbotOutput("Invalid syntax for 'min' command, check 'help min'");
    }

    //parse and verify product
    string product = tokens[1];
    if (availableProducts.find(product) == availableProducts.end()) {
        printAdvisorbotOutput("Product '" + product + "' doesn't exist");
        return;
    }

    //parse and verify order type
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

/**
 * Process 'max' command
 */
void AdvisorbotMain::processCommand_max(const vector<string> &tokens) {
    //verify amount of tokens
    if (tokens.size() != 3) {
        printAdvisorbotOutput("Invalid syntax for 'max' command, check 'help max'");
    }

    //parse and verify product
    string product = tokens[1];
    if (availableProducts.find(product) == availableProducts.end()) {
        printAdvisorbotOutput("Product '" + product + "' doesn't exist");
        return;
    }

    //parse and verify order type
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

/**
 * Process 'avg' command
 */
void AdvisorbotMain::processCommand_avg(const vector<string> &tokens) {
    //verify amount of tokens
    if (tokens.size() != 4) {
        printAdvisorbotOutput("Invalid syntax for 'avg' command, check 'help avg'");
    }

    //parse and verify product
    string product = tokens[1];
    if (availableProducts.find(product) == availableProducts.end()) {
        printAdvisorbotOutput("Product '" + product + "' doesn't exist");
        return;
    }

    //parse and verify order type
    OrderBookType orderBookType = OrderBookEntry::stringToOrderBookType(tokens[2]);
    if (orderBookType == OrderBookType::unknown) {
        printAdvisorbotOutput("Order book type provided is unknown, should be 'ask' or 'bid'");
        return;
    }

    //parse and verify number of timesteps
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

/**
 * Process 'predict' command
 */
void AdvisorbotMain::processCommand_predict(const vector<string> &tokens) {
    //verify amount of tokens
    if (tokens.size() != 4) {
        printAdvisorbotOutput("Invalid syntax for 'max' command, check 'help predict'");
    }

    //parse and verify prediction type - min or max
    string minOrMax = tokens[1];
    if (minOrMax != "min" && minOrMax != "max") {
        printAdvisorbotOutput("The 1st parameter should 'min' or 'max', check 'help predict'");
        return;
    }

    //parse and verify product
    string product = tokens[2];
    if (availableProducts.find(product) == availableProducts.end()) {
        printAdvisorbotOutput("Product '" + product + "' doesn't exist");
        return;
    }

    //parse and verify order type
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

/**
 * Process 'time' command
 */
void AdvisorbotMain::processCommand_time(const vector<string> &tokens) {
    //verify amount of tokens
    if (tokens.size() != 1) {
        printAdvisorbotOutput("Invalid syntax for 'time' command, check 'help time'");
    }

    printAdvisorbotOutput(currentTime);
}

/**
 * Process 'step' command
 */
void AdvisorbotMain::processCommand_step(const vector<string> &tokens) {
    //verify amount of tokens
    if (tokens.size() != 1) {
        printAdvisorbotOutput("Invalid syntax for 'step' command, check 'help step'");
    }

    currentTime = orderBook.getNextTime(currentTime);

    printAdvisorbotOutput("now at " + currentTime);
}

/**
 * Process 'percentile' command
 */
void AdvisorbotMain::processCommand_percentile(const vector<string> &tokens) {
    //verify amount of tokens
    if (tokens.size() != 5) {
        printAdvisorbotOutput("Invalid syntax for 'percentile' command, check 'help percentile'");
    }

    //parse and verify product
    string product = tokens[1];
    if (availableProducts.find(product) == availableProducts.end()) {
        printAdvisorbotOutput("Product '" + product + "' doesn't exist");
        return;
    }

    //parse and verify order type
    OrderBookType orderBookType = OrderBookEntry::stringToOrderBookType(tokens[2]);
    if (orderBookType == OrderBookType::unknown) {
        printAdvisorbotOutput("Order book type provided is unknown, should be 'ask' or 'bid'");
        return;
    }

    //parse and verify number of timesteps
    unsigned int timesteps;
    try {
        timesteps = stoi(tokens[3]);
    } catch (const exception &e) {
        printAdvisorbotOutput("timesteps should positive integer number");
        return;
    }

    //parse and verify percentile value
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

/**
 * Process 'exit' command
 */
void AdvisorbotMain::processCommand_exit(const vector<string> &tokens) {
    //verify amount of tokens
    if (tokens.size() != 1) {
        printAdvisorbotOutput("Invalid syntax for 'exit' command, check 'help exit'");
    }

    printAdvisorbotOutput("Finishing program execution");
    std::exit(0);
}

/**
 * Print output
 */
void AdvisorbotMain::printAdvisorbotOutput(const string &message) {
    //apply color style to advisorbot output cursor
    std::cout << "\033[1;31madvisorbot>\033[0m " << message << std::endl;
}
#pragma once

#include <vector>
#include <map>
#include <set>
#include "OrderBookEntry.h"
#include "OrderBook.h"

using std::string;
using std::vector;
using std::map;

class AdvisorbotMain {
public:
    AdvisorbotMain();

    /**
     * Init the app
     */
    void init();

private:
    /**
     * Type definition of a command processing method
     */
    typedef void (AdvisorbotMain::*command_processing_method)(const vector<string> &);

    /**
     * Type definition of the map <command-nam> -> <processing-method>
     */
    typedef map<string, command_processing_method> command_processing_methods_map;

    void printWelcomeMessage();

    string readUserInput();

    void processUserInput(const string &input);

    void processCommand_help(const vector<string> &tokens);

    void processCommand_prod(const vector<string> &tokens);

    void processCommand_min(const vector<string> &tokens);

    void processCommand_max(const vector<string> &tokens);

    void processCommand_avg(const vector<string> &tokens);

    void processCommand_predict(const vector<string> &tokens);

    void processCommand_time(const vector<string> &tokens);

    void processCommand_step(const vector<string> &tokens);

    void processCommand_percentile(const vector<string> &tokens);

    void processCommand_exit(const vector<string> &tokens);

    static void printAdvisorbotOutput(const string &message);

    string currentTime;
    OrderBook orderBook{"20200601.csv"};
    std::set<string> availableProducts;
    /**
     * Map <command-name> -> <processing-method>
     */
    command_processing_methods_map commandProcessingMethodsMap = {
            {"help",       &AdvisorbotMain::processCommand_help},
            {"prod",       &AdvisorbotMain::processCommand_prod},
            {"min",        &AdvisorbotMain::processCommand_min},
            {"max",        &AdvisorbotMain::processCommand_max},
            {"avg",        &AdvisorbotMain::processCommand_avg},
            {"predict",    &AdvisorbotMain::processCommand_predict},
            {"time",       &AdvisorbotMain::processCommand_time},
            {"step",       &AdvisorbotMain::processCommand_step},
            {"percentile", &AdvisorbotMain::processCommand_percentile},
            {"exit",       &AdvisorbotMain::processCommand_exit}
    };
    /**
     * Map <command-name> -> <help-output>
     */
    map<string, string> commandHelpMap = {
            {"help",       " help -> List all available commands\n"
                           " help <cmd> -> Output help for the specified command"},
            {"prod",       "prod -> List available products\n"
                           " Example: prod -> ETH/BTC,DOGE/BTC"},
            {"min",        " min <product> <bid/ask> -> Find minimum bid or ask for product in current time step\n"
                           " Example: min ETH/BTC ask -> The min ask for ETH/BTC is 1.0"},
            {"max",        " max <product> <bid/ask> -> Find maximum bid or ask for product in current time step\n"
                           " Example: max ETH/BTC ask -> The max ask for ETH/BTC is 1.0"},
            {"avg",        " avg <product> <ask/bid> <timesteps> -> Compute average ask or bid for the sent product over the sent number of time steps\n"
                           " Example: avg ETH/BTC ask 10 -> The average ETH/BTC ask price over the last 10 timesteps was 1.0"},
            {"predict",    " predict <max/min> <product> <ask/bid> -> Predict max or min ask or bid for the sent product for the next time\n"
                           " Example: predict max ETH/BTC bid -> The predicted ETH/BTC bid price is 1.23"},
            {"time",       " time -> State current time in dataset, i.e. which timeframe are we looking at\n"
                           " Example: time ->  2020/03/17 17:01:24"},
            {"step",       " step -> Move to next time step\n"
                           " Example: step -> now at 2020/03/17 17:01:30"},
            {"percentile", " percentile <product> <ask/bid> <timesteps> <integer n from 1 to 100> -> Compute nth percentile ask or bid for the sent product over the sent number of time steps\n"
                           " Example: percentile ETH/BTC ask 10 50-> The 50th percentile of ETH/BTC ask price over the last 10 timesteps was 2.0\""},
            {"exit",       " exit -> finish execution\n"
                           " Example: exit -> <program finished execution>"}
    };
};

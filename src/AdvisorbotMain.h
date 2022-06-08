#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include <map>

class AdvisorbotMain
{
    public:
        AdvisorbotMain();
        /** Call this to start the sim */
        void init();
    private:
        typedef void (AdvisorbotMain::*command_processing_method)(std::vector<std::string>);
        typedef std::map<std::string, command_processing_method> command_processing_methods_map;

        void printWelcomeMessage();
        std::string readUserInput();
        void processUserInput(std::string input);
        std::vector<std::string> tokeniseUserInput(std::string input);
        void processCommand_help(std::vector<std::string> tokens);
        void processCommand_prod(std::vector<std::string> tokens);
        void processCommand_min(std::vector<std::string> tokens);
        void processCommand_max(std::vector<std::string> tokens);
        void processCommand_avg(std::vector<std::string> tokens);
        void processCommand_predict(std::vector<std::string> tokens);
        void processCommand_time(std::vector<std::string> tokens);
        void processCommand_step(std::vector<std::string> tokens);
        void processCommand_quantile(std::vector<std::string> tokens);
        void processCommand_exit(std::vector<std::string> tokens);
        void printAdvisorbotOutput(std::string message);


        void printMarketStats();
        void gotoNextTimeframe();








        std::string currentTime;
        OrderBook orderBook{"20200601.csv"};
        command_processing_methods_map commandProcessingMethodsMap = {
                {"help", &AdvisorbotMain::processCommand_help},
                {"prod", &AdvisorbotMain::processCommand_prod},
                {"min", &AdvisorbotMain::processCommand_min},
                {"max", &AdvisorbotMain::processCommand_max},
                {"avg", &AdvisorbotMain::processCommand_avg},
                {"predict", &AdvisorbotMain::processCommand_predict},
                {"time", &AdvisorbotMain::processCommand_time},
                {"step", &AdvisorbotMain::processCommand_step},
                {"quantile", &AdvisorbotMain::processCommand_quantile},
                {"exit", &AdvisorbotMain::processCommand_exit},
        };
};

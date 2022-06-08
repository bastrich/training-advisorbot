#include "AdvisorbotMain.h"
#include <iostream>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"

AdvisorbotMain::AdvisorbotMain()
{

}

void AdvisorbotMain::init()
{
    currentTime = orderBook.getEarliestTime();

    std::string input;
    while(true)
    {
        printWelcomeMessage();
        input = readUserInput();
        processUserInput(input);
    }
}

/**
 * Prints welcome message
 */
void AdvisorbotMain::printWelcomeMessage()
{
    printAdvisorbotOutput("Please enter a command, or help for a list of commands");
}

/**
 * Reads and returns user input as string
 */
std::string AdvisorbotMain::readUserInput() {
    std::cout << "\033[1;34muser> \033[0m";
    std::string input;
    std::getline(std::cin, input);
    return input;
}

/**
 * Parses and processes user input
 */
void AdvisorbotMain::processUserInput(std::string input)
{
    std::vector<std::string> tokens = tokeniseUserInput(input);

    if (tokens.empty()) {
        printAdvisorbotOutput("You typed nothing");
        return;
    }

    std::string command = tokens[0];
    command_processing_method commandProcessingMethod = commandProcessingMethodsMap[command];
    if (commandProcessingMethod == nullptr) {
        printAdvisorbotOutput("Command '" + command + "' doesn't exist, use 'help' to list available commands");
        return;
    }

    ((*this).*(commandProcessingMethod))(tokens);
}

void AdvisorbotMain::processCommand_help(std::vector<std::string> tokens)
{
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
                              "\n * quantile - AAAA"
                              "\n * exit - finish execution"
                              );
    }
}

void AdvisorbotMain::processCommand_prod(std::vector<std::string> tokens)
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void AdvisorbotMain::processCommand_min(std::vector<std::string> tokens)
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
    //    if (com)
//    try{
//        userOption = std::stoi(line);
//    }catch(const std::exception& e)
//    {
//        //
//    }
}

void AdvisorbotMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, 
                                                                p, currentTime);
        std::cout << "Asks seen: " << entries.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::endl;

    }
    // std::cout << "OrderBook contains :  " << orders.size() << " entries" << std::endl;
    // unsigned int bids = 0;
    // unsigned int asks = 0;
    // for (OrderBookEntry& e : orders)
    // {
    //     if (e.orderType == OrderBookType::ask)
    //     {
    //         asks ++;
    //     }
    //     if (e.orderType == OrderBookType::bid)
    //     {
    //         bids ++;
    //     }  
    // }    
    // std::cout << "OrderBook asks:  " << asks << " bids:" << bids << std::endl;

}
        
void AdvisorbotMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame. " << std::endl;
    for (std::string p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales =  orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl; 
            if (sale.username == "simuser")
            {
                // update the wallet
//                wallet.processSale(sale);
            }
        }
        
    }

    currentTime = orderBook.getNextTime(currentTime);
}

void AdvisorbotMain::printAdvisorbotOutput(std::string message) {
    std::cout << "\033[1;31madvisorbot>\033[0m " << message << std::endl;
}

std::vector<std::string> AdvisorbotMain::tokeniseUserInput(std::string input) {
    char separator = ' ';
    std::vector<std::string> tokens;
    signed int start, end;
    std::string token;
    start = input.find_first_not_of(separator, 0);
    do{
        end = input.find_first_of(separator, start);
        if (start == input.length() || start == end) break;
        if (end >= 0) token = input.substr(start, end - start);
        else token = input.substr(start, input.length() - start);
        tokens.push_back(token);
        start = end + 1;
    } while(end > 0);

    return tokens;
}

void AdvisorbotMain::processCommand_max(std::vector<std::string> tokens) {

}



void AdvisorbotMain::processCommand_avg(std::vector<std::string> tokens) {

}

void AdvisorbotMain::processCommand_predict(std::vector<std::string> tokens) {

}


void AdvisorbotMain::processCommand_time(std::vector<std::string> tokens) {

}

void AdvisorbotMain::processCommand_step(std::vector<std::string> tokens) {

}

void AdvisorbotMain::processCommand_quantile(std::vector<std::string> tokens) {

}

void AdvisorbotMain::processCommand_exit(std::vector<std::string> tokens) {

}


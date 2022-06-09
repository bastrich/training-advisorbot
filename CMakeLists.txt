cmake_minimum_required(VERSION 3.22)
project(merklerex_end_topic_5)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(merklerex_end_topic_5
        src/CSVReader.cpp
        src/CSVReader.h
        src/main.cpp
        src/AdvisorbotMain.cpp
        src/AdvisorbotMain.h
        src/OrderBook.cpp
        src/OrderBook.h
        src/OrderBookEntry.cpp
        src/OrderBookEntry.h
        src/Calculations.cpp
        src/Calculations.h
        )

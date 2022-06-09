#pragma once

#include <string>
#include <vector>

#include "OrderBookEntry.h"
#include "CSVReader.h"

using std::string;
using std::vector;

class Calculations {
public:
    static double max(vector<double> &values);
    static double min(vector<double> &values);
    static double avg(vector<double> &values);
    static double predict(vector<double> &values);
    static double nthPercentile(vector<double> &values, unsigned int n);
};

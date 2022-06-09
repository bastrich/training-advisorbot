#include <algorithm>
#include <set>

#include "Calculations.h"

using std::string;
using std::set;
using std::vector;

double Calculations::max(vector<double> &values) {
    double max = values[0];
    for (double &value: values) {
        if (value > max) max = value;
    }
    return max;
}


double Calculations::min(vector<double> &values) {
    double min = values[0];
    for (double &value: values) {
        if (value < min) min = value;
    }
    return min;
}

double Calculations::avg(vector<double> &values) {
    int count = values.size();
    double sum = 0;
    for (double &value: values) {
        sum += value;
    }
    return sum / count;
}

/**
 * Weighted Moving Average
 */
double Calculations::predict(vector<double> &values) {
    double numerator = 0;
    int denominator = 0;

    for (int i = 0; i < values.size(); i++) {
        numerator += (i + 1) * values[i];
        denominator += (i + 1);
    }

    return numerator / denominator;
}

/** Calculates median price of the orders provided*/
double Calculations::nthPercentile(vector<double> &values, unsigned int n) {
    unsigned long percentileIndex = n / 100 * values.size();
    nth_element(values.begin(), values.begin() + percentileIndex, values.end());

    return values[percentileIndex];
}
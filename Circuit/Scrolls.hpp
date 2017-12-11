#ifndef SCROLLS_H
#define SCROLLS_H
#include <vector>

// Header file for Scrolls.cpp. Declares the functions later linked from Graphing.cpp.

void plot3DCurve(std::vector<double> x, std::vector<double> y, std::vector<double> z);
void plotDifference(std::vector<double> x1, std::vector<double> x2, std::vector<double> t);

#endif
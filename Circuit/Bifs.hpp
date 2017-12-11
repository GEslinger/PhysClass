#ifndef BIFS_H
#define BIFS_H
#include <vector>
#include <string>

// Header file for Bifs.cpp. Declares the functions later linked from Graphing.cpp.

void plotBifur(std::vector<double> rvals, std::vector<std::vector<double>> solutions, std::string title, std::string x, std::string y, std::string fname);
std::vector<double> getExtrema(std::vector<double> v);

#endif
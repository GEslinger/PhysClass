#include <vector>
#include <string>
using namespace std;

// Initializes functions used from Graph.cpp (content linked with the Makefile)

void plot3DCurve(vector<double> x, vector<double> y, vector<double> z, double s, double r, double b);
void plotOneVar(vector<double> v, vector<double> t, string var, double s, double r, double b);
void plotBifur(vector<double> r, vector<vector<double>> solutions);
double inputDouble(double def, std::string title);
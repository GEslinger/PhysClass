#ifndef SIMULATE_H
#define SIMULATE_H
#include <vector>
#include <string>
#include "Planet.hpp"

// Header file for Simulate.cpp

void graphSystem(std::vector<vec3D> paths[], int maxP, int asts, int base, double scale, std::string fname);
void graphLogLog(std::vector<double> lSMA, std::vector<double> lPeriod);
double getEcc(std::vector<vec3D> base, std::vector<vec3D> planet);
std::vector<double> toLogLog(std::vector<double> x);
double getPeriod(std::vector<vec3D> base, std::vector<vec3D> planet, std::vector<double> time);
double inputDouble(double def, std::string title);
double getSMA(std::vector<vec3D> base, std::vector<vec3D> planet);
#endif
#ifndef SIMULATE_H
#define SIMULATE_H
#include <vector>
#include <string>
#include "Planet.hpp"

void graphSystem(std::vector<vec3D> paths[], int maxP, int base, double scale, std::string fname);
double getEcc(std::vector<vec3D> base, std::vector<vec3D> planet);
double getPeriod(std::vector<vec3D> base, std::vector<vec3D> planet, std::vector<double> time);
#endif
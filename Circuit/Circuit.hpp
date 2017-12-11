#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <functional>
#define lambda [&](double t, double v1, double v2, double v3)
#define step std::function<double(double,double,double,double)>

// Header file containing the definition of Circuit class and some useful preprocessor directives

class Circuit{
public:
	Circuit(double x0, double y0, double z0, step& xStep, step& yStep, step& zStep);
	void tStep(double h);
	double getX() const;	// Represents voltage across C1, named to match the shortcut in the derivation
	double getY() const;	// Voltage across C2
	double getZ() const;	// Current through L
	double getT() const;	// Time
	void reset(double x0, double y0, double z0);
private:
	step dx;	// Differential equations from derivation, describing how the values change
	step dy;
	step dz;

	double x;	// Instantaneous values
	double y;
	double z;
	double t;
};

#endif
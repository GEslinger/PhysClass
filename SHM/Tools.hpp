#ifndef INCLUDED_TOOLS		// Protection against double-inclusion, make sure this file hasn't been included by
#define INCLUDED_TOOLS 1	// checking against a flag, then set that flag
#include <functional>
#include <iostream>
#include <vector>
#include <string>
#define inspect(x) std::cerr << #x << " is " << x << " in file " << __FILE__ << " at line " << __LINE__ << std::endl


struct Path1D{				// A custom data type that makes passing around the position and time values easier
	std::vector<double> x;	// Position
	std::vector<double> t;	// Time
};


class Euler1D{	// Euler1D definition
public:
	Euler1D(double x_0, double v_0, std::function<double(double,double)>& func);// Constructor
	virtual void update(double dt);	// Euler step and updating x (virtual signifies the method can be overridden by EulerCromer1D)
	double getX() const;			// Accessor methods
	double getV() const;
protected:		// Protected instead of private allows the child class to make modifications (override mutator functions)
	double x;	// Instance variables
	double v;
	std::function<double(double,double)> f; // Euler step
};

Euler1D::Euler1D(double x_0, double v_0, std::function<double(double,double)>& func){
	x = x_0;	// Constructor initializes position, velocity, and euler step
	v = v_0;
	f = func;
}

void Euler1D::update(double dt){// Update using euler method, dt argument allows for dynamic timestep
	double eulerStep = f(x,v);	// Calculate the step before updating anything
	x += v*dt;					// Update x
	v += eulerStep*dt;			// Update v
}

double Euler1D::getX() const { return x; }	// Accessor methods for x and v
double Euler1D::getV() const { return v; }


class EulerCromer1D : public Euler1D{	// EulerCromer1D inherits from Euler1D with regular permissions since they are almost identical
public:
	EulerCromer1D(double x_0, double v_0, std::function<double(double,double)> func);
	void update(double dt) override;	// Override the update function from Euler1D to conserve energy
};

EulerCromer1D::EulerCromer1D(double x_0, double v_0, std::function<double(double,double)> func)	// Constructor for EulerCromer1D
: Euler1D(x_0, v_0, func){}																		// All it needs to do is call the super constructor

void EulerCromer1D::update(double dt){
	double eulerStep = f(x,v);	// Calculate the step before updating anything
	v += eulerStep*dt;			// Update v
	x += v*dt;					// Update x using v_(n+1) to create a backwards derivative and ensure energy conservation
}


double getMaxVal(std::vector<double> a){// My function to get the largest value in a vector
	double maxVal = a[0];				// Initialize the maximum with a best guess (the first element of a)
	for(int i = 0; i < a.size(); i++){	// Iterate over a
		if(a[i] > maxVal) maxVal = a[i];// If a[i] is greater than the maximum, consider it the new maximum
	}
	return maxVal;	// Return the max
}


double inputDouble(double def, std::string title){
	std::string input;
	double output;
	bool valid = false;

	std::cout << "Enter a valid double for " << title;
	std::cout << " (default is " << def << "): ";

	while(!valid){
		try{
			std::getline(std::cin, input);
			if(input.empty()) return def;

			output = std::stod(input);
			if(output != output) throw "Not a Number!";
			valid = true;
		} catch(...) {
			std::cout << "Invalid input, re-enter: ";
		}
	}

	// std::cout << std::endl << output << std::endl;
	return output;
}


#endif
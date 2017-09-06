#ifndef TOOLS_H		// Protection against double-inclusion, make sure this file hasn't been included by
#define TOOLS_H		// checking against a flag, then set that flag
#include <functional>
#include <iostream>
#include <vector>
#include <string>
#define inspect(x) std::cerr << #x << " is " << x << " in file " << __FILE__ << " at line " << __LINE__ << std::endl
		// ^^ inspect is a clever usage of the preprocessor to output the variable name, value, file, and location for MAXIMUM DEBUGGING POWERRRRRRRRRRRR


/*****************************************************************************

This header file contains definitions useful for the project that would
make the SHMMethods.cpp file bloated if created there. Some of these methods
are from earlier Tools.hpp files.

First is the definition of the Path1D struct which is fairly self-explanatory.

Next, the Euler1D class is defined with only a slight modification from the
one used in the drag project. Euler1D now requires the euler step on 
construction and accepts a timestep as a parameter when updating. These
generalizations allow for a dynamic timestep without constantly modifying
member variables. The member variables are also now protected to allow the
child class to make modifications and avoid copious amounts of mutator
methods.

The EulerCromer1D class inherits from Euler1D, requiring only a constructor
that calls back to the superconstructor and a overridden update method. The
update method was switched so now the velocity updates before the position and
the position uses the updated velocity to calculate its next value. The system
is then using a partial backwards derivative, conserving energy.

After the two classes comes the getMaxVal function from Drag/Tools.hpp
and an entirely new (and likely recurring) function to prompt and scrub input
for a double. It catches all forms of bad input including NaN, which yes, is
valid when using string.stod. That's it!

*****************************************************************************/


struct Path1D{				// A custom data type that makes passing around the position and time values easier
	std::vector<double> x;	// Position
	std::vector<double> t;	// Time
};


class Euler1D{	// Euler1D definition
public:
	Euler1D(double x_0, double v_0, std::function<double(double,double)>& func);// Constructor
	virtual void update(double dt);	// Euler step and updating x (virtual signifies the function can be overridden by EulerCromer1D)
	double getX() const;			// Accessor methods
	double getV() const;
protected:		// Protected instead of private allows the child class to make modifications (override mutator functions)
	double x;	// Member varibles
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


double inputDouble(double def, std::string title){		// Function to prompt and input a double, accepts parameters for name and default value
	std::string input;	// Input string
	double output;		// Output double
	bool valid = false;	// Only true if output is initialized properly

	std::cout << "Enter a valid double for " << title;	// Prompt
	std::cout << " (default is " << def << "): ";		// State default

	while(!valid){	// Go until a valid input (or broken by return statement)
		try{		// Try statement, mostly because of std::stod
			std::getline(std::cin, input);	// Feed a line into input
			if(input.empty()) return def;	// If the line is empty (just pressed enter), return the default value

			output = std::stod(input);		// Otherwise, try converting the string to a double- WHICH MAY THROW EXCEPTIONS
			if(output != output) throw "Not a Number!";	// Also, to find if the sillay user entered 'NaN', check for the equality property violation.
			valid = true;					// If everything succeeded, set valid to true.
		} 
		catch(...) {									// Catch all exceptions
			std::cout << "Invalid input, re-enter: ";	// Kindly tell user to not be dumb
		}
	}

	return output;	// Output the double!
}


#endif	// End of the include protection, anything after this would be included twice if the user tried to do so.
#ifndef INCLUDED_TOOLS
#include <functional>
#include <iostream>
#define INCLUDED_TOOLS 1
#define inspect(x) std::cout << #x << " is " << x << " in file " << __FILE__ << " at line " << __LINE__ << std::endl


class Euler1D{						// Euler1D definition
public:
	Euler1D(double x_0, double v_0, std::function<double(double,double)>& func);// Constructor
	virtual void update(double dt);// Euler step and updating x (virtual signifies the method can be overridden by EulerCromer1D)
	double getX() const;	// Accessor methods
	double getV() const;
protected:
	double x;	// Instance variables
	double v;
	std::function<double(double,double)> f; // Euler step
};

Euler1D::Euler1D(double x_0, double v_0, std::function<double(double,double)>& func){
	x = x_0;	// Constructor initializes position, velocity, and euler step
	v = v_0;
	f = func;
}

void Euler1D::update(double dt){	// Update using euler method, dt argument allows for dynamic timestep
	double eulerStep = f(x,v);	// Calculate the step before updating anything
	x += v*dt;					// Update x
	v += eulerStep*dt;			// Update v
}

double Euler1D::getX() const { return x; }	// Accessor methods for x and v
double Euler1D::getV() const { return v; }


class EulerCromer1D : public Euler1D{	// EulerCromer1D inherits from Euler1D since they are almost identical
public:
	void update(double dt) override;	// Override the update function from Euler1D to conserve energy
	using Euler1D::update;
};

void EulerCromer1D::update(double dt){
	double eulerStep = f(x,v);	// Calculate the step before updating anything
	v += eulerStep*dt;			// Update v
	x += v*dt;					// Update x using v_(n+1) to create a backwards derivative and ensure energy conservation
}


#endif
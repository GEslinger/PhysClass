#ifndef METHOD_H	// Inclusion protection
#define METHOD_H
#include <functional>
#define dfunc std::function<double(double,double)>	// Abbreviation

// Prototype and definition of numerical integration classes!
// Runge-Kutta (RK Class) and Euler-Cromer (EC Class) both inherit Method to save space and enable polymorphism. 

class Method{
public:
	Method(double x0, double dt, dfunc f){	// Constructor, default values and function
		t = 0;
		x = x0;
		h = dt;
		stepFunc = f;
	}
	double getX(){return x;}	// Eazy peazy lemon squeezy accessor methods
	double getT(){return t;}
	void changeFunc(dfunc f){stepFunc = f;}	// Change the function (Called a ton of times)
	void reset(double x0, double dt){	// Reset the values
		t = 0;
		x = x0;
		h = dt;
	}
	virtual void step() = 0;	// Pure virtual function, meaning subclasses must implement it and that the containing class can't be instantiated
								// Because you need to define a step function for numerical integration!
protected:
	dfunc stepFunc;	// Keep data members protected so subclasses can access in their step functions
	double x;
	double t;
	double h;
};



class RK : public Method{	// RK inherits from Method and uses its constructor
public:
	RK(double x0, double dt, dfunc f) : Method(x0,dt,f){}
	void step();
};

class EC : public Method{	// EC inherits from Method and uses its constructor
public:
	EC(double x0, double dt, dfunc f) : Method(x0,dt,f){}
	void step();
};

#endif
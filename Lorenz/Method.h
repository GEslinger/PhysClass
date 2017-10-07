#ifndef METHOD_H
#define METHOD_H
#include <functional>
#define dfunc std::function<double(double,double)>

class Method{
public:
	Method(double x0, double dt, dfunc f){
		t = 0;
		x = x0;
		h = dt;
		stepFunc = f;
	}
	double getX(){return x;}
	double getT(){return t;}
	virtual void step() = 0;

protected:
	dfunc stepFunc;
	double x;
	double t;
	double h;
};



class RK : public Method{
public:
	RK(double x0, double dt, dfunc f) : Method(x0,dt,f){}
	void step();
};

class EC : public Method{
public:
	EC(double x0, double dt, dfunc f) : Method(x0,dt,f){}
	void step();
};

#endif
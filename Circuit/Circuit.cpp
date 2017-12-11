#include "Circuit.hpp"

double RK4Step(double& t, double& v1, double& v2, double& v3, double h, step& stepFunc){	// General timestep function supporting additional arguments
	double t1 = t;
	double t2 = t1 + h/2;
	double t3 = t1 + h/2;
	double t4 = t1 + h;
	double x1 = v1;
	double x2 = x1 + stepFunc(t1,x1,v2,v3) * h/2;
	double x3 = x1 + stepFunc(t2,x2,v2,v3) * h/2;
	double x4 = x1 + stepFunc(t3,x3,v2,v3) * h;
	double k1 = stepFunc(t1,x1,v2,v3);
	double k2 = stepFunc(t2,x2,v2,v3);
	double k3 = stepFunc(t3,x3,v2,v3);
	double k4 = stepFunc(t4,x4,v2,v3);
	return v1 + 1./6. * (k1 + 2*k2 + 2*k3 + k4) * h;	// Returns new value
}

Circuit::Circuit(double x0, double y0, double z0, step& xStep, step& yStep, step& zStep){
	x = x0;		// Constructor is easy
	y = y0;
	z = z0;
	dx = xStep;
	dy = yStep;
	dz = zStep;

	t = 0;
}

void Circuit::reset(double x0, double y0, double z0){
	x = x0;		// Just like new!
	y = y0;
	z = z0;
	t = 0;
}

void Circuit::tStep(double h){
	double nextX = RK4Step(t, x, y, z, h, dx);	// Calculate the next
	double nextY = RK4Step(t, y, x, z, h, dy);
	double nextZ = RK4Step(t, z, x, y, h, dz);

	x = nextX;
	y = nextY;
	z = nextZ;

	t += h;
}


double Circuit::getX() const{
	return x;
}
double Circuit::getY() const{
	return y;
}
double Circuit::getZ() const{
	return z;
}
double Circuit::getT() const{
	return t;
}
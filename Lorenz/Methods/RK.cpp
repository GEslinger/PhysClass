#include "Method.hpp"

void RK::step(){
	double t1 = t;
	double t2 = t1 + h/2;
	double t3 = t1 + h/2;
	double t4 = t1 + h;
	double x1 = x;
	double x2 = x1 + stepFunc(t1,x1) * h/2;
	double x3 = x1 + stepFunc(t2,x2) * h/2;
	double x4 = x1 + stepFunc(t3,x3) * h;
	double k1 = stepFunc(t1,x1);
	double k2 = stepFunc(t2,x2);
	double k3 = stepFunc(t3,x3);
	double k4 = stepFunc(t4,x4);
	x = x + 1./6. * (k1 + 2*k2 + 2*k3 + k4) * h;
	t = t + h;
}
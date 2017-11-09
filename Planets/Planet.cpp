#include <vector>
#include <functional>
#include "Planet.hpp"
#define FUNC [&](double x, double t)
#define secInDay 86400.				// Values used in conversion
#define metersInAU 149597870700.
using namespace std;

/*****************************************************************************

This program implements the Planet class and contains the Runge-Kutta step
function.

*****************************************************************************/

double RKStep(double x, double t, double h, function<double(double,double)> func){	// Runge-Kutta step function
	double t1 = t;
	double t2 = t1 + h/2;
	double t3 = t1 + h/2;
	double t4 = t1 + h;
	double x1 = x;
	double x2 = x1 + (func)(x1,t1) * h/2;
	double x3 = x1 + (func)(x2,t2) * h/2;
	double x4 = x1 + (func)(x3,t3) * h;
	double k1 = (func)(x1,t1);
	double k2 = (func)(x2,t2);
	double k3 = (func)(x3,t3);
	double k4 = (func)(x4,t4);
	return 1./6. * (k1 + 2*k2 + 2*k3 + k4) * h;	// Return the step
}

Planet::Planet(vec3D pos0, vec3D vel0, double m){ // Constructor
	pos = pos0*metersInAU;			// Input units are au and day (from JPL Horizons), must be corrected
	vel = vel0*metersInAU/secInDay;
	nextPos = vec3D{0,0,0};
	nextVel = vec3D{0,0,0};
	mass = m;
}

vec3D Planet::getPos() const {	// Accessor methods
	return pos;
}
vec3D Planet::getVel() const {
	return vel;
}
double Planet::getMass() const {
	return mass;
}
double Planet::getT() const {
	return t;
}

void Planet::update(){	// Update the position and velocity
	pos += nextPos;
	vel += nextVel;
}

void Planet::tStep(vec3D acc, double dt){					// Timestep
	nextPos.x = RKStep(pos.x, t, dt, FUNC{return vel.x;});
	nextPos.y = RKStep(pos.y, t, dt, FUNC{return vel.y;});
	nextPos.z = RKStep(pos.z, t, dt, FUNC{return vel.z;});

	nextVel.x = RKStep(vel.x, t, dt, FUNC{return acc.x;});
	nextVel.y = RKStep(vel.y, t, dt, FUNC{return acc.y;});
	nextVel.z = RKStep(vel.z, t, dt, FUNC{return acc.z;});

	t += dt;	// Add time
}
#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <cmath>
#include "Tools.hpp"
#define PI 3.14159265
using namespace std;

/***********************************************************************

Second Case: 2D drag! DragObject2D is a class for a point particle in
2D coordinates. Instead of individual accessor methods like Euler1D,
getAttrs returns a vector of the four instance variables. The Euler
step and updating code is all built in as well. A frequently used data
type in this program is vector<vector<double>>, usually called path.
The data stored is a vector of x values and y values in a projectile's 
trajectory, ex. {{x1,x2,x3...},{y1,y2,y3...}}. Passing a data type like
this around is much easier than writing two methods or using evil
libraries to make functions return multiple values.

The getPath function takes an initial x and y velocity and performs the
Euler method on an instance of DragObject2D, saving the x and y values
in vectors. It simulates until a sign change is detected in y, meaning
the object has returned to the ground. For that reason,
making v_i negative is a bad idea. After simulation, the function 
returns a path. plotPath just plots the path given with axes of path[0],
path[1].

RangeVAngle is more complicated, as it performs all calculation and 
plots the range vs. angle graph. It iterates from -pi/2 to pi/2 with a 
step size of dth. For each iteration, it gets the path with getPath. The
final x value is the range when the object returns to the ground, by
the definition of the getPath function.

***********************************************************************/

const double v_i = 10;		// Initial velocity for both the trajectory and the range v. angle plot
const double th = PI/6;		// Firing angle for the trajectory plot
const double y_0 = 1;		// Global starting height, Should be either zero or something positive, as the range vs. angle graph must be complete.

const double g = 9.8;		// Gravitational acceleration (negative)
const double b = 0;//.56832;	// Calculated drag constant for Syd Miyasaki
const double dt = 0.001;	// Timestep
const double dth = 0.001;	// Step in angle for the range v. angle plot

class DragObject2D{	// Problem-specific class
public:
	DragObject2D(double vx_0, double vy_0);	// Constructor
	vector<double> getAttrs() const;		// Accessor method for instance variables
	double getSpeed() const;				// Accessor / calculation method for speed
	void update();	// Euler step and updating
private:
	double x, y, vx, vy;
};

DragObject2D::DragObject2D(double vx_0, double vy_0){	// Constructor sets x to zero, y to starting height, initial velocities as arguments
	x = 0;
	y = y_0;
	vx = vx_0;
	vy = vy_0;
}

vector<double> DragObject2D::getAttrs() const {	// Usage of anonymously declared vectors to return all instance variables
	return vector<double>{x,y,vx,vy};
}

double DragObject2D::getSpeed() const {	// *trivial* mechanics (component velocity to speed)
	return sqrt(vx*vx + vy*vy);
}

void DragObject2D::update(){	// Update all the things!
	double v = this->getSpeed();// Get speed on the current object instance. "this" returns a pointer to the instance, and -> is an abbreviation of (*this).getSpeed()
	x += vx*dt;				// Update x and y
	y += vy*dt;

	vx += -(b*v*vx)*dt;		// Euler step for x and y
	vy += -(g+b*v*vy)*dt;
}


vector<vector<double>> getPath(double vx_0, double vy_0){	// Get the path thing
	vector<double> x_n;	// Vectors for the path
	vector<double> y_n;
	double yval = 0;	// The value of y

	DragObject2D object(vx_0, vy_0);// The object to track

	while(yval >= 0){				// Only track until the object hits the ground
		vector<double> attrs = object.getAttrs();	// Get attributes (x, y, vx, vy)
		x_n.push_back(attrs[0]);	// Add attributes to vectors, update y value
		y_n.push_back(attrs[1]);
		yval = attrs[1];

		object.update();			// Perform Euler step and updates
	}

	return vector<vector<double>>{x_n,y_n};	// Return the two vectors of x and y together
}


void plotPath(vector<vector<double>>& path){	// Plot the path; pretty basic stuff
	Gnuplot gp;

	gp << setprecision(3);
	gp << "set xrange [0:" << path[0].back() << "]\n";
	gp << "set yrange [0:" << getMaxVal(path[1])*1.1 << "]\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set xlabel \"x (m)\"\n";
	gp << "set ylabel \"y (m)\"\n";
	gp << "set title \"Trajectory of Syd Under Rayleigh's Drag Equation\\nb = " << b;
	gp << ", v_i = " << v_i << ", {/Symbol q}_i = " << th << "\"\n";
	gp << "set output \"Trajectory.png\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(path[0],path[1]));
}


void rangeVAngle(){			// Calculation and plotting of the range vs. angle plot
	vector<double> ranges;	// Declare vectors
	vector<double> angles;

	for(double ang = -PI/2; ang < PI/2; ang += dth){	// From -pi/2 to pi/2, steps of dth

		double vx_0 = v_i*cos(ang);	// From v_i and ang, determine the initial velocities
		double vy_0 = v_i*sin(ang);	
		vector<vector<double>> path = getPath(vx_0, vy_0);	// Generate path
		double range = path[0].back();	// The last x value is the range

		angles.push_back(ang);	// Save angle and range
		ranges.push_back(range);
	}

	Gnuplot gp;		// Now plot!

	gp << setprecision(3);
	gp << "set xrange [" << -PI/2 << ":" << PI/2 << "]\n";
	gp << "set yrange [0:" << getMaxVal(ranges)*1.1 << "]\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set xlabel \"angle (rad)\"\n";
	gp << "set ylabel \"range (m)\"\n";
	gp << "set title \"Range vs. Firing Angle of Syd Under Rayleigh's Drag Equation, b = " << b << "\"\n"; 
	gp << "set output \"RangeAngle.png\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(angles,ranges));
}


int main(){		// Main function
	double vx_0 = v_i*cos(th);	// Use v_i and the launch angle th to determine initial velocities by components
	double vy_0 = v_i*sin(th);
	vector<vector<double>> path = getPath(vx_0, vy_0);	// Generate the path
	plotPath(path);	// Plot it

	rangeVAngle();	// And make the range vs. angle plot!

	return 0;	// If it all worked, return 0
}
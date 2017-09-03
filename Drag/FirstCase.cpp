#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <cmath>
#include "Tools.hpp"
using namespace std;


/***********************************************************************

This program is for the first case of 1-Dimensional drag. The Euler1D
class is generalized to keep track of position and velocity. It accepts
a lambda as a generic euler step function. The only variable it depends
on is dt, generally used to change the accuracy and computation time.

The main function creates a Euler1D object called point with an initial
velocity of v_i and and an initial position (x) of 0. The main loop
increments t by dt each iteration, saving the x, v, and t in vectors and
applying the euler step. The program then plots v vs t and x vs t in
files VelTime.png and PosTime.png respectively. Then, the program uses
a few functions from Tools.hpp to find the percent error of the terminal
velocity and the rising and falling times of the object.

***********************************************************************/

const double v_i = 5;			// Initial velocity of the object
const double g = 9.8;			// Gravitational acceleration (negative)
const double b = 1;				// Generalized drag constant
const double dt = 0.001;//0.255;		// Time step
const double SIM_TIME = 2;		// Time to run the simulation (Should be large enough to let v = v_t)

function<double(double,double)> dragStep = [](double x, double v){ return -g-b*v*abs(v); };	// This is the lambda function used as the euler step. The type isn't auto because\
	// it's passed as an argument to the Euler1D::update fuction and the compiler has to know parameter type.

class Euler1D{						// Euler1D definition
public:
	Euler1D(double x_0, double v_0);// Constructor
	void update(function<double(double,double)>& f);// Euler step and updating x
	double getX() const;	// Accessor methods
	double getV() const;
private:
	double x;	// Instance variables
	double v;
};

Euler1D::Euler1D(double x_0, double v_0){	// Constructor just initializes position and velocity
	x = x_0;
	v = v_0;
}

void Euler1D::update(function<double(double,double)>& f){	// Reference to Euler step lambda as argument for update function
	double eulerStep = f(x,v);	// Calculate the step before updating anything
	x += v*dt;					// Update x
	v += eulerStep*dt;			// Update v
}

double Euler1D::getX() const { return x; }	// Accessor methods for x and v
double Euler1D::getV() const { return v; }


void plotStuff(vector<double> x, vector<double> v, vector<double> t){	// Plots the velocity v. time and position v. time graphs
	Gnuplot gp;	// Instance of gnuplot terminal stream

	gp << setprecision(3);	// Velocity v. time graph...
	gp << "set xrange [0:" << SIM_TIME << "]\n";
	gp << "set yrange [" << v[0] << ":" << getMinVal(v)*1.1 << "]\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set xlabel \"t (s)\"\n";
	gp << "set ylabel \"v (m/s)\"\n";
	gp << "set title \"Velocity vs. Time with Rayleigh's Drag Equation, b = " << b << "\"\n"; 
	gp << "set output \"VelTime.png\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(t,v));


	gp << "set xrange [0:" << SIM_TIME << "]\n";	// Position v. time graph...
	gp << "set yrange [0:" << getMaxVal(x)*1.1 << "]\n";
	gp << "set xlabel \"t (s)\"\n";
	gp << "set ylabel \"x (m)\"\n";
	gp << "set title \"Position vs. Time with Rayleigh's Drag Equation, b = " << b << "\"\n"; 
	gp << "set output \"PosTime.png\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(t,x));
}


int main(){					// Main program!
	Euler1D point(0,v_i);	// Create an Euler1D object, x=0 v=v_i
	vector<double> x_n;		// Vectors for x, v, and t
	vector <double> v_n;
	vector <double> t_n;

	for(double t = 0; t < SIM_TIME; t += dt){	// Main loop
		x_n.push_back(point.getX());	// Save point's x and v before modification
		v_n.push_back(point.getV());
		t_n.push_back(t);				// Save the value of t for plotting
		point.update(dragStep);			// Update the point using kinematic equations and Euler step
	}

	plotStuff(x_n, v_n, t_n);	// Make the plots!

	double diff = pErr(g,b,v_n.back());	// Calculate the percent error between accepted and estimated terminal velocities
	cout << "Difference between calculated and final value: " << diff*100 << "%\n";	// Display the percent error as a percentage

	double rising = sgnChangeTime(v_n, t_n);	// Find the time when velocity changes sign, marking the peak of the trajectory and rising time
	double hitground = sgnChangeTime(x_n, t_n);	// Find the time when position changes time, marking when the object returns to 0
	cout << "Rising time: " << rising << "\n";	// Output rising time
	cout << "Falling time: " << hitground-rising << "\n";	// Then falling time (they should be different if b!= 0)

	return 0;	// Everything went well, return 0!
}
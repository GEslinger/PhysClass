#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <functional>
#include <iomanip>
#include <string>
#include <cmath>
#include "Tools.hpp"
using namespace std;

const double x_0 = inputDouble(0, "Initial Position");		// Initial position
const double v_0 = inputDouble(10, "Initial Velocity");		// Initial velocity
const double dt = inputDouble(0.005, "Time Step");			// Time step
const double k = inputDouble(25, "Spring Constant");		// Spring Constant
const double m = inputDouble(1, "Mass");					// Mass
const double om = sqrt(k/m);								// Omega
const double SIM_TIME = inputDouble(50, "Simulation Time");	// Time to simulate


function<double(double,double)> springStep = [](double x, double v){ return -om*om*x; };	// The lambda function used as the differential equation


Path1D applyMethod(Euler1D* obj){	// Function to apply euler method to a pointer of Euler1D, allowing for polymorphism
	Path1D out;						// Define an empty path to put the results in and output

	for(double t = 0; t < SIM_TIME; t += dt){	// Iterate from 0 to SIM_TIME with the defined time step
		out.x.push_back(obj->getX());	// Add the current position to the path
		out.t.push_back(t);				// Add the current time to the path
		obj->update(dt);				// Apply the update method of obj (either Euler's or Euler-Cromer)
	}

	return out;	// Return the path variable
}


void plotPath(Path1D p, double amp, string title, string fname){	// Used to plot a path, specialized for this program...
	// Accepts input for the path, maximum amplitude (so graphs have the same scale and aren't terrible), the title of the method, and the file name to output
	Gnuplot gp;	// Instance of gnuplot terminal stream

	gp << setprecision(3);
	gp << "set xrange [0:" << SIM_TIME << "]\n";
	gp << "set yrange [" << -amp*1.1 << ":" << amp*1.1 << "]\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set xlabel \"t (s)\"\n";
	gp << "set ylabel \"x (m)\"\n";
	gp << "set title \"Position Vs. Time in Simple Harmonic Motion\\nUsing " << title << "\n"; 
	gp << "set output \"" << fname << "\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(p.t,p.x));	// Separate the path into components and plot it!
}


int main(){
	Euler1D* badSpring = new Euler1D(x_0, v_0, springStep);			// Make a pointer to a Euler1D object
	Euler1D* goodSpring = new EulerCromer1D(x_0, v_0, springStep);	// And one to a EulerCromer1D object

	Path1D badPath = applyMethod(badSpring);	// Apply the method for both objects
	Path1D goodPath = applyMethod(goodSpring);

	double bigAmp = getMaxVal(badPath.x);	// Amplitude of the oscillation (max value of pos)
	plotPath(badPath, bigAmp, "Euler's Method", "BadMethod.png");			// Plot them accordingly!
	plotPath(goodPath, bigAmp, "The Euler-Cromer Method", "GoodMethod.png");

	delete badSpring;	// Garbage collection to free up allocated memory
	delete goodSpring;
	return 0;			// All done!
}
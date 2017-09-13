#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <functional>
#include <iomanip>
#include <string>
#include <cmath>
#include "Tools.hpp"
using namespace std;


/*****************************************************************************

This program uses the classes and a few functions from Tools.hpp to model a
block-spring system in simple harmonic motion (SHM). The program implements
two methods: Euler's method and the Euler-Cromer method. The variables of the
differential equation are stored in a classes named accordingly, along with
accessor methods and a mutator to apply the defined euler step. Since 
EulerCromer1D is a subclass of Euler1D, they can be used polymorphically by
allocating memory for them and referencing them by a pointer to Euler1D.

In the applyMethod function, the line with obj->update(dt) applies the update
function of whichever class is supplied, not the one for Euler1D. It returns a 
struct of type Path1D (from Tools.hpp) to avoid nasty data types like 2D 
vectors or having to return multiple values.

The plotpath function simply takes the Path1D, along with some information
about what's being plotted and how big it is, and sends it to gnuplot.

Finally, the main method creates the objects, applies the method, and plots
them both. The getMaxValue function returns the amplitude of the Euler Method
path (which should be larger) and plots them both methods at the same scale.
Then, the created objects are deleted and the program ends.

*****************************************************************************/


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
		out.v.push_back(obj->getV());	// Add the current velocity to the path (for phase space)
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
	gp << "set title \"Position Vs. Time in Simple Harmonic Motion\\nUsing " << title << "\"\n"; 
	gp << "set output \"" << fname << "\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(p.t,p.x));	// Separate the path into components and plot it!
}


void plotPhaseSpace(Path1D p, string title, string fname){	
	Gnuplot gp;	// Instance of gnuplot terminal stream
	vector<double> v = getMomentum(p.v, m);

	gp << setprecision(3);
	gp << "set xrange [" << -1.1*getMaxVal(p.x) << ":" << 1.1*getMaxVal(p.x) << "]\n";
	gp << "set yrange [" << -1.1*getMaxVal(v) << ":" << 1.1*getMaxVal(v) << "]\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set xlabel \"x (m)\"\n";
	gp << "set ylabel \"v (m/s)\"\n";
	gp << "set title \"Phase Space Diagram of SHM using " << title << "\"\n"; 
	gp << "set output \"" << fname << "\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(p.x,v));	// Separate the path into components and plot it!
}


int main(){
	Euler1D* badSpring = new Euler1D(x_0, v_0, springStep);			// Make a pointer to a Euler1D object
	Euler1D* goodSpring = new EulerCromer1D(x_0, v_0, springStep);	// And one to a EulerCromer1D object

	Path1D badPath = applyMethod(badSpring);	// Apply the method for both objects
	Path1D goodPath = applyMethod(goodSpring);

	double bigAmp = getMaxVal(badPath.x);	// Amplitude of the oscillation (max value of pos)
	plotPath(badPath, bigAmp, "Euler's Method", "BadMethod.png");			// Plot them accordingly!
	plotPath(goodPath, bigAmp, "The Euler-Cromer Method", "GoodMethod.png");

	plotPhaseSpace(badPath, "Euler's Method", "BadPSD.png");
	plotPhaseSpace(goodPath, "the Euler-Cromer Method", "GoodPSD.png");

	delete badSpring;	// Garbage collection to free up allocated memory
	delete goodSpring;
	return 0;			// All done!
}
#include <vector>
#include <iostream>
#include <cmath>
#include "Circuit.hpp"
#include "Scrolls.hpp"
using namespace std;



/**********************************************************************************


Well, here we are! Final Project! This program creates a double scroll graph and
shows the system's sensitivity to initial conditions. There aren't any special
tricks in this one, so reading the code and the comments gives a good idea of
how it works. The only tricksy thing here is the definition of the differential
equations. To avoid constantly redifining the equations, I opted to add arguments
to the equations for the value of other variables. In my implementation of RK4, 
only t (time) and v1 (first variable) are modified. The other two variables, v1 and
v2, act like constants. 

But, since I'm defining "lambda" as a preprocessor directive in Circuit.hpp, the
values of v1, v2, and v3 change for each differential equation. Here's how:
VC1:
v1 is x,
v2 is y,
v3 is z

VC2:
v1 is y,
v2 is x,
v3 is z,

IL:
v1 is z,
v2 is x,
v3 is y

Hurray!


**********************************************************************************/



const double MAX_TIME = 100;		// Maximum time to simulate
const double DT = 2e-5;				// Size of timestep
const double INTERVAL = 1e-3;		// Interval to record points for plotting double scroll

double R = 10;			// Resistance of R
double L = 6.7e-2;		// Inductance of L
double C1 = 2e-3;	// Capacitance of C1 
double C2 = 8e-2;		// Capacitance of C2

auto N = [](double V){return -V/sqrt(V*V+70);};		// Function describing I-V curve of N_R

step VC1 = lambda{return ((v2-v1)/R-N(v1))/C1;};	// Differential equations for change in Capacitances and Current
step VC2 = lambda{return (v3+(v2-v1)/R)/C2;};
step IL = lambda{return -v3/L;};

int main(){
	vector<double> x;	// Vector for voltage across C1
	vector<double> y;	// Voltage across C2
	vector<double> z;	// Current in L
	vector<double> t;	// Time (s)

	Circuit chua(0.,0.,0.5,VC1,VC2,IL);		// Create a chua's circuit with initial state

	Circuit chua2(0.,0.,0.500000000000001,VC1,VC2,IL);	// Another chua's circuit, except with initial state that is different by 1e-15
	vector<double> x2;	// Only record the voltage across C1 for the second circuit, since we only need one dimension to observe diveregence

	double recTim = 0;							// Next time to record data
	for(double T = 0; T < MAX_TIME; T += DT){	// Simulate untile MAX_TIME with timestep DT
		chua.tStep(DT);		// Give the circuits a timestep
		chua2.tStep(DT);

		if(T > recTim){					// If the current time is greater than when data should be recorded
			x.push_back(chua.getX());	// Record data
			y.push_back(chua.getY());
			z.push_back(chua.getZ());
			t.push_back(chua.getT());
			x2.push_back(chua2.getX());

			recTim += INTERVAL;			// And advance the next time to record by INTERVAL
		}
	}

	cout << "Plotting Curve Difference\n";
	plotDifference(x,x2,t);	// First plot the difference between the two circuits

	cout << "Graphing 3D Curve\n";
	plot3DCurve(x,y,z);	// Then plot the scroll shape!
	
	return 0;	// Return without error
}
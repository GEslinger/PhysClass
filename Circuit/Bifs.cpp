#include <vector>
#include <iostream>
#include <cmath>
#include "Circuit.hpp"
#include "Bifs.hpp"
using namespace std;



/**********************************************************************************


I decided to make this program an excercise in advanced C++ programming. I could've
easily created 3 sets of nested for loops for all 3 bifurcation diagrams, but I 
aimed to use a single, smart function to handle them all.

First, to explain my approach to bifucation diagrams. I record the changing
(independent) variable first. Then, I run through a full simulation of the circuit,
recording a specified value after each timestep. Then, I extract the extrema of the
values recorded. Finally, the array of extrema is put into a vector with index
matching that of the independent variable. The plotBifur function (in Graphing.cpp)
re-arranges the data into matching pairs of independent and dependent values to
create a scatterplot. This results in a lot of repeated data on the independent
side, since hundreds of dependent values are linked to single simulations.

Now, onto my programming approach. doBifur is an all-in-one bifurcation diagram
generator function, accepting several arguments. First is a pointer to the instance
of a circuit (in this program, the object named chua). Second is a reference to the
member function that returns the dependant variable. Wait, what? Yes, it's the
address of a function relative to the class prototype. That means we can specify
whether the dependent variable is x, y, or z, by passing references to different
functions in Circuit. The third argument is a reference to the dependent variable.
The next two arguments are the bounds of the independent variable, and the next is
the step size for the diagram. Then come the x and y axis titles, and finally the
filename. Whew! It's not the simplest approach, but main() sure does look sexy.


**********************************************************************************/



const double MAX_TIME = 100;		// Maximum time to simulate
const double DT = 5e-4;				// Size of timestep

double R = 10;			// Resistance of R
double L = 6.7e-2;		// Inductance of L
double C1 = 2.15e-3;	// Capacitance of C1 
double C2 = 8e-2;		// Capacitance of C2

auto N = [](double V){return -V/sqrt(V*V+70);};		// Function describing I-V curve of N_R

step VC1 = lambda{return ((v2-v1)/R-N(v1))/C1;};	// Differential equations for change in Capacitances and Current
step VC2 = lambda{return (v3+(v2-v1)/R)/C2;};
step IL = lambda{return -v3/L;};

void doBifur(Circuit* c, double(Circuit::*getOut)() const, double& val, double low, double high, double d, string x, string y, string fname){	// holy crap see above
	double begin = val;			// Store the starting value to reset it later
	vector<double> vals;		// Vector to hold the values of independent variable
	vector<vector<double>> out;	// Vector to hold arrays of values from dependent variable

	for(val = low; val < high; val += d){
		c->reset(0., 0., 0.5);		// Set circuit to defualt values
		vector<double> oneRow;		// Holds one row of values in "out" vector, before getting extremas

		for(double t = 0; t < MAX_TIME; t += DT){	// Simulate the circuit
			c->tStep(DT);
			oneRow.push_back((c->*getOut)());	// Record the specified value each timestep by calling the specified function
		}

		vals.push_back(val);				// Save the value of the independent variable
		out.push_back(getExtrema(oneRow));	// And the extrema of the dependent

		//cout << val << " " << flush;
	}

	string title = "Bifurcation Diagram of ";	// Generate an appropriate title
	title.append(y);
	title.append(" vs. ");
	title.append(x);
	plotBifur(vals, out, title, x, y, fname);	// Plot the bifurcation diagram

	val = begin;				// Reset to starting value for next diagram
}

int main(){

	Circuit* chua = new Circuit(0.,0.,0.5,VC1,VC2,IL);	// Chua's Circuit Object


	cout << "Generating Bifurcation for C1...\n";		// Generating and plotting Bifurcation Diagrams
	doBifur(chua, &Circuit::getX, C1, 1.4e-3, 3e-3, 2e-6, "C_1", "V_{C_1}", "bifc1.png");	// Notice how the second argument is a reference to the function
	cout << "\nDone\n";																		// that gets the value of C1 (x)


	cout << "Generating Bifurcation for C2...\n";
	doBifur(chua, &Circuit::getY, C2, 0.05, 0.3, 2e-4, "C_2", "V_{C_2}", "bifc2.png");
	cout << "\nDone\n";


	cout << "Generating Bifurcation for L...\n";
	doBifur(chua, &Circuit::getZ, L, 0.04, 0.109, 1e-4, "L", "I_L", "bifl.png");
	cout << "\nDone\n";

	delete chua;	// Clean up the traaaash!
	return 0;		// Return without error.
}
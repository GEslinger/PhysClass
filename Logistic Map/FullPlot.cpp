#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <vector>
using namespace std;

/**********************************************************

This is the program for the bifurcation diagram! :D
The flow is simple: For each r value between 1 and 4 (with
resolution of dr), 100 values of the logistic map are saved
in the solutions vector. The values are chosen far enough
away from the origin to remove the effects of the initial
behavior. The values are paired with the r value they were
calculated at and then sent to the plotter.

**********************************************************/

const int START = 900;
const int CUTOFF = 1000;	// Cutoff value for finding attractors
const double dr = 0.001;
const double x0 = 0.5;		// Initial value

void plotStuff(vector<double> x, vector<double> y){	// Pretty plotting shenanigans
	Gnuplot gp;

	gp << "set xrange [1:4]\n";
	gp << "set yrange [0:1]\n";
	gp << "set term wxt font \"FreeSerif,12\"\n";
	gp << "set title \"End Behavior of the Logistic Function vs. Growth Rate (x_0 = 0.5)\"\n";
	gp << "set xlabel \"r\"\n";
	gp << "set ylabel \"x_n\"\n";
	gp << "set output \"plot.png\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(x,y));
}

vector<double> getSolutions(double r){
	double x = x0;							// Set up x_0
	vector<double> out;						// All the solutions for this value of r

	for(int i = 0; i < CUTOFF; i++){		// Loop until the cutoff
		if(i >= START){out.push_back(x);}	// Push the x value onto the vector of values, but only after a few (900) iterations to observe only the long-term behavior
		x = r*x*(1-x);						// Perform the logistic map operation x_n+1 = r*x_n*(1-x_n)
	}

	return out;
}

int main(){
	vector<double> rvals;					// R values (there will be lots of repeats since each r value has 100 solutions)
	vector<double> solutions;				// Solutions

	for(double r = 1; r < 4; r += dr){		// between r=1 and r=4
		vector<double> s = getSolutions(r);	// Get the solutions for that value of r

		for(int i = 0; i < s.size(); i++){	// Go through all solutions
			solutions.push_back(s[i]);		// Add them to the main list
			rvals.push_back(r);				// Along with the value of r they were recorded at
		}
	}

	cout << "Done with calculation!\n";		// This is for debug

	plotStuff(rvals,solutions);				// Plot it!
}
#include <FindPeriod.hpp>
#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

/**********************************************************

Plots x_n vs. n. The main function is pretty simple. The
printFixedPoints function (after checking for sillyness)
Iterates the logistic function until FPVAL to remove the
error near the origin, then prints out p values, where p
is the period. Values at each point of the period are
the fixed points.

**********************************************************/

const int CUTOFF = 100;	// Cutoff value for graph
const long FPVAL = 10000; // Value to start finding fixed points
const double r = 3.5;	// Growth rate
const double x_0 = 0.2;	// Initial value

void plotStuff(vector<int> x, vector<double> y){
	Gnuplot gp;										// Define a gnuplot output stream (runs the gnuplot command behind the scenes)
	gp << setprecision(3);
	gp << "set xrange [0:" << CUTOFF << "]\n";		// Set the x and y range for the plot
	gp << "set yrange [0:1]\n";
	gp << "set term wxt font \"FreeSerif,12\"\n";
	gp << "set title \"Logistic Function with x_0 = " << x0 << " and r = " << r << "\"\n";
	gp << "set xlabel \"n\"\n";
	gp << "set ylabel \"x_n\"\n";
	gp << "plot '-' with linespoints lc rgb \"black\" notitle\n";	// Plot the data to be given as a line graph with points, title 
	gp.send1d(boost::make_tuple(x,y));				// Send the data as a tuple
}

void printFixedPoints(){
	int p = getPeriod(r);	// Get period
	cout << "Period: " << p << "\n";

	if(p == 0){
		cout << "Chaotic!\n";	// Notify user if the value of r leads to chaotic behavior
	} else if (x_0 == 0 || x_0 == 1){
		cout << "Trivial!\n";	// Also notify if the initial value is bad
	} else {
		cout << "Fixed points:\n";

		double x = x_0;
		for(int i = 0; i < FPVAL+p; i++){
			x = r*x*(1-x);		// Perform the logistic map operation x_n+1 = r*x_n*(1-x_n)
			if(i >= FPVAL){
				cout << x << "\n";
			}
		}
	}
}

int main(){				// Main section
	vector<double> x_n;	// Stores values for x at generation n
	vector<int> n;		// Store generation n
	
	double x = x_0;		// Current value of x (population)

	for(int i = 0; i < CUTOFF; i++){	// Loop until the cutoff
		x_n.push_back(x);				// Push the x value onto the vector of values
		n.push_back(i);					// same with n
		x = r*x*(1-x);					// Perform the logistic map operation x_n+1 = r*x_n*(1-x_n)
	}

	for(int i = 0; i < x_n.size(); i++){		// Output loop for gnuplot!
		cout << n[i] << "\t" << x_n[i] << "\n";	// x_n vs n
	}

	plotStuff(n,x_n);		// Plot the data

	printFixedPoints();

}
#include <gnuplot-iostream/gnuplot-iostream.h>
#include <iostream>
#include <vector>
using namespace std;

const int CUTOFF = 50;	// Cutoff value for finding attractors
const double r = 1;		// Growth rate
const double x0 = 0.5;	// Initial value

int main(){				// Main section
	vector<double> x_n;	// Stores values for x at generation n
	vector<int> n;		// Store generation n
	
	double x = x0;		// Current value of x (population)

	for(int i = 0; i < CUTOFF; i++){	// Loop until the cutoff
		x_n.push_back(x);				// Push the x value onto the vector of values
		n.push_back(i);					// same with n
		x = r*x*(1-x);					// Perform the logistic map operation x_n+1 = r*x_n*(1-x_n)
	}

	for(int i = 0; i < x_n.size(); i++){		// Output loop for gnuplot!
		cout << n[i] << "\t" << x_n[i] << "\n";	// x_n vs n
	}

}
#include <FindPeriod.hpp>
#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

const long double PRECISION = 0.000001; // Nice and precise
const long double startVal = 2.5;		// Good guess
const long double firstStep = 0.9;		// Also good guess

/**********************************************************

This program uses a numerical binary search to
find the boundary between two conditions based on the
period of the logistic function. The two conditions are
specified by lambdas for moving left (decreasing r) and
moving right (increasing r). Changing the lambdas allows
for detection of different ranges. The current uncommented
ones will detect the point where the logistic map goes
chaotic. The commented ones will detect the boundary
between 2 and 4, then 4 and 8.

Note: the left and right lambdas must be mutually exclusive
and cover the interval [0,inf), otherwise funky things may
happen.

**********************************************************/

// auto leftFunc = [](int pos){ return (pos > 2) || (pos == 0); };		// Boundary between 2 and 4
// auto rightFunc = [](int pos){ return (pos < 4) && (pos != 0); };

// auto leftFunc = [](int pos){ return (pos > 4) || (pos == 0); };		// Boundary between 4 and 8
// auto rightFunc = [](int pos){ return (pos < 8) && (pos != 0); };

auto leftFunc = [](int pos){ return pos == 0; };	// Chaotic Boundary (period function returns 0 when chaotic)
auto rightFunc = [](int pos){ return pos > 0; };

int main(){
	long double step = firstStep;	// Step
	long double pos = startVal;		// Position (r value)

	while(step > PRECISION){	// While the step is larger than the specified precision (see how good you can make it!)

		long period = getPeriod(pos);	// Use the getPeriod function from FindPeriod.hpp
		bool error = false;

		cout << "Period is: " << period << " at pos: " << pos << " and step: " << step << "\n";	// Some nice output

		if(leftFunc(period)){			// If left condition is met
			pos -= step;				// Decrease value by step
		} else if (rightFunc(period)){	// Vice versa
			pos += step;
		} else {
			cout << "BAD LAMBDAS";		// If neither conditions are met (how????) then err0r
		}

		step /= 2;	// Divide step 2
	}

	cout << setprecision(-(int)log10(PRECISION)+2);	// Set output precision to number of zeros in PRECISION
	
	cout << "Logistic Function has boundary between conditions at ~ " << pos << "\n"; // Output
}
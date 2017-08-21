#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
const long START = 100000;
const long MAX_PERIOD = 999999;
const long double THRESHOLD = 0.0000000001;
const long double MAX_DIFF = 0.01;
const long double x0 = 0.5;

/**********************************************************

Welcome to the most clever program I've ever made! It's
pretty cool. Here's a brief description. The constants at
the top are constraints for deciding when to start/stop
looking for the period, as well as what to consider a valid
period. 

In the first for loop of the getPeriod function,
the logistic function is iterated quietly until the loop
hits the START value (chosen to be sufficiently large as to
avoid the early inconsisten behavior of the logistic
function). Once the start value is reached, the value of x
is saved in firstVal. The period is incremented for every
subsequent iteration, and a sum is kept of the values of x.
If, at any point, the value of x is within THRESHOLD of 
firstVal, we can say that the logistic function has
completed a full period, and the loop is broken.

X is reset, and a second for loop is started to verify that
the function isn't chaotic at that value. It works by
summing up the next full period of the function
and comparing the sum to the one from the first for loop.
If the difference is over MAX_DIFF, it sets the period to
zero. During the periodic zone, each period should sum to
the same value (since all values in each period will be
essentially identical). In the chaotic zone, however, the
average value may look similar over many iterations, but
the sum will not be identical over any detected "period".

Finally, make sure that the period is a power of 2, since
The functions will only experience period doubling on the
path to chaos, and any other periods detected are sillay!

**********************************************************/

int getPeriod(double r){
	long p = 0;					// Period
	long double x = x0;			// Initial x value
	long double firstVal = 0;	// The value of x_n when n=START
	long double periodSum = 0;	// Sum of x_n within one period

	if(r >= 4){return 0;}

	for(long i = 0; i < MAX_PERIOD; i++){	// Check up to max period (which is just a huge long)
		x = r*x*(1-x);		// Iterate logistic function

		if(i == START){		// If it's time to start looking for the period length
			firstVal = x;	// Save the first value for later comparison
		}

		else if(i > START){	// Continue searching for the period length
			periodSum += x;	// Add x_n to the sum
			p++;			// Increment the period length variable
			
			if(abs(x - firstVal) <= THRESHOLD){	// If x_n is really close to firstVal
				break;	// Then it's probably been a full period; break the loop
			}
		}
	}

	x = x0;									// Reset x
	long double checkSum = 0;				// New sum variable (to compare to the first one)
	for(long i = 0; i < START+p*2+1; i++){	// Go from x_0 to two periods past the start value
		x = r*x*(1-x);		// You know the drill..

		if(i > START+p){	// Start adding to the sum one period after the start value
			checkSum += x;
		}
	}

	if(abs(checkSum - periodSum) > MAX_DIFF){ return 0; }	// If the difference between the sums is too large, say period is 0

	long double isPwr2 = log2((double)p);
	if(floor(isPwr2)-isPwr2 != 0){ return 0; }

	return p; // Output
}

// int main(){
// 	cout << "Period is: " << getPeriod(3.5699456718709) << "\n";
// }
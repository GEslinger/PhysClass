#include <iostream>
#include <vector>
using namespace std;

/***********************************************************************

This file holds functions either used in both programs or that could be
useful in the future. Expect to see a further modified Tools.hpp in my
later projects; I don't want to always be rewriting these functions and
making small mistakes.

***********************************************************************/

double pErr(double g_val, double b_val, double det){	// Function for calculating percent error given g, b, and the determined value of v_t
	double accepted = sqrt(abs(g_val/b_val));			// The accepted v_t is sqrt(g/b)- see my derivation for v as t->inf (absolute value added for robustness' sake)
	return abs((accepted-abs(det))/(accepted));			// Calculate percent error with absolute values to ensure result is positive and correct
}


double sgnChangeTime(vector<double> a, vector<double> t){	// Function to find the value of a vector at the first sign change of another
	bool sgn = a[1] > 0;				// Take the first non-initial value of the vector; sign will be stored as true(+) and false(-)
	bool oldSgn = sgn;					// Variable for the value of sgn in the previous run of the loop

	for(int i = 1; i < a.size(); i++){	// Iterate over a
		sgn = a[i] > 0;					// Check the sign of a[i]
		if(sgn != oldSgn){				// If the sign is different from the last sign, there's been a change!
			return t[i];				// Return the value of the secondary vector at the index of the sign change
		}
		oldSgn = sgn;	// Save the value of sgn to compare in the next run of the loop
	}

	return 0;			// Return 0 as a failsafe if there isn't any sign change
}


double getMaxVal(vector<double> a){		// My function to get the largest value in a vector
	double maxVal = a[0];				// Initialize the maximum with a best guess (the first element of a)
	for(int i = 0; i < a.size(); i++){	// Iterate over a
		if(a[i] > maxVal) maxVal = a[i];// If a[i] is greater than the maximum, consider it the new maximum
	}
	return maxVal;	// Return the max
}


double getMinVal(vector<double> a){		// This function is identical to getMaxVal except that the comparison is flipped to find the smallest a[i]
	double minVal = a[0];
	for(int i = 0; i < a.size(); i++){
		if(a[i] < minVal) minVal = a[i];
	}
	return minVal;
}

#ifndef TOOLS_H
#define TOOLS_H
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#define PI 3.1415926


int inputInt(int def, std::string title){		// Function to prompt and input a double, accepts parameters for name and default value
	std::string input;	// Input string
	int output;		// Output double
	bool valid = false;	// Only true if output is initialized properly

	std::cout << "Enter a valid int for " << title;	// Prompt
	std::cout << " (default is " << def << "): ";		// State default

	while(!valid){	// Go until a valid input (or broken by return statement)
		try{		// Try statement, mostly because of std::stod
			std::getline(std::cin, input);	// Feed a line into input
			if(input.empty()) return def;	// If the line is empty (just pressed enter), return the default value

			output = std::stoi(input);		// Otherwise, try converting the string to a double- WHICH MAY THROW EXCEPTIONS
			if(output != output) throw "Not a Number!";	// Also, to find if the sillay user entered 'NaN', check for the equality property violation.
			valid = true;					// If everything succeeded, set valid to true.
		} 
		catch(...) {									// Catch all exceptions
			std::cout << "Invalid input, re-enter: ";	// Kindly tell user to not be dumb
		}
	}

	return output;	// Output the double!
}

double inputDouble(double def, std::string title){		// Function to prompt and input a double, accepts parameters for name and default value
	std::string input;	// Input string
	double output;		// Output double
	bool valid = false;	// Only true if output is initialized properly

	std::cout << "Enter a valid double for " << title;	// Prompt
	std::cout << " (default is " << def << "): ";		// State default

	while(!valid){	// Go until a valid input (or broken by return statement)
		try{		// Try statement, mostly because of std::stod
			std::getline(std::cin, input);	// Feed a line into input
			if(input.empty()) return def;	// If the line is empty (just pressed enter), return the default value

			output = std::stod(input);		// Otherwise, try converting the string to a double- WHICH MAY THROW EXCEPTIONS
			if(output != output) throw "Not a Number!";	// Also, to find if the sillay user entered 'NaN', check for the equality property violation.
			valid = true;					// If everything succeeded, set valid to true.
		} 
		catch(...) {									// Catch all exceptions
			std::cout << "Invalid input, re-enter: ";	// Kindly tell user to not be dumb
		}
	}

	return output;	// Output the double!
}

double weight(double x, double u, double s){		// Returns PCF(x,u,s), aka the value of the std. normal curve at x
	double term1 = 1/std::sqrt(2*PI*s*s);			// with mean u and std.dev. s
	double term2 = std::exp(-((x-u)*(x-u))/(2*s*s));
	return term1*term2;
}

std::vector<double> expAvg(std::vector<double> v, double s){	// A weighted rolling average using the normal distribution.
	std::vector<double> out;	// The rolling average output

	for(double i = 0; i < v.size(); i++){	// For each point,
		double sum = 0;						// Set up a sum of weighted neighbors
		for(double j = 0; j < v.size(); j++){		// For each neighbor
			double z_j = (j-i)/(v.size());			// Convert their distance from the point into a z-score
			sum += v[j]*weight(z_j,0,s)/v.size();	// Plug it into the normal curve, then weight it by value and normalize
		}
		out.push_back(sum);	// Add the weighted sum of neighbors to the output
	}
	return out;	// Return the rolling average!
}

#endif
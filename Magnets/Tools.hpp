#ifndef TOOLS_H
#define TOOLS_H
#include <iostream>
#include <string>
#include <vector>


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

double averageVec(std::vector<double>& v){
	double sum = 0;
	for(int i = 0; i < v.size(); i++) sum += v[i];
	return sum/v.size();
}

std::vector<double> rollingAverage(std::vector<double> v, int s){
	std::vector<double> out;
	std::vector<double> w;
	int count = 0;

	for(int i = 0; i < v.size(); i++){
		w.push_back(v[i]);
		if(i > s){
			w.erase(w.begin());
		}
		out.push_back(averageVec(w));
	}

	return out;
}

std::vector<double> derivate(std::vector<double> x, std::vector<double> y){	// Returns dy/dx
	std::vector<double> out;
	out.push_back(0);

	for(int i = 1; i < y.size(); i++){
		out.push_back((y[i]-y[i-1])/(x[i]-x[i-1]));
	}

	return out;
}

#endif
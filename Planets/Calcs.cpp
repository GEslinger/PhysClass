#include <vector>
#include <utility>
#include <cmath>
#include <iostream>
#include "Planet.hpp"
using namespace std;

pair<double,double> extrVal(vector<double> v){
	double max = v[0];
	double min = v[0];
	for(double d : v){
		if(d > max) max = d;
		if(d < min) min = d;
	}
	return make_pair(min,max);
}

template<typename T> int sgn(T val){
	return (val > T(0)) - (val < T(0));
}

double getMag(vec3D v){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

double getAvg(vector<double> v){	// Get the arithmetic mean of a data set
	double sum = 0;
	for(int i = 0; i < v.size(); i++){	// Iterate through data
		sum += v[i];			// Add number to sum
	}
	return sum/(double)v.size();// Output the average
}

double getEcc(vector<vec3D> base, vector<vec3D> planet){
	vector<double> dists;

	for(int i = 0; i < base.size(); i++){

		double dist = getMag(planet[i]-base[i]);

		dists.push_back(dist);
	}

	auto periaph = extrVal(dists);
	double a = periaph.second;
	double b = periaph.first;

	return (a-b)/(a+b);
}

double getSMA(vector<vec3D> base, vector<vec3D> planet){
	vector<double> dists;

	for(int i = 0; i < base.size(); i++){

		double dist = getMag(planet[i]-base[i]);

		dists.push_back(dist);
	}

	auto periaph = extrVal(dists);
	
	return (periaph.second+periaph.first)/2;
}

vector<double> toLogLog(vector<double> x){	// Converts each number in a vector to its natural log
	vector<double> out;
	for(int i = 0; i < x.size(); i++){
		out.push_back(log(x[i]));
	}

	return out;
}

pair<double,double> getLeastSquares(vector<double> x, vector<double> y){	// Pair of doubles for (a,b) of the linear least squares y=ax+b
	double xbar = getAvg(x);
	double ybar = getAvg(y);

	double topSum = 0;
	double botSum = 0;
	for(int i = 0; i < x.size(); i++){ // Algorithm courtesy of Dr. Kroeger's Stats Class
		topSum += (x[i] - xbar)*(y[i] - ybar);
		botSum += (x[i] - xbar)*(x[i] - xbar);
	}

	double slope = topSum/botSum;
	double intercept = ybar - slope*xbar;

	return make_pair(slope,intercept);	// Output!
}

double getPeriod(vector<vec3D> base, vector<vec3D> planet, vector<double> time){
	vector<double> dists;
	bool changing = true;
	int extrema = 0;
	double top = 0;
	double bot = 0;

	for(int i = 0; i < base.size(); i++){
		double dist = getMag(planet[i]-base[i]);

		if(dist > getMag(planet[top]-base[top])){
			top = i;
			changing = true;
		}
		else if(dist < getMag(planet[bot]-base[bot])){
			bot = i;
			changing = true;
		}
		else if(changing){
			changing = false;
			extrema++;
		}


		if(extrema > 2){
			break;
		}
	}

	return 2*fabs(time[bot] - time[top]);
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
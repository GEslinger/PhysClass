#include <vector>
#include <cmath>
#include <utility>
#include <iostream>
using namespace std;

// Functions for analyzing the error and creating the log-log plot!

double getAvg(vector<double> v){	// Get the arithmetic mean of a data set
	double sum = 0;
	for(int i = 0; i < v.size(); i++){	// Iterate through data
		sum += v[i];			// Add number to sum
	}
	return sum/(double)v.size();// Output the average
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

vector<double> toLogLog(vector<double> x){	// Converts each number in a vector to its natural log
	vector<double> out;
	for(int i = 0; i < x.size(); i++){
		out.push_back(log(x[i]));
	}

	return out;
}

double globalError(vector<double> x, vector<double> t){	// Calculates global error against x = e^-t using final values
	double e = exp(-t.back())-x.back();
	return fabs(e);	// Return absolute value of the error
}
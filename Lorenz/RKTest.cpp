#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include "Methods/Method.hpp"
#include "RKTest.hpp"
#define FUNC [](double t, double x)	// Abbreviation for lambda function
using namespace std;


/***************************************************************************

This program uses polymorphism because the only difference in the numerical 
integration methods is the step function. It caluates the global truncation
error in x = -x' for varying dt, using both Runge-Kutta and Euler-Cromer.
It also calculates, using the trend line for the data, what the maximum dt
for each method can be to get an error of <= 0.001.

***************************************************************************/


const int MAX_TIME = 1;				// Maximum time for the simulation to run
auto testStep = FUNC{return -x;};	// Differential equation used


void simMethod(Method* m, vector<double>& v, vector<double>& t, double dt){	// Performs a specified numerical integration method for a certain time.
	for(double i = 0; i < MAX_TIME; i += dt){	// Go until the max time
		v.push_back(m->getX());	// Record the X and T value
		t.push_back(m->getT());	
		m->step();				// Perform the step function
	}
}

void getError(Method* m, vector<double>& dtVec, vector<double>& error){	// Given a method, this method tests and records global truncation error for multiple dt
	vector<double> vals;	// Track the values and time
	vector<double> time;

	for(double t = 0.05; t > 0.001; t -= 0.001){	// Go from dt = 0.05 to 0.001 (49 data points)
		m->reset(1,t);						// Reset the values of the numerical integrator
		simMethod(m,vals,time,t);			// Simulate it for the specified time
		double e = globalError(vals,time);	// Calculate the global truncation error
		error.push_back(e);	// Record the error and corresponding dt
		dtVec.push_back(t);

		vals.erase(vals.begin(),vals.end());	// Reset the values and time for the next iteration
		time.erase(time.begin(),time.end());
	}
}

int main(){

	vector<double> error;	// Values and time for both methods, saving memory
	vector<double> dt;

	Method* rk = new RK(1,0.1,testStep);	// Pointers to instances of the two numerical integration methods
	Method* ec = new EC(1,0.1,testStep);	// RK is Runge-Kutta 4th order and EC is Euler-Cromer
	getError(rk,dt,error);	// Get the error vs. dt data for RK.

	auto logDt = toLogLog(dt);		// Convert data to log-log. Dynamic type inference saves us from importing a bunch of one use libraries
	auto logErr = toLogLog(error);
	auto line = getLeastSquares(logDt, logErr);	// Gets the least squares linear regression line of the log-log data. line.first is slope, line.second is y-intercept
	cout << "Trendline for RK: y = " << line.first << "*x + " << line.second << endl;					// Output the trendline
	cout << "Predicted dt for error of 0.001: " << exp((log(0.001)-line.second)/line.first) << "\n\n";	// And use it to calculate the dt necessary for an error of 0.001
	plotErrorAndLine(logDt,logErr,line,"Runge-Kutta","rk.png");	// Plot log-log and trendlie

	
	error.erase(error.begin(),error.end());	// Reset error and dt terms instead of making more vectors
	dt.erase(dt.begin(),dt.end());



	getError(ec,dt,error);	// This is the same as above, just with Euler-Cromer.
	logDt = toLogLog(dt);
	logErr = toLogLog(error);
	line = getLeastSquares(logDt, logErr);
	cout << "Trendline for EC: y = " << line.first << "*x + " << line.second << endl;
	cout << "Predicted dt for error of 0.001: " << exp((log(0.001)-line.second)/line.first) << "\n";
	plotErrorAndLine(logDt,logErr,line,"Euler-Cromer","ec.png");


	delete rk;	// Garbage collection!
	delete ec;
	return 0;
}
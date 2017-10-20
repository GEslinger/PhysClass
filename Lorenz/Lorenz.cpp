#include <iostream>
#include <vector>
#include "Methods/Method.hpp"
#include "Lorenz.hpp"
#define FUNC [&](double t, double x)	// Abbreviation for lambda function
using namespace std;

/*******************************************************************

This program does everything Lorenz! It combines stuff from a bunch
of other programs, most importantly Method.hpp and RK.cpp. It does
numerical integration by constantly updating the equations of a
4th order Runge-Kutta integrator. First, it simulates a single run
of the Lorenz system with the values from the user, then resets r
and goes into a loop to generate the points for the bifurcation diagram

*******************************************************************/

const double x_0 = inputDouble(1, "Initial x");	// Initial values of x, y and z
const double y_0 = inputDouble(0, "Initial y");
const double z_0 = inputDouble(0, "Initial z");

double s = inputDouble(10, "Sigma");	// Sigma, Rho, and Beta- not constants so they can be modified for the bifurcation diagram
double r = inputDouble(25, "Rho");
double b = inputDouble(8./3., "Beta");

const double MAX_TIME = inputDouble(50, "Max time");	// Initial valus important to how long the program takes
const double dt = inputDouble(0.001, "dt (0.0001 is best)");
const double MAX_R = inputDouble(200, "Max r");
const double dr = inputDouble(1, "dr");


struct path3D{			// Variable that stores the path of a particle in the system
	vector<double> x;
	vector<double> y;
	vector<double> z;
	vector<double> t;
};

void addNextPoint(path3D& path, Method* dx, Method* dy, Method* dz){	// This method adds the next point onto the 3D path. It performs one timestep.
	double px = path.x.back();	// Assignments to shorten the equations
	double py = path.y.back();
	double pz = path.z.back();

	dx->changeFunc(FUNC{return s*(py-px);});	// Redefine the differential equations with the most recent values.
	dy->changeFunc(FUNC{return -px*pz+r*px-py;});
	dz->changeFunc(FUNC{return px*py-b*pz;});

	dx->step();	// Step through each one
	dy->step();
	dz->step();

	path.x.push_back(dx->getX());	// Add new values to the path!
	path.y.push_back(dy->getX());
	path.z.push_back(dz->getX());
	path.t.push_back(dx->getT());
}

void resetPath(path3D& path){	// Resets the path and pushes on the initial values
	path.x.erase(path.x.begin(),path.x.end());
	path.y.erase(path.y.begin(),path.y.end());
	path.z.erase(path.z.begin(),path.z.end());

	path.x.push_back(x_0);
	path.y.push_back(y_0);
	path.z.push_back(z_0);
	path.t.push_back(0);
}

vector<double> getExtrema(vector<double> v){	// Gets the extremea of the last 10% of a function (to filter out initial stability)
	vector<double> out;		// Vector to hold extrema
	double back1 = v[0];	// Keep track of the past two points
	double back2 = v[0];
	for(int i = v.size()*0.90; i < v.size(); i++){	// Go from 90% of the end to the end
		if(back1 > back2 && v[i] < back1){	// If there was a peak in the data
			out.push_back(v[i]);	// Add to list
		}
		if(back1 < back2 && v[i] > back1){	// Also look for troughs in data
			out.push_back(v[i]);	// Add to list
		}

		back2 = back1;	// Update the past values
		back1 = v[i];
	}

	return out;	// Output
}



int main(){

	cout << "Allocating memory for differential equations...\n";	// Tell the user what's going

	path3D p;	// Path variable (it can get big and it's hard to make/destroy, so only one is used)
	Method* dx = new RK(x_0,dt,FUNC{return 0;});	// Initialize pointers to differential equation integraters
	Method* dy = new RK(y_0,dt,FUNC{return 0;});
	Method* dz = new RK(z_0,dt,FUNC{return 0;});

	p.x.push_back(x_0);	// Set up the initial values
	p.y.push_back(y_0);
	p.z.push_back(z_0);
	p.t.push_back(0);


	cout << "Simulating system for phase space graphs\n";

	for(double t = 0; t < MAX_TIME; t += dt){	// Does a full simulation of the system with the values (specifically r) that the user input.
		addNextPoint(p,dx,dy,dz);	// Add points!
	}

	cout << "Plotting phase space and z vs t...\n";

	plot3DCurve(p.x,p.y,p.z,s,r,b);	// Plot graphs
	plotOneVar(p.z,p.t,s,r,b);

	resetPath(p);	// Reset the path

	vector<vector<double>> solutions;	// Solutions (values of z during numerical integration)
	vector<double> rvals;	// R values when those solutions occurred

	cout << "Simulating system a bunch for bifurcation diagram (You can kill it here if you don't want one)...\n";	// :)
	cout << "________________________________________\n";	// Prep for loading bar
	double pointsAdded = 0;	// How many points have been added to the loading bar


	for(r = 10; r < MAX_R; r += dr){	// Go through different values of r
		for(double t = 0; t < MAX_TIME; t += dt){	// For each, run a full simulation
			addNextPoint(p,dx,dy,dz);
		}

		solutions.push_back(getExtrema(p.z));	// Add the extrema of z as the solutions
		rvals.push_back(r);						// Add the current r value

		dx->reset(x_0,dt);	// Reset everything
		dy->reset(y_0,dt);
		dz->reset(z_0,dt);
		resetPath(p);

		if(pointsAdded/40.*MAX_R < r){	// Add a piece to the loading bar ever 40th of the way done
		 	cout << "|" << flush;
		 	pointsAdded++;
		}
	}

	cout << "\n\nPlotting bifurcation diagram\n";

	plotBifur(rvals,solutions);	// Plot the bifurcation diagram

	delete dx;	// Delete pointers to prevent memory leaks
	delete dy;
	delete dz;

	cout << "Done\n";	// Done!
}
#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "Tools.hpp"
#include "Magnet.hpp"
using namespace std;

/*****************************************************************************

This program contains the main function for the project, as well as graphing
capability. Several constants are prompted from the user, and several are just
assumed for the sake of consistency. The program first seeds the random
number generator, vital for the Monte-Carlo method to function. It then
creates an allocated magnet object, initialized with random spins and the
temperature that the user input. It simulates for the specified number of
iterations and then plots the final matrix of spins. 

It then jumps into a loop to generate the Energy and Magnetization vs. Time 
plots. For each temperature, mag is reset to all zeroes (to make graphs easier 
to understand) and simulated. The final energy and magnetization are recorded 
and added to their respective vectors along with time. After the loop exits,
a running exponential average is performed on both data vectors before they
are plotted. The Magnet object is then deleted and the program exits.

*****************************************************************************/

const int lattice = inputInt(50,"lattice size");				// Constants used for simulation / initialization
const double defaultJ = 1;										// of the Magnet object. J = kb = 1 from the assignment
const double firstTemp = inputDouble(1,"ambient temperature");;
const double defaultKb = 1;
const double tStep = 0.01;
const int MAX_ITERS = inputInt(100,"maximum iterations");

void plotSpinMatrix(Magnet* m){			// Plot the matrix of spins!
	Gnuplot gp; 
	gp << setprecision(3);
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set title \"Spins of a Randomized Ising Model After " << MAX_ITERS;
	gp << " iterations of the Metropolis Algorithm\\nwith Ambient Temperature " << firstTemp;
	gp << ", k_b = " << defaultKb << ", J = " << defaultJ << "\"\n";
	gp << "set output \"mag.png\"\n";
	gp << "set palette grey\n";
	gp << "set pm3d map\n";
	gp << "unset key\n";
	gp << "unset colorbox\n";
	gp << "splot '-' matrix with image\n";
	gp.send1d(m->getAllSpins());		// For some reason, gnuplot requires matrix input from a 2-D array to be sent as 1-D data
}

void plotEnergy(vector<double>& eng, vector<double>& t){	// Plotting energy per spin
	Gnuplot gp;
	gp << setprecision(3);
	gp << "set xrange [1:4]\n";
	gp << "set yrange [-2:0]\n";
	gp << "set title \"Energy per Spin vs. Temperature in the Ising Model\\n";
	gp << "with k_b = " << defaultKb << ", J = " << defaultJ << "\"\n";
	gp << "set xlabel \"Temperature (inv. Boltzmann Constants)\"\n";
	gp << "set ylabel \"Energy per Spin (E_{/Symbol a} / N)\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set output \"energy.png\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(t,eng));
}

void plotMagnetization(vector<double>& v, vector<double>& t){	// Magnetization per spin
	Gnuplot gp;
	gp << setprecision(3);
	gp << "set xrange [1:4]\n";
	gp << "set yrange [-1:0.1]\n";
	gp << "set title \"Average Magnetization Per Spin vs. Temperature in the Ising Model\\n";
	gp << "with k_b = " << defaultKb << ", J = " << defaultJ << "\"\n";
	gp << "set xlabel \"Temperature (inv. Boltzmann Constants)\"\n";
	gp << "set ylabel \"Magnetization Per Spin\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set output \"magnetization.png\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(t,v));
}

int main(){						// Main function
	srand(time(NULL));			// Seed the random number generator (this carries to the other programs)
	Magnet* mag = new Magnet(lattice, firstTemp, defaultKb, defaultJ);	// Initialize the Magnet object
	vector<double> temp;		// Create vectors to store data for later plots
	vector<double> energy;
	vector<double> mzation;

	mag->simulate(MAX_ITERS);	// Simulate the magnet with user-given parameters
	plotSpinMatrix(mag);		// Plot the final spins
	mag->setNeg();				// Reset to all spin down for nest segment

	for(double t = 0.8; t < 4.2; t+= tStep){	// Bounds chosen to allow rolling average to stabilize at ends (only plotting 1-4)
		mag->setTemp(t);						// Set temperature
		mag->simulate(MAX_ITERS);				// and simulate!
		energy.push_back(mag->getEnergy());		// Then store important quantities
		mzation.push_back(mag->getMag());
		temp.push_back(t);						// As well as the temperature
		mag->setNeg();							// Reset for next simulation
		cout << "Finished T = " << t << endl;	// Just to track progress, print that it finished an iteration
	}

	vector<double> rollingE = expAvg(energy,0.02);	// Get the exponential rolling average of the quantities
	vector<double> rollingM = expAvg(mzation,0.02);

	plotEnergy(rollingE,temp);			// Plot energy and magnetization!
	plotMagnetization(rollingM,temp);

	delete mag;	// Garbage collection (since object was created with "new")
	return 0;	// Return without error
}
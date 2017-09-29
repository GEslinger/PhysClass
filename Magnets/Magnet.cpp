#include <iostream>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include "Magnet.hpp"
using namespace std;


/*****************************************************************************

Implementation of the Magnet class using the Metropolis Algorithm. Methods
are designed to stand alone i.e. total energy isn't tracked while simulating;
a separate method is provided.

*****************************************************************************/


// void printVector(vector<vector<bool>> v){	// For debug purposes, print a 2-D vector 
// 	for(int i = 0; i < v.size(); i++){			// Why isn't this a part of the vector class?!
// 		for(int j = 0; j < v.size(); j++){		
// 			cout << v[i][j] << " ";
// 		}
// 		cout << endl;
// 	}
// }

Magnet::Magnet(int sz, double temp, double boltz, double energy){	// Constructor, accepting params for...
	size = sz;	// Size
	T = temp;	// Initial temperature
	kb = boltz;	// Boltzmann Constant
	J = energy;	// Energy of a spin

	for(int i = 0; i < size; i++){			// Initialize the spins array
		spins.push_back(vector<bool>(0));	// Make sure each index has a row
		for(int j = 0; j < size; j++){
			spins[i].push_back(0);			// And each row is filled with zeroes
		}
	}

	randomize();	// Randomize it to start!
}

void Magnet::setTemp(double temp){	// Public method for updating temperature, used in the loop in Ising.cpp
	T = temp;
}

std::vector<std::vector<bool>> Magnet::getAllSpins() const {	// Gets all spins, used in plotting spin matrix
	return spins;
}

void Magnet::printSpins(){				// Also for debug purposes, but this one prints -1 and 1 instead of 0 and 1
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			cout << getSpin(i,j);
			cout << "\t";
		}
		cout << endl;
	}
}

void Magnet::randomize(){							// Function to randomize spins!
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){				// For each spin
			double choice = rand()/(double)RAND_MAX;// Generate a random number between 0 and 1

			if(choice > 0.5) spins[i][j] = true;	// If it's greater that 0.5, spin up
			else spins[i][j] = false;				// Else (<= 0.5) spin down, splitting the probability evenly
		}
	}
}

void Magnet::setNeg(){					// Set to negative (spin down)
	for(int i = 0; i < size; i++){		// Used to define a preferential state
		for(int j = 0; j < size; j++){	// so that the magnetization vs. temp curve will behave nicely
			spins[i][j] = false;
		}
	}
}

int Magnet::getSpin(int i, int j) const {	// Get a spin (must be able to handle out of bounds!)
	if(i < 0) i += size;				// If the number is negative, wrap around
	if(j < 0) j += size;
	return spins[i%size][j%size]*2-1;	// If the number is too high, the modulo will wrap it, then convert boolean (0 to 1) to spin (-1 to 1)
}

double Magnet::getSpinEnergy(int i, int j) const {	// Get the current energy of a spin
	int current = getSpin(i,j);			// Define the current spin
	int top = getSpin(i,j+1);			// Explore the Von Neumann neighborhood
	int bot = getSpin(i,j-1);
	int right = getSpin(i+1,j);
	int left = getSpin(i-1,j);
	double sum = current*(double)top;	// Sum up energies 
	sum += current*(double)bot;
	sum += current*(double)right;
	sum += current*(double)left;
	return -J*sum;		// Don't forget the actual energy per spin!
}

bool Magnet::shouldFlip(int i, int j) const {	// Should spins[i][j] be flipped given the current state of things?

	double flipEnergy = -getSpinEnergy(i,j)*2;	// Expanded it's E_f - E_i, but E_f = -E_i so it's simplified to -E_i*2 for speed


	if(flipEnergy < 0) return true;	// If negative energy to flip (flipping is preferred) then yes!

	double boltzmann = exp(-flipEnergy / (double)(kb*T));	// Find the boltzmann factor
	double p = ((double)rand())/(double)RAND_MAX;			// Generate random number

	if(p < boltzmann) return true;	// If the random probability to flip is less than boltzmann factor, then yes!

	return false;	// Otherwise no :(
}

void Magnet::simulate(int MAX_ITERS){	// Main function to simulate

	for(int iters = 0; iters < MAX_ITERS; iters++){	// Until MAX_ITERS is reached
		for(int steps = 0; steps < size*size; steps++){	// Select N*N spins
			int i = rand()%size;	// Chose a random spin (by index)
			int j = rand()%size;

			if(shouldFlip(i,j)){	// If it should flip,
				spins[i][j] = !spins[i][j];	// Flip it!
			}
		}
	}
}

double Magnet::getEnergy() const {		// Gets average energy per spin
	double sum = 0;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){	// Iterate through each spin
			sum += getSpinEnergy(i,j);	// Add energy to the sum
		}
	}

	return sum / (double)(size*size) / 2;	// Make it average per spin, then div by 2 to remove redundant comparisons.
}


double Magnet::getMag() const {			// Average magnetization per spin
	double sum = 0;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){	// For each spin
			sum += getSpin(i,j);		// Add it to the sum
		}
	}

	return sum / (size*size);	// Make average!
}
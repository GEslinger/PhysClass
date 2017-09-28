#include <iostream>
#include <cmath>
#include <cstdlib>
#include <unistd.h>
#include "Magnet.hpp"
using namespace std;


void printVector(vector<vector<bool>> v){
	for(int i = 0; i < v.size(); i++){
		for(int j = 0; j < v.size(); j++){
			cout << v[i][j] << " ";
		}
		cout << endl;
	}
}

Magnet::Magnet(int sz, double temp, double boltz, double energy){
	size = sz;
	kb = boltz;
	J = energy;
	T = temp;

	for(int i = 0; i < size; i++){
		spins.push_back(vector<bool>(0));
		for(int j = 0; j < size; j++){
			spins[i].push_back(0);
		}
	}

	randomize();
}

void Magnet::setTemp(double temp){
	T = temp;
}

std::vector<std::vector<bool>> Magnet::getAllSpins() const {
	return spins;
}

void Magnet::printSpins(){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			cout << getSpin(i,j);
			cout << "\t";
		}
		cout << endl;
	}
}

void Magnet::randomize(){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			double choice = rand()/(double)RAND_MAX;

			if(choice > 0.5) spins[i][j] = true;
			else spins[i][j] = false;
		}
	}
}

void Magnet::setNeg(){
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			spins[i][j] = false;
		}
	}
}

int Magnet::getSpin(int i, int j) const {
	if(i < 0) i += size;
	if(j < 0) j += size;
	return spins[i%size][j%size]*2-1;
}

double Magnet::getSpinEnergy(int i, int j) const {
	int current = getSpin(i,j);
	int top = getSpin(i,j+1);
	int bot = getSpin(i,j-1);
	int right = getSpin(i+1,j);
	int left = getSpin(i-1,j);
	double sum = current*(double)top;
	sum += current*(double)bot;
	sum += current*(double)right;
	sum += current*(double)left;
	return -J*sum;
}

bool Magnet::shouldFlip(int i, int j) const {

	double flipEnergy = -getSpinEnergy(i,j)*2;


	double boltzmann = exp(-flipEnergy / (double)(kb*T));
	double p = ((double)rand())/(double)RAND_MAX;
	//cout << flipEnergy << " " << p << " " << boltzmann << endl;

	if(flipEnergy < 0) return true;

	if(p < boltzmann){
		//cout << "Boltzmann flip!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";
		return true;
	}

	return false;
}

void Magnet::simulate(int MAX_ITERS){

	for(int iters = 0; iters < MAX_ITERS; iters++){
		for(int steps = 0; steps < size*size; steps++){
			int i = rand()%size;
			int j = rand()%size;

			if(shouldFlip(i,j)){
				spins[i][j] = !spins[i][j];
			}
		}
	}
}

double Magnet::getEnergy() const {
	double sum = 0;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			sum += getSpinEnergy(i,j)/2;	// div by 2
		}
	}

	return sum / (double)(size*size);
}


double Magnet::getMag() const {
	double sum = 0;
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			sum += getSpin(i,j);
		}
	}

	return sum / (size*size);
}
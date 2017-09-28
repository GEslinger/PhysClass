#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <iomanip>
#include <ctime>
#include "Tools.hpp"
#include "Magnet.hpp"
using namespace std;

const int lattice = inputInt(100,"lattice size");
const double defaultJ = 1;
const double firstTemp = inputDouble(1,"ambient temperature");;
const double defaultKb = 1;
const double tStep = 0.001;
const int MAX_ITERS = inputInt(200,"maximum iterations");

void plotSpinMatrix(Magnet* m){
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
	gp.send1d(m->getAllSpins());
}

void plotEnergy(vector<double>& eng, vector<double>& t){
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

void plotDeriv(vector<double>& eng, vector<double>& t){
	Gnuplot gp;
	gp << setprecision(3);
	gp << "set xrange [1:4]\n";
	gp << "set yrange [0:1.5]\n";
	gp << "set title \"Heat Density vs. Temperature in the Ising Model\\n";
	gp << "with k_b = " << defaultKb << ", J = " << defaultJ << "\"\n";
	gp << "set xlabel \"Temperature (inv. Boltzmann Constants)\"\n";
	gp << "set ylabel \"Energy per Spin (E_{/Symbol a} / N)\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set output \"Denergy.png\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(t,eng));
}

int main(){
	srand(time(NULL));
	Magnet* mag = new Magnet(lattice, firstTemp, defaultKb, defaultJ);
	vector<double> time;
	vector<double> energy;
	vector<double> magnetism;

	mag->simulate(MAX_ITERS);
	plotSpinMatrix(mag);
	mag->randomize();

	for(double t = 0.8; t < 4; t+= tStep){
		mag->setTemp(t);
		mag->simulate(MAX_ITERS);
		energy.push_back(mag->getEnergy());
		time.push_back(t);
		mag->randomize();
		cout << "Finished " << t << endl;
	}

	vector<double> rolling = rollingAverage(energy,0.2/tStep);
	vector<double> deriv = derivate(time,rolling);
	vector<double> rollingderiv = rollingAverage(deriv,0.2/tStep);

	plotEnergy(rolling,time);
	plotDeriv(rollingderiv,time);

	delete mag;
	return 0;
}
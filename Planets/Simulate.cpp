#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include "Planet.hpp"
#include "Simulate.hpp"
using namespace std;

const double G = 6.67408e-11; // m^3 kg^-1 s^-2
const double MAX_TIME = 3.154e+7 * inputDouble(1, "max time in years");
const double RECORD_EVERY = 3.154e+7 / inputDouble(500, "number of points to record each year");
const double SCALE = inputDouble(2, "scale of graph in AU");
const int PLANETS = 9;
const int ASTEROIDS = 0;
double dt = inputDouble(100, "dt");


vec3D getAcceleration(Planet* a, Planet* b){	// Gets the acceleration between two planets
	vec3D ap = a->getPos();	// Temporary position variables
	vec3D bp = b->getPos();
	double dist = sqrt(pow(ap.x-bp.x,2) + pow(ap.y-bp.y,2) + pow(ap.z-bp.z,2));	// Distance

	double term = G*b->getMass() / pow(dist,2);	// First half of Newton's law of gravitation (Using vectors!)
	vec3D out{
		(bp.x-ap.x)*term,	// Multiply by distance component-wise 
		(bp.y-ap.y)*term,
		(bp.z-ap.z)*term
	};

	return out;	// Output acceleration
}

void simulate(Planet** system, vector<vec3D>* track, vector<double>& times, double T, int P, int A, double d){	// Simulate a system of planets!

	double points = 0;	// Keep track of the number of points recorded (used with the argument 'd')

	for(double t = 0; t < T; t += dt){	// From 0 to the max time, with timestep dt
		for(int i = 0; i < P+A; i++){	// Iterate through all planets and asteroids
			Planet* a = system[i];		// a is the planet that will be stepped

			vec3D acc {0,0,0};			// Acceleration vector, to hold the sum of the pulls of all other planets
			for(int j = 0; j < P; j++){	// Iterate through all planets, EXCLUDE ASTEROIDS because they don't have any significant pull
				Planet* b = system[j];
				if(i != j){
					acc += getAcceleration(a,b);
				}
			}
			a->tStep(acc,dt);

			if(points*d < t){
				track[i].push_back(a->getPos());
			}
		}

		for(int i = 0; i < P+A; i++){
			system[i]->update();
		}

		if(points*d < t){
			times.push_back(t);
			cout << points << "/" << floor(T/d) << endl;
			points++;
		}
	}

}

int main(){
	Planet* terran[PLANETS+ASTEROIDS];
	vector<vec3D> track[PLANETS+ASTEROIDS];
	vector<double> times;
	terran[0] = new Planet(vec3D{2.187003065211543E-03,5.768166559108312E-03,-1.294147734354897E-04}, vec3D{-5.280315745929900E-06,5.460827268770582E-06,1.244590551885596E-07}, 1.988544e30);	// sun
	terran[1] = new Planet(vec3D{-2.139370590565288E-01,-4.028814669327753E-01,-1.369419923866817E-02}, vec3D{1.919605110412534E-02,-1.178939724774313E-02,-2.725194971976164E-03}, 3.302e23);	// mercury
	terran[2] = new Planet(vec3D{-6.915411411024813E-01,1.907707656342085E-01,4.244080591200550E-02}, vec3D{-5.318149817917223E-03,-1.962946252497859E-02,3.737564099986648E-05}, 48.685e23);	// venus
	terran[3] = new Planet(vec3D{8.589987108796383E-01,5.110680605545673E-01,-1.568623415833688E-04}, vec3D{-9.031139666444341E-03,1.476253462344845E-02,-1.943130198126555E-07}, 5.97219e24);	// erth
	terran[4] = new Planet(vec3D{-1.590488403238053E+00,4.879788693373922E-01,4.906264799695509E-02}, vec3D{-3.536379998688235E-03,-1.219225340287500E-02,-1.688251041034333E-04}, 6.4185e23);	// mars
	terran[5] = new Planet(vec3D{-4.556745348155565E+00,-2.963008457339381E+00,1.14210860308719E-01}, vec3D{4.025290127025506E-03,-5.968165948463064E-03,-6.526566072536134E-05}, 1898.13e24);	// jupiter
	terran[6] = new Planet(vec3D{-3.158467085324504E-01,-1.005065028034512E+01,1.87322229867813E-01}, vec3D{5.270276974251006E-03,-1.927338311574419E-04,-2.066633331055191E-04}, 5.68319e26);	// saturn
	terran[7] = new Planet(vec3D{1.784724616993690E+01,8.833225342557650E+00-1.984072076144117E-01}, vec3D{-1.773481248035921E-03,3.341641213902314E-03,3.532859459458915E-05}, 86.8103e24);	// uranus
	terran[8] = new Planet(vec3D{2.862016046630078E+01,-8.800209679340895E+00,-4.783572794559496E-01}, vec3D{9.014315108950128E-04,3.018736862613127E-03,-8.311114083455642E-05}, 102.41e24);	// neptune
	for(int i = 0; i < PLANETS+ASTEROIDS; i++){track[i] = vector<vec3D>{};}

	simulate(terran, track, times, MAX_TIME, PLANETS, ASTEROIDS, RECORD_EVERY);

	graphSystem(track,PLANETS,ASTEROIDS,0,SCALE,"solar.png");
	
	double ecc_e = getEcc(track[0],track[3]);
	double period_e = getPeriod(track[0],track[3],times);
	cout << "Eccentricity of Earth: " << ecc_e << endl;
	cout << "Percent error: " << (abs(0.0167-ecc_e)/0.0167)*100 << "%" << endl;
	cout << "Period of Earth: " << period_e << endl;
	cout << "Percent error: " << (abs(3.154e7-period_e)/3.154e7)*100 << "%" << endl << endl;

	
	vector<double> SMA;
	vector<double> period;

	for(int i = 1; i < PLANETS+ASTEROIDS; i++){
		// cout << getSMA(track[0],track[i]) << endl;
		SMA.push_back(getSMA(track[0],track[i]));
		period.push_back(getPeriod(track[0],track[i],times));
	}

	graphLogLog(toLogLog(SMA), toLogLog(period));

	for(Planet* p : terran){
		delete p;
	}
	return 0;
}
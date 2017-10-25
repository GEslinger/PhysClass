#include <iostream>
#include <vector>
#include <cmath>
#include "Planet.hpp"
#include "Simulate.hpp"
using namespace std;

const double G = 6.67408e-11; // m^3 kg^-1 s^-2
const double MAX_TIME = 3.2e+7;
const int MAX_PLANETS = 9;
const int RECORDED_POINTS = 5000;
double dt = 100;


vec3D getAcceleration(Planet* a, Planet* b){
	vec3D ap = a->getPos();
	vec3D bp = b->getPos();
	double dist = sqrt(pow(ap.x-bp.x,2) + pow(ap.y-bp.y,2) + pow(ap.z-bp.z,2));

	double term = G*b->getMass() / pow(dist,3);
	vec3D out{
		(bp.x-ap.x)*term,
		(bp.y-ap.y)*term,
		(bp.z-ap.z)*term
	};

	return out;
}

int main(){
	Planet* terran[MAX_PLANETS];
	vector<vec3D> track[MAX_PLANETS];
	vector<double> time;
	terran[0] = new Planet(vec3D{2.187003065211543E-03,5.768166559108312E-03,-1.294147734354897E-04}, vec3D{-5.280315745929900E-06,5.460827268770582E-06,1.244590551885596E-07}, 1.988544e30);	// sol
	terran[1] = new Planet(vec3D{-2.139370590565288E-01,-4.028814669327753E-01,-1.369419923866817E-02}, vec3D{1.919605110412534E-02,-1.178939724774313E-02,-2.725194971976164E-03}, 3.302e23);	// mercur
	terran[2] = new Planet(vec3D{-6.915411411024813E-01,1.907707656342085E-01,4.244080591200550E-02}, vec3D{-5.318149817917223E-03,-1.962946252497859E-02,3.737564099986648E-05}, 48.685e23);	// naheed
	terran[3] = new Planet(vec3D{8.589987108796383E-01,5.110680605545673E-01,-1.568623415833688E-04}, vec3D{-9.031139666444341E-03,1.476253462344845E-02,-1.943130198126555E-07}, 5.97219e24);	// terra
	terran[4] = new Planet(vec3D{-1.590488403238053E+00,4.879788693373922E-01,4.906264799695509E-02}, vec3D{-3.536379998688235E-03,-1.219225340287500E-02,-1.688251041034333E-04}, 6.4185e23);	// kasei
	terran[5] = new Planet(vec3D{-4.556745348155565E+00,-2.963008457339381E+00,1.14210860308719E-01}, vec3D{4.025290127025506E-03,-5.968165948463064E-03,-6.526566072536134E-05}, 1898.13e24);	// jov
	terran[6] = new Planet(vec3D{-3.158467085324504E-01,-1.005065028034512E+01,1.87322229867813E-01}, vec3D{5.270276974251006E-03,-1.927338311574419E-04,-2.066633331055191E-04}, 5.68319e26);	// kronos
	terran[7] = new Planet(vec3D{1.784724616993690E+01,8.833225342557650E+00-1.984072076144117E-01}, vec3D{-1.773481248035921E-03,3.341641213902314E-03,3.532859459458915E-05}, 86.8103e24);	// uron
	terran[8] = new Planet(vec3D{2.862016046630078E+01,-8.800209679340895E+00,-4.783572794559496E-01}, vec3D{9.014315108950128E-04,3.018736862613127E-03,-8.311114083455642E-05}, 102.41e24);	// vodopan


	for(int i = 0; i < MAX_PLANETS; i++){track[i] = vector<vec3D>{};}

	int points = 0;

	for(double t = 0; t < MAX_TIME; t += dt){
		for(int i = 0; i < MAX_PLANETS; i++){
			Planet* a = terran[i];

			vec3D acc {0,0,0};
			for(int j = 0; j < MAX_PLANETS; j++){
				Planet* b = terran[j];
				if(i != j){
					acc += getAcceleration(a,b);
				}
			}
			a->tStep(acc,dt);

			if((double)points/(double)RECORDED_POINTS*(double)MAX_TIME < t){
				track[i].push_back(a->getPos());
			}
		}

		for(Planet* a: terran){
			a->update();
		}

		if((double)points/(double)RECORDED_POINTS*(double)MAX_TIME < t){
			time.push_back(t);
			cout << points << "/" << RECORDED_POINTS << endl;
			points++;
		}
	}

	graphSystem(track,MAX_PLANETS,0,2e0,"solar.png");
	double ecc = getEcc(track[0],track[3]);
	cout << "Eccentricity: " << ecc << endl;
	cout << "Percent error: " << (abs(0.0167-ecc)/0.0167)*100 << "%" << endl;

	for(Planet* p : terran){
		delete p;
	}
	return 0;
}
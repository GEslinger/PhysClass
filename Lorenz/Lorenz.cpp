#include <iostream>
#include <vector>
#include "Methods/Method.hpp"
#include "Lorenz.hpp"
#define FUNC [&](double t, double x)
using namespace std;

const double x_0 = 1.;
const double y_0 = 0;
const double z_0 = 0;

const double dt = 0.0001;
const double MAX_TIME = 100;

double s = 10;
double r = 25;
double b = 8./3.;

struct path3D{
	vector<double> x;
	vector<double> y;
	vector<double> z;
	vector<double> t;
};

void addNextPoint(path3D& path, Method* dx, Method* dy, Method* dz){
	double px = path.x.back();
	double py = path.y.back();
	double pz = path.z.back();

	dx->changeFunc(FUNC{return s*(py-px);});
	dy->changeFunc(FUNC{return -px*pz+r*px-py;});
	dz->changeFunc(FUNC{return px*py-b*pz;});

	dx->step();
	dy->step();
	dz->step();

	path.x.push_back(dx->getX());
	path.y.push_back(dy->getX());
	path.z.push_back(dz->getX());
	path.t.push_back(dx->getT());
}

void resetPath(path3D& path){
	path.x.erase(path.x.begin(),path.x.end());
	path.y.erase(path.y.begin(),path.y.end());
	path.z.erase(path.z.begin(),path.z.end());

	path.x.push_back(x_0);
	path.y.push_back(y_0);
	path.z.push_back(z_0);
	path.t.push_back(0);
}

vector<double> getExtrema(vector<double> v){
	vector<double> out;
	double back1 = v[0];
	double back2 = v[0];
	for(int i = v.size()*0.95; i < v.size(); i++){
		if(back1 > back2 && v[i] < back1){
			out.push_back(v[i]);
			// cout << "max\n";
		}
		if(back1 < back2 && v[i] > back1){
			out.push_back(v[i]);
			// cout << "min\n";
		}
		back2 = back1;
		back1 = v[i];
	}

	return out;
}



int main(){
	path3D p;
	Method* dx = new RK(x_0,dt,FUNC{return 0;});
	Method* dy = new RK(y_0,dt,FUNC{return 0;});
	Method* dz = new RK(z_0,dt,FUNC{return 0;});

	p.x.push_back(x_0);
	p.y.push_back(y_0);
	p.z.push_back(z_0);
	p.t.push_back(0);

	for(int i = 0; i < MAX_TIME/dt; i++){
		addNextPoint(p,dx,dy,dz);
	}
	plot3DCurve(p.x,p.y,p.z);
	plotOneVar(p.z,p.t);

	resetPath(p);

	vector<vector<double>> solutions;
	vector<double> rvals;

	for(r = 23; r < 25; r += 0.001){
		for(int i = 0; i < MAX_TIME/dt; i++){
			addNextPoint(p,dx,dy,dz);
		}

		solutions.push_back(getExtrema(p.z));
		rvals.push_back(r);

		dx->reset(x_0,dt);
		dy->reset(y_0,dt);
		dz->reset(z_0,dt);
		resetPath(p);
		cout << r << endl;
	}

	plotBifur(rvals,solutions);

	delete dx;
	delete dy;
	delete dz;
}
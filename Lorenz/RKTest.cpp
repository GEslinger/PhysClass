#include <string>
#include <vector>
#include <iostream>
#include <cmath>
#include "Methods/Method.hpp"
#include "RKTest.hpp"
#define FUNC [](double t, double x)
using namespace std;


const int MAX_TIME = 1;
auto testStep = FUNC{return -x;};


void simMethod(Method* m, vector<double>& v, vector<double>& t, double dt){
	for(int i = 0; i < MAX_TIME/dt; i++){
		v.push_back(m->getX());
		t.push_back(m->getT());
		m->step();
	}
	// cout << t.back() << endl;
}

int main(){
	vector<double> vals;
	vector<double> time;

	vector<double> error;
	vector<double> dt;

	Method* rk = new RK(1,0.1,testStep);

	for(double t = 0.05; t > 0.001; t -= 0.001){
		rk->reset(1,t);
		simMethod(rk,vals,time,t);
		double e = globalError(vals,time);
		error.push_back(e);
		dt.push_back(t);

		//cout << t << "\t" << e << endl;

		vals.erase(vals.begin(),vals.end());
		time.erase(time.begin(),time.end());
	}

	auto plot = toLogLog(dt, error);
	auto line = getLeastSquares(plot.first, plot.second);

	cout << line.first << " " << line.second << endl;

	plotErrorAndLine(plot,line,"Runge-Kutta","rk.png");

	delete rk;
	return 0;
}
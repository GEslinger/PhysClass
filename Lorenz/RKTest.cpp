#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include <functional>
#include "Method.h"
#define FUNC [](double t, double x)
using namespace std;

void plotDiffEQ(vector<double> x, vector<double> t, int T, double DT, string title, string fname);

const int MAX_TIME = 5;
const double DT = 0.01;
auto testStep = FUNC{return -x;};

int main(){
	vector<double> vals;
	vector<double> time;
	EC eulerCromer(1,DT,testStep);

	for(int i = 0; i < MAX_TIME/DT; i++){
		vals.push_back(eulerCromer.getX());
		time.push_back(eulerCromer.getT());
		eulerCromer.step();
	}

	plotDiffEQ(vals,time,MAX_TIME,DT,"Euler-Cromer","ec.png");


	vals.erase(vals.begin(),vals.end());
	time.erase(time.begin(),time.end());
	RK rungeKutta(1,DT,testStep);

	for(int i = 0; i < MAX_TIME/DT; i++){
		vals.push_back(rungeKutta.getX());
		time.push_back(rungeKutta.getT());
		rungeKutta.step();
	}

	plotDiffEQ(vals,time,MAX_TIME,DT,"Runge-Kutta","rk.png");

	return 0;
}
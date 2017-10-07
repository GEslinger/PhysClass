#include <string>
#include <cmath>
#include <vector>
#include "Methods/Method.hpp"
#define FUNC [](double t, double x)
using namespace std;

void plotDiffEQ(vector<double> x, vector<double> t, int T, double DT, string title, string fname);

const int MAX_TIME = 5;
const double DT = 0.2;
auto testStep = FUNC{return -x;};


void simMethod(Method* m, vector<double>& v, vector<double>& t){
	for(int i = 0; i < MAX_TIME/DT; i++){
		v.push_back(m->getX());
		t.push_back(m->getT());
		m->step();
	}
}

int main(){
	vector<double> vals;
	vector<double> time;
	Method* ec = new EC(1,DT,testStep);
	Method* rk = new RK(1,DT,testStep);

	simMethod(ec,vals,time);

	plotDiffEQ(vals,time,MAX_TIME,DT,"Euler-Cromer","ec.png");

	vals.erase(vals.begin(),vals.end());
	time.erase(time.begin(),time.end());
	
	simMethod(rk,vals,time);

	plotDiffEQ(vals,time,MAX_TIME,DT,"Runge-Kutta","rk.png");

	delete ec;
	delete rk;
	return 0;
}
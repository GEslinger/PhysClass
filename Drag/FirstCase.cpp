#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <iomanip>
#include <functional>
#include <vector>
#include <cmath>
using namespace std;

const double g = 9.8;
const double b = 1;
const double dt = 0.0001;
const double SIM_TIME = 2;
const double THRESHOLD = 0.01;

function<double(double,double)> dragStep = [](double x, double v){ return g-b*v*abs(v); };

class Euler1D{
public:
	Euler1D(double x_0, double v_0);
	void update(function<double(double,double)>& f);
	double getX() const;
	double getV() const;
private:
	double x;
	double v;
};

Euler1D::Euler1D(double x_0, double v_0){
	x = x_0;
	v = v_0;
}

void Euler1D::update(function<double(double,double)>& f){
	double eulerStep = f(x,v);
	x += v*dt;
	v += eulerStep*dt;
}

double Euler1D::getX() const { return x; }
double Euler1D::getV() const { return v; }


void plotStuff(vector<double> x, vector<double> v, vector<double> t){
	Gnuplot gp;

	gp << setprecision(3);
	gp << "set xrange [0:" << SIM_TIME << "]\n";
	gp << "set yrange [" << v[0] << ":" << v.back()*1.1 << "]\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term wxt font \"FreeSerif,12\"\n";
	gp << "set xlabel \"t (s)\"\n";
	gp << "set ylabel \"v (m/s)\"\n";
	gp << "set title \"Velocity vs. Time with Rayleigh's Drag Equation, b = " << b << "\"\n"; 
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(t,v));

	Gnuplot gp2;

	gp2 << setprecision(3);
	gp2 << "set xrange [0:" << SIM_TIME << "]\n";
	gp2 << "set yrange [" << 0 << ":" << -2 << "]\n";
	gp2 << "set format y \"%.1f\"\n";
	gp2 << "set term wxt font \"FreeSerif,12\"\n";
	gp2 << "set xlabel \"t (s)\"\n";
	gp2 << "set ylabel \"x (m)\"\n";
	gp2 << "set title \"Position vs. Time with Rayleigh's Drag Equation, b = " << b << "\"\n"; 
	gp2 << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp2.send1d(boost::make_tuple(t,x));
}


double pDiff(double a_val, double b_val, double calc){
	double yes = sqrt(a_val/b_val);
	return abs((yes-calc)/((yes+calc)/2));
}


double sgnChangeTime(vector<double> a, vector<double> t){
	bool sgn = a[1] > 0;
	bool oldSgn = sgn;

	for(int i = 1; i < a.size(); i++){
		sgn = a[i] > 0;
		if(sgn != oldSgn){
			return t[i];
		}
		oldSgn = sgn;
	}

	return 0;
}


int main(){
	Euler1D point(0,-20);
	vector<double> x_n;
	vector <double> v_n;
	vector <double> t_n;

	for(double t = 0; t < SIM_TIME; t += dt){
		x_n.push_back(point.getX());
		v_n.push_back(point.getV());
		t_n.push_back(t);
		point.update(dragStep);
	}

	plotStuff(x_n, v_n, t_n);

	double diff = pDiff(g,b,v_n.back());
	cout << "Difference between calculated and final value: " << diff*100 << "%\n";
	if(diff < THRESHOLD){
		cout << "Final value is within " << THRESHOLD*100 << "% of the calculated value\n";
	} else {
		cout << "Final value IS NOT within " << THRESHOLD*100 << "% of the calculated value\n";
	}

	double rising = sgnChangeTime(v_n, t_n);
	double hitground = sgnChangeTime(x_n, t_n);
	cout << "Rising time: " << rising << "\n";
	cout << "Falling time: " << hitground-rising << "\n";

	return 0;
}
#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <functional>
#include <iomanip>
#include "Tools.hpp"
using namespace std;

const double om = 0.5;

function<double(double,double)> springStep = [](double x, double v){ return -om*om*x; };

int main(){
	int b = 20;
	inspect(b);

	return 0;
}
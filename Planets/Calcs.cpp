#include <vector>
#include <utility>
#include <cmath>
#include "Planet.hpp"
using namespace std;

pair<double,double> extrema(vector<double> v){
	double max = v[0];
	double min = v[0];
	for(double d : v){
		if(d > max) max = d;
		if(d < min) min = d;
	}
	return make_pair(min,max);
}

double getEcc(vector<vec3D> base, vector<vec3D> planet){
	vector<double> dists;

	for(int i = 0; i < base.size(); i++){
		double x = base[i].x-planet[i].x;
		double y = base[i].y-planet[i].y;
		double z = base[i].z-planet[i].z;

		double dist = sqrt(x*x+y*y+z*z);
		dists.push_back(dist);
	}

	auto periaph = extrema(dists);
	double a = periaph.second;
	double b = periaph.first;

	return (a-b)/(a+b);
}
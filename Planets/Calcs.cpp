#include <vector>
#include <utility>
#include <cmath>
#include <iostream>
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

template<typename T> int sgn(T val){
	return (val > T(0)) - (val < T(0));
}

double getMag(vec3D v){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

double getEcc(vector<vec3D> base, vector<vec3D> planet){
	vector<double> dists;

	for(int i = 0; i < base.size(); i++){

		double dist = getMag(base[i]-planet[i]);

		dists.push_back(dist);
	}

	auto periaph = extrema(dists);
	double a = periaph.second;
	double b = periaph.first;

	return (a-b)/(a+b);
}

double getPeriod(vector<vec3D> base, vector<vec3D> planet, vector<double> time){
	vec3D firstPos = planet[0]-base[0];
	double lastr = 0;
	double lastdr = 0;

	for(int i = 0; i < base.size(); i++){
		double r = getMag(firstPos - (planet[i]-base[i]));
		double dr = lastr - r;
		cout << dr << "\t" << lastdr << "\t" << r << endl;

		if(sgn(lastdr) > 0 && sgn(dr) < 0){
			return time[i];
		}

		lastdr = dr;
		lastr = r;
	}

	return -1;
}
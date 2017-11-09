#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <utility>
#include <vector>
#include <string>
#include "Graph.hpp"
#include "Planet.hpp"
using namespace std;

/*****************************************************************************

GRAPHING PROGRAM! To graph both the system and the log-log plot

*****************************************************************************/

void graphSystem(vector<vec3D> paths[], int planets, int asts, int base, double scale, string fname){
	Gnuplot gp;

	double sz = scale*149597870700; // Scale from au to meters

	gp << setprecision(3);
	gp << "set xrange [" << -sz << ":" << sz << "]\n";
	gp << "set yrange [" << -sz << ":" << sz << "]\n";
	gp << "set zrange [" << -sz << ":" << sz << "]\n";
	gp << "set xlabel \"x\"\n";
	gp << "set ylabel \"y\"\n";
	gp << "set zlabel \"z\"\n";
	gp << "set term png size 1920,1080 font \"FreeSerif,12\"\n";
	gp << "set title \"ZOLAR ZYZDEM\"\n"; 
	gp << "set output \"" << fname << "\"\n";
	gp << "set hidden3d\n";
	gp << "set view 60, 60, 1, 1.5\n";
	gp << "splot ";

	for(int i = 0; i < planets+asts; i++){	// Plot planets in black and asteroids in red
		gp << "'-' with lines lc rgb \"";
		if(i >= planets){
			gp << "red";
		} else {
			gp << "black";
		}
		gp << "\" notitle, ";
	}
	gp << "\n";
	
	for(int i = 0; i < planets+asts; i++){			// Data must be rearranged into vectors of doubles, removed from vec3D tuple
		vector<double> x;
		vector<double> y;
		vector<double> z;

		for(int j = 0; j < paths[i].size(); j++){
			vec3D v = paths[i][j];
			x.push_back(v.x-paths[base][j].x);		// Adjust coordinates to be centered around the base (the sun)
			y.push_back(v.y-paths[base][j].y);
			z.push_back(v.z-paths[base][j].z);
		}

		gp.send1d(boost::make_tuple(x,y,z));
	}
}

void graphLogLog(vector<double> lSMA, vector<double> lPeriod){
	Gnuplot gp;

	auto line = getLeastSquares(lSMA,lPeriod);			// Get the least squares parameters (slope, intercept) from the Calcs.cpp file

	gp << "set xrange [20:35]\n";
	gp << "set yrange [0:30]\n";
	gp << "set xlabel \"log(SMA)\"\n";
	gp << "set ylabel \"log(period)\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set title \"Log-Log plot of Semi-Major Axis vs. Period\"\n";
	gp << "set output \"ll.png\"\n";
	gp << "f(x) = " << line.first << "*x + " << line.second << "\n"; // Defines a function for gnuplot to evaluate
	gp << "plot '-' with lines lc rgb \"black\" notitle, f(x) with lines title 'Trendline y = " << line.first << "*x + " << line.second << "'\n";
	gp.send1d(boost::make_tuple(lSMA,lPeriod));
}
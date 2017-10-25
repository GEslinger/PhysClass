#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <utility>
#include <vector>
#include <string>
#include "Graph.hpp"
#include "Planet.hpp"
using namespace std;

void graphSystem(vector<vec3D> paths[], int maxP, int base, double scale, string fname){
	Gnuplot gp;

	double sz = scale*149597870700; // Scale in AU

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

	for(int i = 0; i < maxP; i++){
		gp << "'-' with lines lc rgb \"black\" notitle, ";
	}
	gp << "\n";
	
	for(int i = 0; i < maxP; i++){
		vector<double> x;
		vector<double> y;
		vector<double> z;

		for(int j = 0; j < paths[i].size(); j++){
			vec3D v = paths[i][j];
			x.push_back(v.x-paths[base][j].x);
			y.push_back(v.y-paths[base][j].y);
			z.push_back(v.z-paths[base][j].z);
		}

		gp.send1d(boost::make_tuple(x,y,z));
	}
}
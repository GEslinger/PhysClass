#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <vector>
#include <utility>
#include <iostream>
using namespace std;


void plotErrorAndLine(pair<vector<double>,vector<double>> err, pair<double,double> line, string title, string fname){
	Gnuplot gp;

	gp << setprecision(3);
	gp << "set xrange [-10:0]\n";
	gp << "set yrange [-40:0]\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set xlabel \"log(dt)\"\n";
	gp << "set ylabel \"log(error)\"\n";
	gp << "set title \"Global Truncation Error of x'=-x vs dt using " << title << "\"\n"; 
	gp << "set output \"" << fname << "\"\n";

	gp << "f(x) = " << line.first << "*x + " << line.second << "\n";

	gp << "plot '-' with points lc rgb \"black\" notitle, f(x) with lines title 'Trendline y = " << line.first << "*x + " << line.second << "'\n";
	gp.send1d(boost::make_tuple(err.first,err.second));	// Separate the path into components and plot it!
}


void plot3DCurve(vector<double> x, vector<double> y, vector<double> z){
	Gnuplot gp;

	gp << setprecision(3);
	gp << "set xrange [-30:30]\n";
	gp << "set yrange [-30:30]\n";
	gp << "set zrange [0:60]\n";
	gp << "set xlabel \"x\"\n";
	gp << "set ylabel \"y\"\n";
	gp << "set zlabel \"z\"\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set title \"Phase Space of Lorenz System\"\n"; 
	gp << "set output \"3d.png\"\n";
	gp << "set hidden3d\n";
	gp << "set view 80, 60, 1, 1.5\n";
	gp << "splot '-' with lines lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(x,y,z));
}

void plotOneVar(vector<double> v, vector<double> t){
	Gnuplot gp;

	gp << setprecision(3);
	gp << "set xrange [0:" << t.back() << "]\n";
	gp << "set yrange [0:45]\n";
	gp << "set xlabel \"t\"\n";
	gp << "set ylabel \"z\"\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set title \"Z vs t of the Lorenz System\"\n"; 
	gp << "set output \"zvt.png\"\n";
	gp << "plot '-' with lines lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(t,v));
}

void plotBifur(vector<double> rvals, vector<vector<double>> solutions){
	Gnuplot gp;

	vector<double> r;
	vector<double> s;
	for(int i = 0; i < solutions.size(); i++){
		for(int j = 0; j < solutions[i].size(); j++){
			r.push_back(rvals[i]);
			s.push_back(solutions[i][j]);
			// cout << rvals[i] << "\t";
			// cout << solutions[i][j] << endl;
		}
	}

	gp << setprecision(3);
	gp << "set xrange [" << rvals.front() << ":" << rvals.back() << "]\n";
	gp << "set yrange [0:50]\n";
	gp << "set xlabel \"r\"\n";
	gp << "set ylabel \"z\"\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 1920,1080 font \"FreeSerif,12\"\n";
	gp << "set title \"Bifurcation Diagram of the Lorenz System\"\n"; 
	gp << "set output \"bif.png\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(r,s));

}
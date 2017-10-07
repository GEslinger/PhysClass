#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <vector>
using namespace std;

void plotDiffEQ(vector<double> x, vector<double> t, int T, double DT, string title, string fname){	
	Gnuplot gp;	// Instance of gnuplot terminal stream

	gp << setprecision(3);
	gp << "set xrange [0:" << T << "]\n";
	gp << "set yrange [0:1]\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set xlabel \"x\"\n";
	gp << "set ylabel \"t\"\n";
	gp << "set title \"Evaluation of x'=-x using " << title << ", dt=" << DT << "\"\n"; 
	gp << "set output \"" << fname << "\"\n";
	gp << "plot '-' with lines lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(t,x));	// Separate the path into components and plot it!
}
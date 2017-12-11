#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <vector>
#include <string>
using namespace std;

pair<double,double> extrVal(vector<double> v){	// Global extrema of a vector, useful for bounds on graph
	double max = v[0];
	double min = v[0];
	for(double d : v){
		if(d > max) max = d;
		if(d < min) min = d;
	}
	return make_pair(min,max);
}

vector<double> getExtrema(vector<double> v){	// Gets the local extremea of the last 50% of a function (to filter out initial instability)
	vector<double> out;		// Vector to hold extrema
	double back1 = v[0];	// Keep track of the past two points
	double back2 = v[0];
	for(int i = v.size()*0.50; i < v.size(); i++){	// Go from 90% of the end to the end
		if(back1 > back2 && v[i] < back1){	// If there was a peak in the data
			out.push_back(v[i]);	// Add to list
		}
		if(back1 < back2 && v[i] > back1){	// Also look for troughs in data
			out.push_back(v[i]);	// Add to list
		}

		back2 = back1;	// Update the past values
		back1 = v[i];
	}

	return out;	// Output
}

void plot3DCurve(vector<double> x, vector<double> y, vector<double> z){ // 3D Path
	Gnuplot gp;

	auto xt = extrVal(x);
	auto yt = extrVal(y);
	auto zt = extrVal(z);

	gp << setprecision(3);
	gp << "set xrange [" << xt.first*1.1 << ":" << xt.second*1.1 << "]\n";
	gp << "set yrange [" << yt.first*1.1 << ":" << yt.second*1.1 << "]\n";
	gp << "set zrange [" << zt.first*1.1 << ":" << zt.second*1.1 << "]\n";
	gp << "set xlabel \"V_{C_1}\"\n";
	gp << "set ylabel \"V_{C_2}\"\n";
	gp << "set zlabel \"I_{L}\"\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set title \"Phase Space of Chua's Circuit\"\n"; 
	gp << "set output \"3d.png\"\n";
	gp << "set hidden3d\n";
	gp << "set view 60, 60, 1, 1.5\n";
	gp << "splot '-' with lines lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(x,y,z));
}

void plotDifference(vector<double> x1, vector<double> x2, vector<double> t){	// Plots difference between two curves
	Gnuplot gp;

	vector<double> diff;
	for(int i = 0; i < x1.size(); i++){
		diff.push_back(x1[i]-x2[i]);
	}

	gp << setprecision(3);
	gp << "set xrange [" << t.front() << ":" << t.back() << "]\n";
	gp << "set yrange [-10:10]\n";
	gp << "set xlabel \"time (s)\"\n";
	gp << "set ylabel \"V_1 - V_2\"\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set format x \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set title \"Difference in Voltage Between Two Chua's Circuits, I_L Deviation of 1.0E-15\"\n"; 
	gp << "set output \"diff.png\"\n";
	gp << "plot '-' with lines lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(t,diff));
}

void plotBifur(vector<double> indep, vector<vector<double>> dep, string title, string x, string y, string fname){
	Gnuplot gp;

	vector<double> n;
	vector<double> d;
	for(int i = 0; i < indep.size(); i++){	// Turn the vector of vectors into one big vector and matching independant values
		for(int j = 0; j < dep[i].size(); j++){	// More explained in Biff.cpp
			n.push_back(indep[i]);
			d.push_back(dep[i][j]);
		}
	}

	auto ybounds = extrVal(d);

	gp << setprecision(3);
	gp << "set xrange [" << indep.front() << ":" << indep.back() << "]\n";
	gp << "set yrange [" << ybounds.first*1.1 << ":" << ybounds.second*1.1 << "]\n";
	gp << "set xlabel \"" << x << "\"\n";
	gp << "set ylabel \"" << y << "\"\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set title \"" << title << "\"\n"; 
	gp << "set output \"" << fname << "\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(n,d));
}
#include <gnuplot-iostream/gnuplot-iostream.h>
#include <boost/tuple/tuple.hpp>
#include <vector>
#include <utility>
#include <iostream>
using namespace std;

// Graphing functions!
// And input double

void plotErrorAndLine(vector<double> dt, vector<double> error, pair<double,double> line, string title, string fname){ // Plotting the log-log of error, dt, and trendline
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

	gp << "f(x) = " << line.first << "*x + " << line.second << "\n"; // Defines a function for gnuplot to evaluate

	gp << "plot '-' with points lc rgb \"black\" notitle, f(x) with lines title 'Trendline y = " << line.first << "*x + " << line.second << "'\n";
	gp.send1d(boost::make_tuple(dt,error));	// Separate the path into components and plot it!
}


void plot3DCurve(vector<double> x, vector<double> y, vector<double> z, double s, double r, double b){ // 3D Path
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
	gp << "set title \"Phase Space of Lorenz System, {/Symbol s} = " << s << ", {/Symbol r} = " << r << "{/Symbol b} = " << b << "\"\n"; 
	gp << "set output \"3d.png\"\n";
	gp << "set hidden3d\n";
	gp << "set view 60, 60, 1, 1.5\n";
	gp << "splot '-' with lines lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(x,y,z));
}

void plotOneVar(vector<double> v, vector<double> t, double s, double r, double b){	// One variable over time
	Gnuplot gp;

	gp << setprecision(3);
	gp << "set xrange [0:" << t.back() << "]\n";
	gp << "set yrange [0:45]\n";
	gp << "set xlabel \"t\"\n";
	gp << "set ylabel \"z\"\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set title \"Z vs t of the Lorenz System, {/Symbol s} = " << s << ", {/Symbol r} = " << r << "{/Symbol b} = " << b << "\"\n";
	gp << "set output \"zvt.png\"\n";
	gp << "plot '-' with lines lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(t,v));
}

void plotBifur(vector<double> rvals, vector<vector<double>> solutions){
	Gnuplot gp;

	vector<double> r;
	vector<double> s;
	for(int i = 0; i < solutions.size(); i++){	// Turn the vector of vectors into one big vector and matching r values
		for(int j = 0; j < solutions[i].size(); j++){
			r.push_back(rvals[i]);
			s.push_back(solutions[i][j]);
		}
	}

	gp << setprecision(3);
	gp << "set xrange [" << rvals.front() << ":" << rvals.back() << "]\n";
	gp << "set yrange [0:300]\n";
	gp << "set xlabel \"r\"\n";
	gp << "set ylabel \"z\"\n";
	gp << "set format y \"%.1f\"\n";
	gp << "set term png size 720,480 font \"FreeSerif,12\"\n";
	gp << "set title \"Bifurcation Diagram of the Lorenz System\"\n"; 
	gp << "set output \"bif.png\"\n";
	gp << "plot '-' with dots lc rgb \"black\" notitle\n";
	gp.send1d(boost::make_tuple(r,s));

}

double inputDouble(double def, std::string title){		// Function to prompt and input a double, accepts parameters for name and default value
	std::string input;	// Input string
	double output;		// Output double
	bool valid = false;	// Only true if output is initialized properly

	std::cout << "Enter a valid double for " << title;	// Prompt
	std::cout << " (default is " << def << "): ";		// State default

	while(!valid){	// Go until a valid input (or broken by return statement)
		try{		// Try statement, mostly because of std::stod
			std::getline(std::cin, input);	// Feed a line into input
			if(input.empty()) return def;	// If the line is empty (just pressed enter), return the default value

			output = std::stod(input);		// Otherwise, try converting the string to a double- WHICH MAY THROW EXCEPTIONS
			if(output != output) throw "Not a Number!";	// Also, to find if the sillay user entered 'NaN', check for the equality property violation.
			valid = true;					// If everything succeeded, set valid to true.
		} 
		catch(...) {									// Catch all exceptions
			std::cout << "Invalid input, re-enter: ";	// Kindly tell user to not be dumb
		}
	}

	return output;	// Output the double!
}
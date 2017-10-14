#include <vector>
#include <cmath>
#include <utility>
#include <iostream>
using namespace std;

double getAvg(vector<double> v){
	double sum = 0;
	for(int i = 0; i < v.size(); i++){
		sum += v[i];
	}
	return sum/(double)v.size();
}

pair<double,double> getLeastSquares(vector<double> x, vector<double> y){
	double xbar = getAvg(x);
	double ybar = getAvg(y);

	double topSum = 0;
	double botSum = 0;
	for(int i = 0; i < x.size(); i++){;
		topSum += (x[i] - xbar)*(y[i] - ybar);
		botSum += (x[i] - xbar)*(x[i] - xbar);
	}

	double slope = topSum/botSum;
	double intercept = ybar - slope*xbar;

	return make_pair(slope,intercept);
}

pair<vector<double>,vector<double>> toLogLog(vector<double> x, vector<double> y){
	vector<double> outx;
	vector<double> outy;
	for(int i = 0; i < x.size(); i++){
		outx.push_back(log(x[i]));
		outy.push_back(log(y[i]));
	}

	return make_pair(outx,outy);
}

double globalError(vector<double> x, vector<double> t){
	double e = exp(-t.back())-x.back();
	cout << e << endl;
	return fabs(e);
}
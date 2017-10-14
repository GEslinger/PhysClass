#include <vector>
#include <utility>
using namespace std;

void plotErrorAndLine(pair<vector<double>,vector<double>> err, pair<double,double> line, string title, string fname);
pair<double,double> getLeastSquares(vector<double> x, vector<double> y);
pair<vector<double>,vector<double>> toLogLog(vector<double> x, vector<double> y);
double globalError(vector<double> x, vector<double> t);

#include <vector>
#include <utility>
using namespace std;

void plotErrorAndLine(vector<double> dt, vector<double> error, pair<double,double> line, string title, string fname);
pair<double,double> getLeastSquares(vector<double> x, vector<double> y);
vector<double> toLogLog(vector<double> x);
double globalError(vector<double> x, vector<double> t);
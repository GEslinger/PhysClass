#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

const double x0 = 0.5;

int main(){

	cout << setprecision(2);

	for(double r = 2.8; r < 4; r+=0.0001){

		double x = x0;
		for(int i = 0; i < 100; i++){

			x = r*x*(1 - x);
			if(i > 90){
				cout << floor(x*10);
			}
		}

	}

}
#include <iostream>

int main(){
	double x=0,v=10,t=0;
	for(t;t<10;t+=0.005){
		v-=25*x*0.005;
		x+=v*0.005;
		std::cout<<t<<"\t"<<x<<"\t"<<v<<"\n";
	}
}
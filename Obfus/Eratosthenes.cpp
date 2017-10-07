#include <iostream>
#include <bitset>
#define U uint64_t

const U n = 10000;
main(){
	bool* a = new bool[n];
	for(U i=2;i<n;i++){
		for(U j=i*i;j<n;j+=i){
			a[j]=true;
		}
		if((i&(i+1)))a[i]=true;
	}
	for(U i=0;i<n;i++){if(!a[i])std::cout << i << "\n";}
	delete [] a;
}
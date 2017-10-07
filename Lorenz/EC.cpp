#include "Method.h"

void EC::step(){
	t += h;
	x += stepFunc(t,x)*h;
}
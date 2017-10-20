#include "Method.hpp"

// The euler cromer step method

void EC::step(){
	t += h;
	x += stepFunc(t,x)*h;
}
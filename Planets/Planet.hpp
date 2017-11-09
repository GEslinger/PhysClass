#ifndef PLANET_H
#define PLANET_H

// Main header file for the whole project
// Includes implementation of vec3D

struct vec3D{	// vec3D contains x,y,z and overloads operations for addition,subraction, and using scalars
	double x;
	double y;
	double z;

	vec3D& operator+=(vec3D add){ 
		x += add.x;
		y += add.y;
		z += add.z;
		return *this;
	};

	vec3D operator+(vec3D add){
		vec3D out{
			x+add.x,
			y+add.y,
			z+add.z
		};
		return out;
	};

	vec3D& operator-=(vec3D add){ 
		x -= add.x;
		y -= add.y;
		z -= add.z;
		return *this;
	};

	vec3D operator-(vec3D add){
		vec3D out{
			x-add.x,
			y-add.y,
			z-add.z
		};
		return out;
	};

	vec3D operator*(double scalar){
		vec3D out{
			x*scalar,
			y*scalar,
			z*scalar
		};
		return out;
	};

	vec3D operator/(double scalar){
		vec3D out{
			x/scalar,
			y/scalar,
			z/scalar
		};
		return out;
	};
};



class Planet{	// Planet class
public:
	Planet(vec3D pos0, vec3D vel0, double m);	// Constructor using initial position, velocity, and mass
	vec3D getPos() const;	// Accessor methods
	vec3D getVel() const;
	double getMass() const;
	double getT() const;
	void tStep(vec3D acc, double dt);	// Timestep using acceleration and dt
	void update();			// Update positions and velocities
private:
	vec3D nextPos;	// The change in position
	vec3D nextVel;	// Velocity
	vec3D pos;
	vec3D vel;
	double t = 0;	// Planet is created with time = 0
	double mass;
};

#endif
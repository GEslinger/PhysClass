#ifndef PLANET_H
#define PLANET_H

struct vec3D{
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



class Planet{
public:
	Planet(vec3D pos0, vec3D vel0, double m);
	vec3D getPos() const;
	vec3D getVel() const;
	double getMass() const;
	double getT() const;
	void tStep(vec3D acc, double dt);
	void update();
private:
	vec3D nextPos;
	vec3D nextVel;
	vec3D pos;
	vec3D vel;
	double t = 0;
	double mass;
};

#endif
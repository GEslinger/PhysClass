#include <vector>

class Magnet{
public:
	Magnet(int sz, double temp, double boltz, double energy);
	void randomize();
	void setNeg();
	void printSpins();
	void simulate(int MAX_ITERS);
	void setTemp(double temp);
	std::vector<std::vector<bool>> getAllSpins() const;
	double getEnergy() const;
	double getMag() const;

private:
	int getSpin(int i, int j) const;
	double getSpinEnergy(int i, int j) const;
	bool shouldFlip(int i, int j) const;
	int size;
	double T;
	double kb;
	double J;
	std::vector<std::vector<bool>> spins;
};
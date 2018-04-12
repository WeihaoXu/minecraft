#ifndef PERLIN_H
#define PERLIN_H

#include <vector>
#include <cmath>

// Improved perlin noise. Reference: http://mrl.nyu.edu/~perlin/paper445.pdf
class Perlin {

public:
	Perlin();
	~Perlin();

double noise3D(double x, double y, double z);
double noise2D(double x, double z);
double fade(double t);
double lerp(double t, double a, double b);
double grad(int hash, double x, double y, double z);

double grad2D(int hash, double x, double z);





private:
	std::vector<double> p = {1.0, 2.0, 3.0};
};

#endif
#ifndef PERLIN_H
#define PERLIN_H

#include <vector>
#include <cmath>

// Improved perlin noise. Reference: http://mrl.nyu.edu/~perlin/paper445.pdf
class Perlin {

public:
	Perlin();
	~Perlin();

float noise3D(float x, float y, float z);
float fade(float t);
float lerp(float t, float a, float b);
float grad(int hash, float x, float y, float z);





private:
	std::vector<float> p = {1.0, 2.0, 3.0};
};

#endif
#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "perlin.h"


class TerrainGenerator {
public:
	TerrainGenerator(float cube_width, int terrain_x_size, int terrain_z_size);
	~TerrainGenerator();

	
	void sinusoidalTransform();
	void perlinTransform();	// to be impelemted

	float getYcoordinate(glm::vec3 character_position) { return 30.0f; }
	

	std::vector<glm::vec4> cube_vertices;
	std::vector<glm::uvec3> cube_faces;
	std::vector<glm::vec3> cube_positions;
	std::vector<glm::vec4> cube_normals;
	std::vector<glm::vec2> cube_uvs;

private:
	Perlin perlin_;
	void addCube(glm::vec3 grid_coord);
	void generateUnitCube(float cube_width);
	void generatePlane(int x_size_, int z_size_);
	
	
	float cube_width_;
	int x_size_, z_size_;	//the width and height of this terrain (in cube numbers)
	std::vector<std::vector<float>> height_map_;

};


#endif

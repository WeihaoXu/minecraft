#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "perlin.h"


class TerrainGenerator {
public:
	TerrainGenerator(float cube_width, int terrain_x_size, int terrain_z_size, glm::vec3 camera_position);
	~TerrainGenerator();

	
	void sinusoidalTransform();
	void perlinTransform();	// to be impelemted
	

	std::vector<glm::vec4> cube_vertices;
	std::vector<glm::uvec3> cube_faces;
	std::vector<glm::vec3> cube_positions;
	std::vector<glm::vec4> cube_normals;
	std::vector<glm::vec2> cube_uvs;

private:
	Perlin perlin_;
	// void addCube(glm::vec3 grid_coord);
	void generateUnitCube();
	void generateHeightMap();	// compute height map based on camera position
	void generateCubes();	// generate the positions of cubes to render based on height map.
	// void generatePlane();
	
	glm::vec3 gridToWorld(int grid_x, int grid_y, int grid_z);
	
	float cube_width_;
	int x_size_, z_size_;	//the width and height of this terrain (in cube numbers)
	std::vector<std::vector<int>> height_map_;
	int grid_shift_x_, grid_shift_z_;

	float perlin_freq_ = 0.0378; // adjust frequency of perlin noise
	int perlin_height_amp_ = 5;	// adjust amplitude of perlin noise 
	glm::vec3 camera_position_;	

};


#endif

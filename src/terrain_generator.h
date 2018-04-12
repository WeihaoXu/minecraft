#ifndef TERRAIN_GENERATOR_H
#define TERRAIN_GENERATOR_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>

#include "perlin.h"

enum CubeType { WATER = 0, DIRT_GRASS, DIRT, STONE }; 

class TerrainGenerator {
public:
	TerrainGenerator(float cube_width, int terrain_x_size, int terrain_z_size, glm::vec3 camera_position);
	~TerrainGenerator();

	bool updateTerrain(glm::vec3 camera_position);
	bool generateHeightMap();	// compute height map based on camera position
	void generateCubes();	// generate the positions of cubes to render based on height map.
	void deleteCube(glm::vec3 cube_positon, glm::vec3 look_dir);	// delete the top cube in the front direction


	float getHeight(float pos_x, float pos_z);
	glm::vec3 sky_offset = glm::vec3(0.0, 0.0, 0.0);	
	// glm::vec3* getSkyOffsetPointer() {return &sky_offset_;};

	std::vector<glm::vec4> cube_vertices;
	std::vector<glm::uvec3> cube_faces;
	std::vector<glm::vec4> cube_positions;
	// std::vector<glm::vec4> cube_positions;	// <x, y, z, cube_Id>
	std::vector<glm::vec4> cube_normals;
	std::vector<glm::vec2> cube_uvs;

	std::vector<glm::vec4> sky_cube_vertices;
	std::vector<glm::uvec3> sky_cube_faces;
	std::vector<glm::vec2> sky_cube_uvs;

private:
	Perlin perlin_;
	// void addCube(glm::vec3 grid_coord);
	void generateUnitCubes();
	
	
	glm::vec3 gridToWorld(int grid_x, int grid_y, int grid_z);
	glm::ivec3 worldToGrid(glm::vec3 world_pos);

	bool modify_map_contains(int grid_x, int grid_z);
	

	float cube_width_;
	int x_size_, z_size_;	//the width and height of this terrain (in cube numbers)
	std::vector<std::vector<int>> height_map_;
	std::map<int, std::map<int, int>> modify_map_;
	int grid_shift_x_, grid_shift_z_;

	float perlin_freq_ = 0.0323; // adjust frequency of perlin noise
	int perlin_height_amp_ = 5;	// adjust amplitude of perlin noise 
	glm::vec3 camera_position_;
	
};


#endif

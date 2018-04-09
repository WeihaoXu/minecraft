#include <iostream>

#include "terrain_generator.h"
#include "procedure_geometry.h"



TerrainGenerator::TerrainGenerator(float cube_width, int terrain_x_size, int terrain_z_size)
	:cube_width_(cube_width), x_size_(terrain_x_size), z_size_(terrain_z_size)
{
	generateUnitCube(cube_width);
	generatePlane(terrain_x_size, terrain_z_size);
}

TerrainGenerator::~TerrainGenerator()
{

}

// create a cube located at (0, 0, 0) with width = cube_width
void TerrainGenerator::generateUnitCube(float cube_width) {
	generate_cube(cube_width, cube_vertices, cube_normals, cube_faces, cube_uvs);

	// glm::vec3 min(-cube_width / 2.0, -cube_width / 2.0, -cube_width / 2.0);
	// glm::vec3 max(cube_width / 2.0, cube_width / 2.0, cube_width / 2.0);

	// cube_vertices.push_back(glm::vec4(min.x, min.y, min.z, 1.0));
	// cube_vertices.push_back(glm::vec4(max.x, min.y, min.z, 1.0));
	// cube_vertices.push_back(glm::vec4(max.x, max.y, min.z, 1.0));
	// cube_vertices.push_back(glm::vec4(min.x, max.y, min.z, 1.0));

	// cube_vertices.push_back(glm::vec4(min.x, min.y, max.z, 1.0));
	// cube_vertices.push_back(glm::vec4(max.x, min.y, max.z, 1.0));
	// cube_vertices.push_back(glm::vec4(max.x, max.y, max.z, 1.0));
	// cube_vertices.push_back(glm::vec4(min.x, max.y, max.z, 1.0));

	// cube_faces.push_back(glm::uvec3(1, 0, 3));
	// cube_faces.push_back(glm::uvec3(3, 2, 1));
	
	// cube_faces.push_back(glm::uvec3(4, 5, 7));
	// cube_faces.push_back(glm::uvec3(5, 6, 7));

	// cube_faces.push_back(glm::uvec3(5, 4, 0));
	// cube_faces.push_back(glm::uvec3(0, 1, 5));

	// cube_faces.push_back(glm::uvec3(3, 7, 6));
	// cube_faces.push_back(glm::uvec3(6, 2, 3));

	// cube_faces.push_back(glm::uvec3(5, 1, 2));
	// cube_faces.push_back(glm::uvec3(2, 6, 5));
	
	// cube_faces.push_back(glm::uvec3(4, 7, 3));
	// cube_faces.push_back(glm::uvec3(3, 0, 4));
}



void TerrainGenerator::addCube(glm::vec3 grid_coord) {
	cube_positions.push_back(grid_coord * cube_width_);
}

void TerrainGenerator::generatePlane(int x_size_, int z_size_) {
	cube_positions.clear();
	for(int grid_x = 0; grid_x < x_size_; grid_x++) {
		for(int grid_z = 0; grid_z < z_size_; grid_z++) {
			addCube(glm::vec3(grid_x, 0, grid_z));
		}
	}	
}

void TerrainGenerator::sinusoidalTransform() {
	for(int grid_x = 0; grid_x < x_size_; grid_x++) {
		for(int grid_z = 0; grid_z < z_size_; grid_z++) {
			glm::vec3& cube_pos = cube_positions[grid_x * z_size_ + grid_z];
			// cube_pos.y = std::sin(cube_pos.x * cube_pos.x + cube_pos.z * cube_pos.z);
			cube_pos.y = std::sin(cube_pos.x);
		}
	}
}


void TerrainGenerator::perlinTransform() {
	for(int grid_x = 0; grid_x < x_size_; grid_x++) {
		for(int grid_z = 0; grid_z < z_size_; grid_z++) {
			glm::vec3& cube_pos = cube_positions[grid_x * z_size_ + grid_z];
			float freq = 0.005;
			int y_scale = 5;
			cube_pos.y = cube_width_ * std::floor(y_scale * perlin_.noise3D(cube_pos.x * freq, 0.0, cube_pos.z * freq));

			// float x_perlin = 1.0 * grid_x / x_size_;
			// float z_perlin = 1.0 * grid_z / z_size_;
			// cube_pos.y = cube_width_ * perlin_.noise3D(x_perlin, 0.0, z_perlin);
		}
	}
}
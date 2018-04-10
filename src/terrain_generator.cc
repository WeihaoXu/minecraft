#include <iostream>

#include "terrain_generator.h"
#include "procedure_geometry.h"



TerrainGenerator::TerrainGenerator(float cube_width, int terrain_x_size, int terrain_z_size, glm::vec3 camera_position)
	:cube_width_(cube_width), x_size_(terrain_x_size), z_size_(terrain_z_size), camera_position_(camera_position)
{

	generateUnitCubes();
	std::cout << "done generate cube" << std::endl;

	height_map_ = std::vector<std::vector<int>>(x_size_, std::vector<int>(z_size_, 0));
	std::cout << "done height map initialization" << std::endl;
	
	generateHeightMap();	// generate 
	std::cout << "done height map generation" << std::endl;
	
	generateCubes();
	std::cout << "done cube generation" << std::endl;
}

TerrainGenerator::~TerrainGenerator()
{

}

// create a cube located at (0, 0, 0) with width = cube_width
void TerrainGenerator::generateUnitCubes() {
	generate_unit_cube(cube_width_, cube_vertices, cube_normals, cube_faces, cube_uvs);	// in procedure_geometry.h
	generate_unit_cube_inside_out(cube_width_ * x_size_, sky_cube_vertices, sky_cube_faces);
}

bool TerrainGenerator::generateHeightMap()
{
	// height_map_.clear();	// stupid bug!

	float center_x = camera_position_.x, center_z = camera_position_.z;
	int grid_center_x = std::floor(center_x / cube_width_), grid_center_z = std::floor(center_z / cube_width_);
	// std::cout << "grid center: (x0, y0) = " << grid_center_x << ", " << grid_center_z << std::endl;
	int origin_grid_center_x = x_size_ / 2, origin_grid_center_z = z_size_ / 2;
	
	int curr_grid_shift_x = grid_center_x - origin_grid_center_x;
	int curr_grid_shift_z = grid_center_z - origin_grid_center_z;
	


	if(curr_grid_shift_x == grid_shift_x_ && curr_grid_shift_z == grid_shift_z_ && !cube_positions.empty()) {	// no need to update
		return false;
	}
	
	// regenerate terrain
	grid_shift_x_ = curr_grid_shift_x;
	grid_shift_z_ = curr_grid_shift_z;
	for(int grid_x = 0; grid_x < x_size_; grid_x++) {
		for(int grid_z = 0; grid_z < z_size_; grid_z++) {
			int pos_x = grid_x + grid_shift_x_;
			int pos_z = grid_z + grid_shift_z_;
			// int grid_y = std::floor(perlin_height_amp_ * (float) perlin_.noise3D(pos_x * perlin_freq_, 0.0, pos_z * perlin_freq_));
			int grid_y = std::floor(perlin_height_amp_ * (0.3 + perlin_.noise3D(pos_x * perlin_freq_, 0.0, pos_z * perlin_freq_)));  // adjust water area
			if(grid_y < 0) {
				grid_y = -1;
			}
			height_map_[grid_x][grid_z] = grid_y;
			
		}
	}
	return true;
}

void TerrainGenerator::generateCubes() {
	// cube_positions.clear();
	cube_positions = std::vector<glm::vec3>();
	for(int grid_x = 0; grid_x < x_size_; grid_x++) {
		for(int grid_z = 0; grid_z < z_size_; grid_z++) {
			int top_grid_y = height_map_[grid_x][grid_z];
			cube_positions.push_back(gridToWorld(grid_x, top_grid_y, grid_z));

			// fill gaps between neighboring cubes
			int cube_to_fill_num = 0;
			if(grid_x > 0) {
				cube_to_fill_num = std::max(top_grid_y - height_map_[grid_x - 1][grid_z], cube_to_fill_num);
			}
			if(grid_x < x_size_ - 1) {
				cube_to_fill_num = std::max(top_grid_y - height_map_[grid_x + 1][grid_z], cube_to_fill_num);
			}
			if(grid_z > 0) {
				cube_to_fill_num = std::max(top_grid_y - height_map_[grid_x][grid_z - 1], cube_to_fill_num);
			}
			if(grid_z < z_size_ - 1) {
				cube_to_fill_num = std::max(top_grid_y - height_map_[grid_x][grid_z + 1], cube_to_fill_num);
			}
			for(int y_offset = 1; y_offset <= cube_to_fill_num; y_offset++) {
				cube_positions.push_back(gridToWorld(grid_x, top_grid_y - y_offset, grid_z));
			}
			
		}
	}
	// std::cout << "updated center (x, z): " << cube_positions[x_size_ / 2].x << ", " << cube_positions[z_size_ / 2].z << std::endl;
}

glm::vec3 TerrainGenerator::gridToWorld(int grid_x, int grid_y, int grid_z) {
	return glm::vec3(grid_x + grid_shift_x_, grid_y, grid_z + grid_shift_z_) * cube_width_;
} 

// This method will update terrain if necessary based on camera's location.
// Input: camera location.
bool TerrainGenerator::updateTerrain(glm::vec3 camera_position) {
	camera_position_ = camera_position;
	sky_offset = gridToWorld(0, - x_size_ / 2, 0);
	bool hasUpdate = generateHeightMap();
	if(!hasUpdate) return false;
	
	generateCubes();
	return true;
	

}

// Get the terrain height at (pos_x, pos_z) position.
float TerrainGenerator::getHeight(float pos_x, float pos_z) {
	int grid_x = std::floor(pos_x / cube_width_) - grid_shift_x_;
	int grid_z = std::floor(pos_z / cube_width_) - grid_shift_z_;
	return height_map_[grid_x][grid_z] * cube_width_;
}
#include <iostream>

#include "terrain_generator.h"
#include "procedure_geometry.h"


#include "glm/ext.hpp"

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
	generate_unit_cube_inside_out(cube_width_ * x_size_, sky_cube_vertices, sky_cube_faces, sky_cube_uvs);
	generate_unit_cube(cube_width_ * 2, moon_cube_vertices, moon_cube_normals, moon_cube_faces, moon_cube_uvs);
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
	
	// std::cout << "current camera: " << grid_center_x << ", " << grid_center_z
	// 			<< "  modify map value: " << modify_map_contains(grid_center_x, grid_center_z)
	// 			<< std::endl;

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
			int grid_y = std::floor(perlin_height_amp_ * (0.3 + perlin_.noise2D(pos_x * perlin_freq_, pos_z * perlin_freq_)));  // adjust water area
			if(grid_y < 0) {
				grid_y = -1;
			}
			height_map_[grid_x][grid_z] = grid_y;
		}
	}
	return true;
}

void TerrainGenerator::generateCubes() {

	for(auto const& subMap : modify_map_) {
		int modify_grid_x = subMap.first;
		for(auto const& z_v : subMap.second) {
			int modify_grid_z = z_v.first;
			int modify_value = z_v.second;
			height_map_[modify_grid_x - grid_shift_x_][modify_grid_z - grid_shift_z_] += modify_value;
		}
	}

	
	// cube_positions.clear();
	cube_positions.clear();
	for(int grid_x = 0; grid_x < x_size_; grid_x++) {
		for(int grid_z = 0; grid_z < z_size_; grid_z++) {
			int top_grid_y = height_map_[grid_x][grid_z];

			// if(modify_map_contains(grid_x + grid_shift_x_, grid_z + grid_shift_z_)) {
			// 	top_grid_y += modify_map_[grid_x + grid_shift_x_][grid_z + grid_shift_z_];
			// }

			CubeType cube_type;
			if(modify_map_contains(grid_x + grid_shift_x_, grid_z + grid_shift_z_)) {
				cube_type = CubeType::DIRT;	// top cube deleted. so the remain can only be dirt
			}
			else if(top_grid_y < 0) {
				cube_type = CubeType::WATER;
			}
			else if(top_grid_y >= perlin_height_amp_ * 0.5) {
				cube_type = CubeType::STONE;
			}
			else if(top_grid_y % 3 == 0){
				cube_type = CubeType::DIRT;
			}
			else {
				cube_type = CubeType::DIRT_GRASS;
			}

			glm::vec3 world_pos = gridToWorld(grid_x, top_grid_y, grid_z);
			cube_positions.push_back(glm::vec4(world_pos, (int) cube_type));

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
					glm::vec3 world_pos = gridToWorld(grid_x, top_grid_y - y_offset, grid_z);
					cube_positions.push_back(glm::vec4(world_pos, (int) CubeType::DIRT));
			}
			
		}
	}

	for(auto const& subMap : modify_map_) {
		int modify_grid_x = subMap.first;
		for(auto const& z_v : subMap.second) {
			int modify_grid_z = z_v.first;
			int modify_value = z_v.second;
			height_map_[modify_grid_x - grid_shift_x_][modify_grid_z - grid_shift_z_] -= modify_value;
		}
	}

	// std::cout << "updated center (x, z): " << cube_positions[x_size_ / 2].x << ", " << cube_positions[z_size_ / 2].z << std::endl;
}

glm::vec3 TerrainGenerator::gridToWorld(int grid_x, int grid_y, int grid_z) {
	return glm::vec3(grid_x + grid_shift_x_, grid_y, grid_z + grid_shift_z_) * cube_width_;
} 

void TerrainGenerator::deleteCube(glm::vec3 camera_pos, glm::vec3 look_dir) {
	int curr_grid_x = std::floor(camera_pos.x / cube_width_);
	int curr_grid_z = std::floor(camera_pos.z / cube_width_);
	float x_dir = look_dir.x, z_dir = look_dir.z;
 	
	int x_to_delete = curr_grid_x;
	int z_to_delete = curr_grid_z;

	if(x_dir > 0 && fabs(x_dir) > fabs(z_dir)) {	// delete x+1 direction
		x_to_delete = curr_grid_x + 1;
		z_to_delete = curr_grid_z;
	}
	else if(x_dir < 0 && fabs(x_dir) > fabs(z_dir))	{// delete x - 1 direction
		x_to_delete = curr_grid_x - 1;
		z_to_delete = curr_grid_z;
	}
	else if(z_dir > 0 && fabs(z_dir) > fabs(x_dir)) {
		x_to_delete = curr_grid_x;
		z_to_delete = curr_grid_z + 1;
	}
	else if(z_dir < 0 && fabs(z_dir) > fabs(x_dir)) {
		x_to_delete = curr_grid_x;
		z_to_delete = curr_grid_z - 1;
	}
	modify_map_[x_to_delete][z_to_delete] --;
	std::cout << "delete cube at world grid: (" << x_to_delete << ", " << z_to_delete << ") "
				<< "current del cubes: " << modify_map_[x_to_delete][z_to_delete] << "\n";
}

void TerrainGenerator::addCube(glm::vec3 camera_pos, glm::vec3 look_dir) {
	int curr_grid_x = std::floor(camera_pos.x / cube_width_);
	int curr_grid_z = std::floor(camera_pos.z / cube_width_);
	float x_dir = look_dir.x, z_dir = look_dir.z;
 	
	int x_to_delete = curr_grid_x;
	int z_to_delete = curr_grid_z;

	if(x_dir > 0 && fabs(x_dir) > fabs(z_dir)) {	// delete x+1 direction
		x_to_delete = curr_grid_x + 1;
		z_to_delete = curr_grid_z;
	}
	else if(x_dir < 0 && fabs(x_dir) > fabs(z_dir))	{// delete x - 1 direction
		x_to_delete = curr_grid_x - 1;
		z_to_delete = curr_grid_z;
	}
	else if(z_dir > 0 && fabs(z_dir) > fabs(x_dir)) {
		x_to_delete = curr_grid_x;
		z_to_delete = curr_grid_z + 1;
	}
	else if(z_dir < 0 && fabs(z_dir) > fabs(x_dir)) {
		x_to_delete = curr_grid_x;
		z_to_delete = curr_grid_z - 1;
	}
	modify_map_[x_to_delete][z_to_delete] ++;
	std::cout << "add cube at world grid: (" << x_to_delete << ", " << z_to_delete << ") "
				<< "current del cubes: " << modify_map_[x_to_delete][z_to_delete] << "\n";
}


// This method will update terrain if necessary based on camera's location.
// Input: camera location.
bool TerrainGenerator::updateTerrain(glm::vec3 camera_position) {
	camera_position_ = camera_position;
	sky_offset = gridToWorld(0, - x_size_ / 2, 0);
	//std::cout << glm::to_string(sky_offset) << "\n";

	bool hasUpdate = generateHeightMap();
	if(!hasUpdate) return false;
	
	generateCubes();
	return true;
}

void TerrainGenerator::updateMoonOffset(){
	moon_offset = gridToWorld(0, x_size_ / 2, 0);
	moon_offset.y = 0.0f;
	moon_offset.z += z_size_ / 2;
}

// Get the terrain height at (pos_x, pos_z) position.
float TerrainGenerator::getHeight(float pos_x, float pos_z) {
	int grid_x = std::floor(pos_x / cube_width_) - grid_shift_x_;
	int grid_z = std::floor(pos_z / cube_width_) - grid_shift_z_;
	int grid_y = height_map_[grid_x][grid_z];
	if(modify_map_contains(grid_x + grid_shift_x_, grid_z + grid_shift_z_)) {
		grid_y += modify_map_[grid_x + grid_shift_x_][grid_z + grid_shift_z_];
	}
	return grid_y * cube_width_;
}

bool TerrainGenerator::modify_map_contains(int grid_x, int grid_z) {
	if(modify_map_.find(grid_x) == modify_map_.end()
				|| modify_map_[grid_x].find(grid_z) == modify_map_[grid_x].end()) {
		return false;
	}
	return true;
}


#include <iostream>

#include "terrain_generator.h"
#include "procedure_geometry.h"



TerrainGenerator::TerrainGenerator(float cube_width, int terrain_x_size, int terrain_z_size, glm::vec3 camera_position)
	:cube_width_(cube_width), x_size_(terrain_x_size), z_size_(terrain_z_size), camera_position_(camera_position)
{
	// for(float x = -10.0; x < 10.0;) {
	// 	for(float z = -10.0; z < 10.0;) {
	// 		std::cout << "x = " << x << " z = " << z << " noise = " << perlin_.noise3D(x, 0.0, z) << std::endl;
	// 		z += 0.1;
	// 	}
	// 	x += 0.1;
	// }
	generateUnitCube();
	std::cout << "done generate cube" << std::endl;
	// initialize height map
	// for(int grid_x = 0; grid_x < x_size_; grid_x++) {
	// 	std::vector<int> curr;
	// 	height_map_.push_back(curr);
	// 	for(int grid_z = 0; grid_z < z_size_; grid_z++) {
	// 		height_map_[grid_x].push_back(0);
	// 	}
	// }	

	// height_map_.resize(x_size_);
	// for(int grid_x = 0; grid_x < x_size_; grid_x++) {
	// 	height_map_[grid_x].resize(z_size_);
	// }
	height_map_ = std::vector<std::vector<int>>(x_size_, std::vector<int>(z_size_, 0));
	for(int grid_x = 0; grid_x < x_size_; grid_x++) {
		for(int grid_z = 0; grid_z < z_size_; grid_z++) {
			if(height_map_[grid_x][grid_z] != 0) {
				std::cout << "height map init error! value: " << height_map_[grid_x][grid_z] << std::endl;
			}
		}
	}	

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
void TerrainGenerator::generateUnitCube() {
	generate_cube(cube_width_, cube_vertices, cube_normals, cube_faces, cube_uvs);	// in procedure_geometry.h

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

void TerrainGenerator::generateHeightMap()
{
	height_map_.clear();

	float center_x = camera_position_.x, center_z = camera_position_.z;
	int grid_center_x = std::floor(center_x / cube_width_), grid_center_z = std::floor(center_z / cube_width_);
	int origin_grid_center_x = x_size_ / 2, origin_grid_center_z = z_size_ / 2;
	
	grid_shift_x_ = grid_center_x - origin_grid_center_x;
	grid_shift_z_ = grid_center_z - origin_grid_center_z;

	for(int grid_x = 0; grid_x < x_size_; grid_x++) {
		for(int grid_z = 0; grid_z < z_size_; grid_z++) {
			int pos_x = grid_x + grid_shift_x_;
			int pos_z = grid_z + grid_shift_z_;

			// std::cout << pos_x << ", " << pos_z << std::endl;
			int grid_y = std::floor(perlin_height_amp_ * abs((float) perlin_.noise3D(pos_x * perlin_freq_, 0.0, pos_z * perlin_freq_)));
			height_map_[grid_x][grid_z] = grid_y;
			
			// std::cout << "grid_y = " << grid_y << std::endl;
			
			if(grid_x == 0 && grid_z == 63) {
				std::cout << "[0, 63]! grid_y = " << height_map_[grid_x][grid_z] << std::endl;
			}

		}
	}
}

void TerrainGenerator::generateCubes() {
	cube_positions.clear();

	for(int grid_x = 0; grid_x < x_size_; grid_x++) {
		for(int grid_z = 0; grid_z < z_size_; grid_z++) {
			// std::cout << "0" << std::endl;
			// std::cout << "grid_x = " << grid_x << " grid_z = " << grid_z << " x_size = " << x_size_ << " z_size = " << z_size_ << std::endl;
			int top_grid_y = height_map_[grid_x][grid_z];
			cube_positions.push_back(gridToWorld(grid_x, top_grid_y, grid_z));

			if(top_grid_y > 10) {
				std::cout << "large top value = " << top_grid_y << " cur_x = "<< grid_x << "cur_z = " << grid_z << std::endl;
			}

			// std::cout << "1" << std::endl;
			// fill gaps of neighbors
			int cube_to_fill_num = 0;
			if(grid_x > 0) {
				cube_to_fill_num = std::max(top_grid_y - height_map_[grid_x - 1][grid_z], cube_to_fill_num);
				if(cube_to_fill_num > 10) {
					std::cout << "large cube_to_fill_num, top_grid_y = " << top_grid_y << " left: " << height_map_[grid_x - 1][grid_z] << " cur_x = "<< grid_x << "cur_z = " << grid_z << std::endl;
				}
			}
			// std::cout << "2" << std::endl;

			if(grid_x < x_size_ - 1) {
				cube_to_fill_num = std::max(top_grid_y - height_map_[grid_x + 1][grid_z], cube_to_fill_num);
				if(cube_to_fill_num > 10) {
					std::cout << "large cube_to_fill_num, top_grid_y = " << top_grid_y << " right: " << height_map_[grid_x + 1][grid_z] << " cur_x = "<< grid_x << "cur_z = " << grid_z << std::endl;
				}
			}

			// std::cout << "3" << std::endl;

			if(grid_z > 0) {
				cube_to_fill_num = std::max(top_grid_y - height_map_[grid_x][grid_z - 1], cube_to_fill_num);
				if(cube_to_fill_num > 10) {
					std::cout << "large cube_to_fill_num, top_grid_y = " << top_grid_y << " front: " << height_map_[grid_x][grid_z - 1] << " cur_x = "<< grid_x << "cur_z = " << grid_z << std::endl;
				}
			}

			// std::cout << "4" << std::endl;

			if(grid_z < z_size_ - 1) {
				cube_to_fill_num = std::max(top_grid_y - height_map_[grid_x][grid_z + 1], cube_to_fill_num);
				if(cube_to_fill_num > 10) {
					std::cout << "large cube_to_fill_num, top_grid_y = " << top_grid_y << " back: " << height_map_[grid_x][grid_z + 1] << " cur_x = "<< grid_x << "cur_z = " << grid_z << std::endl;
				}
			}

			// std::cout << "5" << std::endl;
			std::cout << "cube_to_fill_num = " << cube_to_fill_num << std::endl;
			// for(int y_offset = 1; y_offset <= cube_to_fill_num; y_offset++) {
			// 	cube_positions.push_back(gridToWorld(grid_x, top_grid_y - y_offset, grid_z));
			// }
			
		}
	}
}

glm::vec3 TerrainGenerator::gridToWorld(int grid_x, int grid_y, int grid_z) {
	return glm::vec3(grid_x + grid_shift_x_, grid_y, grid_z + grid_shift_z_) * cube_width_;
} 



// void TerrainGenerator::addCube(glm::vec3 grid_coord) {
// 	cube_positions.push_back(grid_coord * cube_width_);
// }

// void TerrainGenerator::generatePlane() {
// 	cube_positions.clear();
// 	for(int grid_x = 0; grid_x < x_size_; grid_x++) {
// 		for(int grid_z = 0; grid_z < z_size_; grid_z++) {
// 			addCube(glm::vec3(grid_x, 0, grid_z));
// 		}
// 	}	
// }

// void TerrainGenerator::sinusoidalTransform() {
// 	for(int grid_x = 0; grid_x < x_size_; grid_x++) {
// 		for(int grid_z = 0; grid_z < z_size_; grid_z++) {
// 			glm::vec3& cube_pos = cube_positions[grid_x * z_size_ + grid_z];
// 			// cube_pos.y = std::sin(cube_pos.x * cube_pos.x + cube_pos.z * cube_pos.z);
// 			cube_pos.y = std::sin(cube_pos.x);
// 		}
// 	}
// }


// void TerrainGenerator::perlinTransform() {
// 	for(int grid_x = 0; grid_x < x_size_; grid_x++) {
// 		for(int grid_z = 0; grid_z < z_size_; grid_z++) {
// 			glm::vec3& cube_pos = cube_positions[grid_x * z_size_ + grid_z];
// 			float freq = 0.005;
// 			int y_scale = 5;
// 			cube_pos.y = cube_width_ * std::floor(y_scale * perlin_.noise3D(- cube_pos.x * freq, 0.0, - cube_pos.z * freq));

// 			// float x_perlin = 1.0 * grid_x / x_size_;
// 			// float z_perlin = 1.0 * grid_z / z_size_;
// 			// cube_pos.y = cube_width_ * perlin_.noise3D(x_perlin, 0.0, z_perlin);
// 		}
// 	}
// }
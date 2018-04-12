#ifndef PROCEDURE_GEOMETRY_H
#define PROCEDURE_GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>


void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces);
void create_cube(std::vector<glm::vec4>& cube_vertices, 
					std::vector<glm::uvec3>& cube_faces,
					std::vector<glm::vec4>& cube_normals);

void generate_unit_cube(float cube_width, std::vector<glm::vec4>& cube_vertices,
	std::vector<glm::vec4>& cube_normals, std::vector<glm::uvec3>& cube_faces, std::vector<glm::vec2> &cube_uv);

void create_offsets(std::vector<glm::vec3>& offsets);

void generate_unit_cube_inside_out(float cube_width, std::vector<glm::vec4>& cube_vertices, 
								std::vector<glm::uvec3>& cube_faces);

void generate_unit_cube_inside_out_horizon(float cube_width, std::vector<glm::vec4>& cube_vertices, 
								std::vector<glm::uvec3>& cube_faces);

#endif

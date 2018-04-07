#ifndef PROCEDURE_GEOMETRY_H
#define PROCEDURE_GEOMETRY_H

#include <vector>
#include <glm/glm.hpp>


void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces);
void create_cube(std::vector<glm::vec4>& cube_vertices, 
					std::vector<glm::uvec3>& cube_faces,
					std::vector<glm::vec4>& cube_normals);
void create_offsets(std::vector<glm::vec3>& offsets);
#endif

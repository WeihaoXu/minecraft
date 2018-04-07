#include "procedure_geometry.h"
// #include "bone_geometry.h"
#include "config.h"

void create_floor(std::vector<glm::vec4>& floor_vertices, std::vector<glm::uvec3>& floor_faces)
{
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMax, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMax, kFloorY, kFloorZMin, 1.0f));
	floor_vertices.push_back(glm::vec4(kFloorXMin, kFloorY, kFloorZMin, 1.0f));
	floor_faces.push_back(glm::uvec3(0, 1, 2));
	floor_faces.push_back(glm::uvec3(2, 3, 0));
}

// void create_cube(std::vector<glm::vec4>& cube_vertices, 
// 					std::vector<glm::uvec3>& cube_faces,
// 					std::vector<glm::vec4>& cube_normals) {
// 	glm::vec3 min(0, 0, 0);
// 	glm::vec3 max(2, 2, 2);

// 	cube_vertices.push_back(glm::vec4(min.x, min.y, min.z, 1.0));
// 	cube_vertices.push_back(glm::vec4(max.x, min.y, min.z, 1.0));
// 	cube_vertices.push_back(glm::vec4(max.x, max.y, min.z, 1.0));
// 	cube_vertices.push_back(glm::vec4(min.x, max.y, min.z, 1.0));

// 	cube_vertices.push_back(glm::vec4(min.x, min.y, max.z, 1.0));
// 	cube_vertices.push_back(glm::vec4(max.x, min.y, max.z, 1.0));
// 	cube_vertices.push_back(glm::vec4(max.x, max.y, max.z, 1.0));
// 	cube_vertices.push_back(glm::vec4(min.x, max.y, max.z, 1.0));

// 	cube_faces.push_back(glm::uvec3(1, 0, 3));
// 	cube_faces.push_back(glm::uvec3(3, 2, 1));
	
// 	cube_faces.push_back(glm::uvec3(4, 5, 7));
// 	cube_faces.push_back(glm::uvec3(5, 6, 7));

// 	cube_faces.push_back(glm::uvec3(5, 4, 0));
// 	cube_faces.push_back(glm::uvec3(0, 1, 5));

// 	cube_faces.push_back(glm::uvec3(3, 7, 6));
// 	cube_faces.push_back(glm::uvec3(6, 2, 3));

// 	cube_faces.push_back(glm::uvec3(5, 1, 2));
// 	cube_faces.push_back(glm::uvec3(2, 6, 5));
	
// 	cube_faces.push_back(glm::uvec3(4, 7, 3));
// 	cube_faces.push_back(glm::uvec3(3, 0, 4));
// }



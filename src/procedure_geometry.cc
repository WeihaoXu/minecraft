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


void generate_cube(float cube_width, std::vector<glm::vec4>& cube_vertices,
	std::vector<glm::vec4>& cube_normals, std::vector<glm::uvec3>& cube_faces, std::vector<glm::vec2> &cube_uv) {

	float half_size = cube_width / 2.0;
	// float minU = tex_coords[texID].x;
	// float minV = tex_coords[texID].y;
	// float maxU = minU + 0.19;
	// float maxV = minV + 0.32333;
	float minU = 0.0;
	float minV = 0.0;
	float maxU = 1.0;
	float maxV = 1.0;


	//F1 T1 (wall orthogonal to z-axis @ z = -1)
	cube_vertices.push_back(glm::vec4(-half_size, -half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));
	cube_vertices.push_back(glm::vec4(half_size, -half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));
	cube_vertices.push_back(glm::vec4(-half_size, half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	//F1 T2
	cube_vertices.push_back(glm::vec4(half_size, -half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));
	cube_vertices.push_back(glm::vec4(half_size, half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));
	cube_vertices.push_back(glm::vec4(-half_size, half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	//F2 T1 (wall orthogonal to z-axis @ z = 1)
	cube_vertices.push_back(glm::vec4(-half_size, -half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));
	cube_vertices.push_back(glm::vec4(half_size, -half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));
	cube_vertices.push_back(glm::vec4(-half_size, half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	//F2 T2
	cube_vertices.push_back(glm::vec4(half_size, -half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));
	cube_vertices.push_back(glm::vec4(half_size, half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));
	cube_vertices.push_back(glm::vec4(-half_size, half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	//F3 T1
	cube_vertices.push_back(glm::vec4(-half_size, half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));
	cube_vertices.push_back(glm::vec4(-half_size, half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));
	cube_vertices.push_back(glm::vec4(half_size, half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	//F3 T2
	cube_vertices.push_back(glm::vec4(half_size, half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));
	cube_vertices.push_back(glm::vec4(-half_size, half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));
	cube_vertices.push_back(glm::vec4(half_size, half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	//F4 T1
	cube_vertices.push_back(glm::vec4(-half_size, -half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));
	cube_vertices.push_back(glm::vec4(-half_size, -half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));
	cube_vertices.push_back(glm::vec4(-half_size, half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	//F4 T2
	cube_vertices.push_back(glm::vec4(-half_size, -half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));
	cube_vertices.push_back(glm::vec4(-half_size, half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));
	cube_vertices.push_back(glm::vec4(-half_size, half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	//F5 T1
	cube_vertices.push_back(glm::vec4(half_size, -half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));
	cube_vertices.push_back(glm::vec4(half_size, -half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));
	cube_vertices.push_back(glm::vec4(half_size, half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	//F5 T2
	cube_vertices.push_back(glm::vec4(half_size, -half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));
	cube_vertices.push_back(glm::vec4(half_size, half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));
	cube_vertices.push_back(glm::vec4(half_size, half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	//F6 T1
	cube_vertices.push_back(glm::vec4(-half_size, -half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));
	cube_vertices.push_back(glm::vec4(-half_size, -half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));
	cube_vertices.push_back(glm::vec4(half_size, -half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	//F6 T2
	cube_vertices.push_back(glm::vec4(half_size, -half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));
	cube_vertices.push_back(glm::vec4(-half_size, -half_size, half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));
	cube_vertices.push_back(glm::vec4(half_size, -half_size, -half_size, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	cube_faces.push_back(glm::uvec3(0, 2, 1));	// correct
	cube_faces.push_back(glm::uvec3(5, 4, 3));

	cube_faces.push_back(glm::uvec3(6, 7, 8));	// correct
	cube_faces.push_back(glm::uvec3(9, 10, 11));

	cube_faces.push_back(glm::uvec3(12, 14, 13));	// correct
	cube_faces.push_back(glm::uvec3(15, 17, 16));

	cube_faces.push_back(glm::uvec3(18, 19, 20));
	cube_faces.push_back(glm::uvec3(21, 22, 23));

	cube_faces.push_back(glm::uvec3(24, 26, 25));
	cube_faces.push_back(glm::uvec3(27, 29, 28));
	
	cube_faces.push_back(glm::uvec3(30, 31, 32));
	cube_faces.push_back(glm::uvec3(33, 34, 35));

	
}
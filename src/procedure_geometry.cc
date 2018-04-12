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


// a unit cube between (0, 0, 0) and (width, width, width)
void generate_unit_cube(float cube_width, std::vector<glm::vec4>& cube_vertices,
	std::vector<glm::vec4>& cube_normals, std::vector<glm::uvec3>& cube_faces, std::vector<glm::vec2> &cube_uv) {

	// float half_size = cube_width / 2.0;
	// float offset = half_size;
	float minU = 0.0, minV = 0.0;
	float maxU = 1.0, maxV = 1.0;


	glm::vec3 corner0 = glm::vec3(0, 0, 0) * cube_width;
	glm::vec3 corner1 = glm::vec3(1, 0, 0) * cube_width;
	glm::vec3 corner2 = glm::vec3(1, 0, 1) * cube_width;
	glm::vec3 corner3 = glm::vec3(0, 0, 1) * cube_width;

	glm::vec3 corner4 = glm::vec3(0, 1, 0) * cube_width;
	glm::vec3 corner5 = glm::vec3(1, 1, 0) * cube_width;
	glm::vec3 corner6 = glm::vec3(1, 1, 1) * cube_width;
	glm::vec3 corner7 = glm::vec3(0, 1, 1) * cube_width;


	//buttom face, T1, 0-1-2
	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));

	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));

	cube_vertices.push_back(glm::vec4(corner2, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));

	//buttom face, T2, 2-3-0
	cube_vertices.push_back(glm::vec4(corner2, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));

	cube_vertices.push_back(glm::vec4(corner3, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, -1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));




	// up face, T1, 6-5-4
	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));

	cube_vertices.push_back(glm::vec4(corner5, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));

	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));

	// up face, T2, 4-7-6
	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));

	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));




	// left face, T1, 7-4-0
	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	cube_normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));

	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	cube_normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));

	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	cube_normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));

	// left face, T2, 0-3-7
	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	cube_normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));

	cube_vertices.push_back(glm::vec4(corner3, 1.0f));
	cube_normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	cube_normals.push_back(glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));



	// right face, T1, 1-5-6
	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));

	cube_vertices.push_back(glm::vec4(corner5, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));

	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));

	// right face, T2, 6-2-1
	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));

	cube_vertices.push_back(glm::vec4(corner2, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	cube_normals.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));




	// back face, T1, 4, 5, 1
	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));

	cube_vertices.push_back(glm::vec4(corner5, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));

	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));

	// back face, T2, 1, 0, 4
	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));

	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));




	// front face, T1, 2-6-7
	cube_vertices.push_back(glm::vec4(corner2, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));

	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, minV));

	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));

	// front face, T2, 7-3-2
	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(maxU, maxV));

	cube_vertices.push_back(glm::vec4(corner3, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, maxV));

	cube_vertices.push_back(glm::vec4(corner2, 1.0f));
	cube_normals.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 0.0f));
	cube_uv.push_back(glm::vec2(minU, minV));




	cube_faces.push_back(glm::uvec3(0, 1, 2));
	cube_faces.push_back(glm::uvec3(3, 4, 5));

	cube_faces.push_back(glm::uvec3(6, 7, 8));
	cube_faces.push_back(glm::uvec3(9, 10, 11));

	cube_faces.push_back(glm::uvec3(12, 13, 14));
	cube_faces.push_back(glm::uvec3(15, 16, 17));

	cube_faces.push_back(glm::uvec3(18, 19, 20));
	cube_faces.push_back(glm::uvec3(21, 22, 23));

	cube_faces.push_back(glm::uvec3(24, 25, 26));
	cube_faces.push_back(glm::uvec3(27, 28, 29));
	
	cube_faces.push_back(glm::uvec3(30, 31, 32));
	cube_faces.push_back(glm::uvec3(33, 34, 35));

	
}

// for the sky
void generate_unit_cube_inside_out(float cube_width, std::vector<glm::vec4>& cube_vertices, 
								std::vector<glm::uvec3>& cube_faces) {
	glm::vec3 corner0 = glm::vec3(0, 0, 0) * cube_width;
	glm::vec3 corner1 = glm::vec3(1, 0, 0) * cube_width;
	glm::vec3 corner2 = glm::vec3(1, 0, 1) * cube_width;
	glm::vec3 corner3 = glm::vec3(0, 0, 1) * cube_width;

	glm::vec3 corner4 = glm::vec3(0, 1, 0) * cube_width;
	glm::vec3 corner5 = glm::vec3(1, 1, 0) * cube_width;
	glm::vec3 corner6 = glm::vec3(1, 1, 1) * cube_width;
	glm::vec3 corner7 = glm::vec3(0, 1, 1) * cube_width;


	//buttom face, T1, 0-1-2
	cube_vertices.push_back(glm::vec4(corner2, 1.0f));
	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	
	//buttom face, T2, 2-3-0
	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	cube_vertices.push_back(glm::vec4(corner3, 1.0f));
	cube_vertices.push_back(glm::vec4(corner2, 1.0f));



	// up face, T1, 6-5-4
	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	cube_vertices.push_back(glm::vec4(corner5, 1.0f));
	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	
	// up face, T2, 4-7-6
	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	cube_vertices.push_back(glm::vec4(corner4, 1.0f));



	// left face, T1, 7-4-0
	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	
	// left face, T2, 0-3-7
	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	cube_vertices.push_back(glm::vec4(corner3, 1.0f));
	cube_vertices.push_back(glm::vec4(corner0, 1.0f));




	// right face, T1, 1-5-6
	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	cube_vertices.push_back(glm::vec4(corner5, 1.0f));
	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	
	// right face, T2, 6-2-1
	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	cube_vertices.push_back(glm::vec4(corner2, 1.0f));
	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	


	// back face, T1, 4, 5, 1
	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	cube_vertices.push_back(glm::vec4(corner5, 1.0f));
	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	
	// back face, T2, 1, 0, 4
	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	


	// front face, T1, 2-6-7
	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	cube_vertices.push_back(glm::vec4(corner2, 1.0f));

	// front face, T2, 7-3-2
	cube_vertices.push_back(glm::vec4(corner2, 1.0f));
	cube_vertices.push_back(glm::vec4(corner3, 1.0f));
	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	




	cube_faces.push_back(glm::uvec3(0, 1, 2));	
	cube_faces.push_back(glm::uvec3(3, 4, 5));

	cube_faces.push_back(glm::uvec3(6, 7, 8));
	cube_faces.push_back(glm::uvec3(9, 10, 11));

	cube_faces.push_back(glm::uvec3(12, 13, 14));
	cube_faces.push_back(glm::uvec3(15, 16, 17));

	cube_faces.push_back(glm::uvec3(18, 19, 20));
	cube_faces.push_back(glm::uvec3(21, 22, 23));

	cube_faces.push_back(glm::uvec3(24, 25, 26));
	cube_faces.push_back(glm::uvec3(27, 28, 29));
	
	cube_faces.push_back(glm::uvec3(30, 31, 32));
	cube_faces.push_back(glm::uvec3(33, 34, 35));

}

void generate_unit_cube_inside_out_horizon(float cube_width, std::vector<glm::vec4>& cube_vertices, 
								std::vector<glm::uvec3>& cube_faces) {
	glm::vec3 corner0 = glm::vec3(0, 0, 0) * cube_width;
	glm::vec3 corner1 = glm::vec3(1, 0, 0) * cube_width;
	glm::vec3 corner2 = glm::vec3(1, 0, 1) * cube_width;
	glm::vec3 corner3 = glm::vec3(0, 0, 1) * cube_width;

	glm::vec3 corner4 = glm::vec3(0, 0.55, 0) * cube_width;
	glm::vec3 corner5 = glm::vec3(1, 0.55, 0) * cube_width;
	glm::vec3 corner6 = glm::vec3(1, 0.55, 1) * cube_width;
	glm::vec3 corner7 = glm::vec3(0, 0.55, 1) * cube_width;


	//buttom face, T1, 0-1-2
	cube_vertices.push_back(glm::vec4(corner2, 1.0f));
	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	
	//buttom face, T2, 2-3-0
	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	cube_vertices.push_back(glm::vec4(corner3, 1.0f));
	cube_vertices.push_back(glm::vec4(corner2, 1.0f));



	// up face, T1, 6-5-4
	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	cube_vertices.push_back(glm::vec4(corner5, 1.0f));
	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	
	// up face, T2, 4-7-6
	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	cube_vertices.push_back(glm::vec4(corner4, 1.0f));



	// left face, T1, 7-4-0
	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	
	// left face, T2, 0-3-7
	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	cube_vertices.push_back(glm::vec4(corner3, 1.0f));
	cube_vertices.push_back(glm::vec4(corner0, 1.0f));




	// right face, T1, 1-5-6
	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	cube_vertices.push_back(glm::vec4(corner5, 1.0f));
	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	
	// right face, T2, 6-2-1
	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	cube_vertices.push_back(glm::vec4(corner2, 1.0f));
	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	


	// back face, T1, 4, 5, 1
	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	cube_vertices.push_back(glm::vec4(corner5, 1.0f));
	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	
	// back face, T2, 1, 0, 4
	cube_vertices.push_back(glm::vec4(corner4, 1.0f));
	cube_vertices.push_back(glm::vec4(corner0, 1.0f));
	cube_vertices.push_back(glm::vec4(corner1, 1.0f));
	


	// front face, T1, 2-6-7
	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	cube_vertices.push_back(glm::vec4(corner6, 1.0f));
	cube_vertices.push_back(glm::vec4(corner2, 1.0f));

	// front face, T2, 7-3-2
	cube_vertices.push_back(glm::vec4(corner2, 1.0f));
	cube_vertices.push_back(glm::vec4(corner3, 1.0f));
	cube_vertices.push_back(glm::vec4(corner7, 1.0f));
	




	cube_faces.push_back(glm::uvec3(0, 1, 2));	
	cube_faces.push_back(glm::uvec3(3, 4, 5));

	// cube_faces.push_back(glm::uvec3(6, 7, 8));
	// cube_faces.push_back(glm::uvec3(9, 10, 11));

	cube_faces.push_back(glm::uvec3(12, 13, 14));
	cube_faces.push_back(glm::uvec3(15, 16, 17));

	cube_faces.push_back(glm::uvec3(18, 19, 20));
	cube_faces.push_back(glm::uvec3(21, 22, 23));

	cube_faces.push_back(glm::uvec3(24, 25, 26));
	cube_faces.push_back(glm::uvec3(27, 28, 29));
	
	cube_faces.push_back(glm::uvec3(30, 31, 32));
	cube_faces.push_back(glm::uvec3(33, 34, 35));

}
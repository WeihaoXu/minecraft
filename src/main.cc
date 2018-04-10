#include <GL/glew.h>
#include <dirent.h>

// #include "bone_geometry.h"
#include "procedure_geometry.h"
#include "render_pass.h"
#include "config.h"
#include "gui.h"
#include "terrain_generator.h"
#include "perlin.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <glm/gtx/component_wise.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/io.hpp>
#include <debuggl.h>

int window_width = 800, window_height = 600;
const std::string window_title = "Skinning";

const char* vertex_shader =
#include "shaders/default.vert"
;

const char* geometry_shader =
#include "shaders/default.geom"
;

const char* fragment_shader =
#include "shaders/default.frag"
;

const char* floor_fragment_shader =
#include "shaders/floor.frag"
;

const char* cube_vertex_shader =
#include "shaders/cube.vert"
;

const char* cube_geometry_shader = 
#include "shaders/cube.geom"
;

const char* cube_fragment_shader =
#include "shaders/cube.frag"
;

// FIXME: Add more shaders here.

void ErrorCallback(int error, const char* description) {
	std::cerr << "GLFW Error: " << description << "\n";
}

GLFWwindow* init_glefw()
{
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwSetErrorCallback(ErrorCallback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	auto ret = glfwCreateWindow(window_width, window_height, window_title.data(), nullptr, nullptr);
	CHECK_SUCCESS(ret != nullptr);
	glfwMakeContextCurrent(ret);
	glewExperimental = GL_TRUE;
	CHECK_SUCCESS(glewInit() == GLEW_OK);
	glGetError();  // clear GLEW's error for it
	glfwSwapInterval(1);
	const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
	const GLubyte* version = glGetString(GL_VERSION);    // version as a string
	std::cout << "Renderer: " << renderer << "\n";
	std::cout << "OpenGL version supported:" << version << "\n";

	return ret;
}

int main(int argc, char* argv[])
{

	GLFWwindow *window = init_glefw();
	GUI gui(window);

	std::vector<glm::vec4> floor_vertices;
	std::vector<glm::uvec3> floor_faces;
	create_floor(floor_vertices, floor_faces);

	float cube_width = 10.0;
	int x_size = 200, z_size = 200;

	TerrainGenerator terrain_generator(cube_width, x_size, z_size);
	// terrain_generator.sinusoidalTransform();
	terrain_generator.perlinTransform();
	

	// Perlin perlin;
	// for(float x = 0.0; x < 10.0; x++) {
	// 	for(float z = 0.0; z < 10.0; z++) {	
	// 		std::cout << perlin.noise3D(x / 3.0, 0.0, z / 3.0) << ", ";
	// 	}
	// 	std::cout << std::endl;
	// }


		

	glm::vec4 light_position = glm::vec4(0.0f, 100.0f, 0.0f, 1.0f);
	MatrixPointers mats; // Define MatrixPointers here for lambda to capture
	/*
	 * In the following we are going to define several lambda functions to bind Uniforms.
	 *
	 * Introduction about lambda functions:
	 *      http://en.cppreference.com/w/cpp/language/lambda
	 *      http://www.stroustrup.com/C++11FAQ.html#lambda
	 */
	/*
	 * The following lambda functions are defined to bind uniforms
	 */
	auto matrix_binder = [](int loc, const void* data) {
		glUniformMatrix4fv(loc, 1, GL_FALSE, (const GLfloat*)data);
	};
	auto vector_binder = [](int loc, const void* data) {
		glUniform4fv(loc, 1, (const GLfloat*)data);
	};
	auto vector3_binder = [](int loc, const void* data) {
		glUniform3fv(loc, 1, (const GLfloat*)data);
	};
	auto float_binder = [](int loc, const void* data) {
		glUniform1fv(loc, 1, (const GLfloat*)data);
	};
	auto int_binder = [](int loc, const void* data) {
		glUniform1iv(loc, 1, (const GLint*)data);
	};
	auto cube_positions_binder = [&terrain_generator](int loc, const void* data) {
		glUniform3fv(loc, terrain_generator.cube_positions.size(), (const GLfloat*)data);
	};
	

	/*
	 * The lambda functions below are used to retrieve data
	 */
	auto std_model_data = [&mats]() -> const void* {
		return mats.model;
	}; // This returns point to model matrix
	glm::mat4 floor_model_matrix = glm::mat4(1.0f);
	auto floor_model_data = [&floor_model_matrix]() -> const void* {
		return &floor_model_matrix[0][0];
	}; // This return model matrix for the floor.
	glm::mat4 cube_model_matrix = glm::mat4(1.0f);
	auto cube_model_data = [&cube_model_matrix]() -> const void* {
		return &cube_model_matrix[0][0];
	};
	auto std_view_data = [&mats]() -> const void* {
		return mats.view;
	};
	auto std_camera_data  = [&gui]() -> const void* {
		return &gui.getCamera()[0];
	};
	auto std_proj_data = [&mats]() -> const void* {
		// std::cout << "projection data acquired" << std::endl;
		return mats.projection;
	};
	auto std_light_data = [&light_position]() -> const void* {
		return &light_position[0];
	};
	auto cube_positions_data = [&terrain_generator]() -> const void* {
		// std::cout << "offsets data acquired, size = " << cube_positions.size() << std::endl;
		return terrain_generator.cube_positions.data();

	};

	// FIXME: add more lambdas for data_source if you want to use RenderPass.
	//        Otherwise, do whatever you like here


	
	ShaderUniform std_model = { "model", matrix_binder, std_model_data };
	ShaderUniform floor_model = { "model", matrix_binder, floor_model_data };
	ShaderUniform cube_model = {"model", matrix_binder, cube_model_data};
	ShaderUniform std_view = { "view", matrix_binder, std_view_data };
	ShaderUniform std_camera = { "camera_position", vector3_binder, std_camera_data };
	ShaderUniform std_proj = { "projection", matrix_binder, std_proj_data };
	ShaderUniform std_light = { "light_position", vector_binder, std_light_data };

	// ShaderUniform cube_positions_uniform = {"offsets", cube_positions_binder, cube_positions_data};

	// Floor render pass
	RenderDataInput floor_pass_input;
	floor_pass_input.assign(0, "vertex_position", floor_vertices.data(), floor_vertices.size(), 4, GL_FLOAT);
	floor_pass_input.assignIndex(floor_faces.data(), floor_faces.size(), 3);
	RenderPass floor_pass(-1,
			floor_pass_input,
			{ vertex_shader, geometry_shader, floor_fragment_shader},
			{ floor_model, std_view, std_proj, std_light },
			{ "fragment_color" }
			);
	// FIXME: define more ShaderUniforms for RenderPass if you want to use it.
	//        Otherwise, do whatever you like here


	// Cube render pass
	RenderDataInput cube_pass_input;
	cube_pass_input.assign(0, "vertex_position", terrain_generator.cube_vertices.data(), terrain_generator.cube_vertices.size(), 4, GL_FLOAT);
	cube_pass_input.assign(1, "normal", terrain_generator.cube_normals.data(), terrain_generator.cube_normals.size(), 4, GL_FLOAT);
	cube_pass_input.assign(2, "uv", terrain_generator.cube_uvs.data(), terrain_generator.cube_uvs.size(), 2, GL_FLOAT);
	cube_pass_input.assign(3, "offset", terrain_generator.cube_positions.data(), terrain_generator.cube_positions.size(), 3, GL_FLOAT);
	cube_pass_input.assignIndex(terrain_generator.cube_faces.data(), terrain_generator.cube_faces.size(), 3);

	RenderPass cube_pass(-1,
			cube_pass_input,
			{cube_vertex_shader, cube_geometry_shader, cube_fragment_shader},
			{cube_model, std_view, std_proj, std_light, std_camera},
			{"fragment_color"}
			);

	bool draw_floor = true;
	bool draw_cube = true;

	while (!glfwWindowShouldClose(window)) {
		// Setup some basic window stuff.
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glCullFace(GL_BACK);

		gui.updateMatrices();
		mats = gui.getMatrixPointers();

		glm::vec3 character_pos = gui.getMinecraftCharacterPosition();
		float y_coord = terrain_generator.getYcoordinate(character_pos);
		gui.updateMinecraftCharacterYcoordinate(y_coord);
		if(gui.isCharacterJumping()){
			gui.doJump();
		}

		// // Then draw floor.
		// if (draw_floor) {
		// 	floor_pass.setup();
		// 	// Draw our triangles.
		// 	CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES,
		// 	                              floor_faces.size() * 3,
		// 	                              GL_UNSIGNED_INT, 0));
		// }

		if (draw_cube) {
			cube_pass.setup();
			// CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES,
			//                               terrain_generator.cube_faces.size() * 3,
			//                               GL_UNSIGNED_INT, 0));
			CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES,
					                              terrain_generator.cube_faces.size() * 3,
					                              GL_UNSIGNED_INT, 0,
					                              terrain_generator.cube_positions.size()));
			// std::cout << "offset size: " << cube_positions.size() << std::endl;
		}
		glVertexAttribDivisor(3, 1);

		// Poll and swap.
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

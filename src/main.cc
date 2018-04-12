#include <GL/glew.h>
#include <dirent.h>

// #include "bone_geometry.h"
#include "procedure_geometry.h"
#include "render_pass.h"
#include "config.h"
#include "gui.h"
#include "terrain_generator.h"
#include "perlin.h"
#include "perm_texture.hpp"
#include "tictoc.h"

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

const char* sky_vertex_shader =
#include "shaders/sky.vert"
;

const char* sky_geometry_shader = 
#include "shaders/sky.geom"
;

const char* sky_fragment_shader =
#include "shaders/sky.frag"
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

	float cube_width = 1.0;
	int x_size = 100, z_size = 100;

	TerrainGenerator terrain_generator(cube_width, x_size, z_size, gui.getCamera());
	gui.assignTerrainGenerator(&terrain_generator);
	gui.setPoseDirty();

	unsigned int perm_tex_id;
	initPermTexture(&perm_tex_id);

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
	auto texture0_binder = [](int loc, const void* data) {
		CHECK_GL_ERROR(glUniform1i(loc, 0));
		CHECK_GL_ERROR(glActiveTexture(GL_TEXTURE0 + 0));
		CHECK_GL_ERROR(glBindTexture(GL_TEXTURE_2D, (long)data));
		//std::cerr << " bind texture " << long(data) << std::endl;
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
	auto texture_data = [&perm_tex_id]() -> const void* {
		return (const void*)(intptr_t)perm_tex_id;
	};
	auto sky_offset_data = [&terrain_generator]() -> const void* {
		return (const void*) &terrain_generator.sky_offset[0];
	};
	auto moon_offset_data = [&terrain_generator]() -> const void* {
		return (const void*) &terrain_generator.moon_offset[0];
	};
	float timeDiff = 0.0;
	auto time_passed_data = [&timeDiff]() -> const void* {
		return &timeDiff;
	};
	int is_moon_bool = 1;
	auto is_moon_data = [&is_moon_bool]() -> const void* {
		return &is_moon_bool;
	};
	int is_not_moon_bool = 0;
	auto is_not_moon_data = [&is_not_moon_bool]() -> const void* {
		return &is_not_moon_bool;
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
	ShaderUniform perm_texture = {"perm_texture", texture0_binder, texture_data};
	ShaderUniform sky_offset = {"sky_offset", vector3_binder, sky_offset_data};
	ShaderUniform moon_offset = {"sky_offset", vector3_binder, moon_offset_data};
	ShaderUniform time_pass = { "day_time", float_binder, time_passed_data};
	ShaderUniform is_moon = { "is_moon", int_binder, is_moon_data};
	ShaderUniform is_not_moon = { "is_moon", int_binder, is_not_moon_data};

	// sky render pass
	RenderDataInput sky_pass_input;
	sky_pass_input.assign(0, "vertex_position", terrain_generator.sky_cube_vertices.data(), terrain_generator.sky_cube_vertices.size(), 4, GL_FLOAT);
	sky_pass_input.assign(1, "uv", terrain_generator.sky_cube_uvs.data(), terrain_generator.sky_cube_uvs.size(), 2, GL_FLOAT);
	sky_pass_input.assignIndex(terrain_generator.sky_cube_faces.data(), terrain_generator.sky_cube_faces.size(), 3);
	
	RenderPass sky_pass(-1,
			sky_pass_input,
			{sky_vertex_shader, sky_geometry_shader, sky_fragment_shader},
			{cube_model, std_view, std_proj, perm_texture, sky_offset, time_pass, is_not_moon},
			{"fragment_color"}
			);

	RenderDataInput moon_pass_input;
	moon_pass_input.assign(0, "vertex_position", terrain_generator.moon_cube_vertices.data(), terrain_generator.moon_cube_vertices.size(), 4, GL_FLOAT);
	moon_pass_input.assign(1, "uv", terrain_generator.moon_cube_uvs.data(), terrain_generator.moon_cube_uvs.size(), 2, GL_FLOAT);
	moon_pass_input.assignIndex(terrain_generator.moon_cube_faces.data(), terrain_generator.moon_cube_faces.size(), 3);
	
	RenderPass moon_pass(-1,
			moon_pass_input,
			{sky_vertex_shader, sky_geometry_shader, sky_fragment_shader},
			{cube_model, std_view, std_proj, perm_texture, moon_offset, time_pass, is_moon},
			{"fragment_color"}
			);



	// Cube render pass
	RenderDataInput cube_pass_input;
	cube_pass_input.assign(0, "vertex_position", terrain_generator.cube_vertices.data(), terrain_generator.cube_vertices.size(), 4, GL_FLOAT);
	cube_pass_input.assign(1, "normal", terrain_generator.cube_normals.data(), terrain_generator.cube_normals.size(), 4, GL_FLOAT);
	cube_pass_input.assign(2, "uv", terrain_generator.cube_uvs.data(), terrain_generator.cube_uvs.size(), 2, GL_FLOAT);
	cube_pass_input.assign(3, "offset", terrain_generator.cube_positions.data(), terrain_generator.cube_positions.size(), 4, GL_FLOAT);
	cube_pass_input.assignIndex(terrain_generator.cube_faces.data(), terrain_generator.cube_faces.size(), 3);

	RenderPass cube_pass(-1,
			cube_pass_input,
			{cube_vertex_shader, cube_geometry_shader, cube_fragment_shader},
			{cube_model, std_view, std_proj, std_light, std_camera, perm_texture},
			{"fragment_color"}
			);


	
	
	bool draw_floor = true;
	bool draw_cube = true;
	TicTocTimer timer = tic();

	float totalTimeDifference = 0.0f;
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

	
		if(gui.isPoseDirty()) {
			terrain_generator.updateTerrain(gui.getCamera());
			gui.setMinecraftCharacterPosition();
			// std::cout << "camera position: " << gui.getCamera().x << ", " << gui.getCamera().z << std::endl;
			// std::cout << "first position: " << terrain_generator.cube_positions[0].x << "," << terrain_generator.cube_positions[0].z << std::endl;
			// std::cout << "last position: " << terrain_generator.cube_positions[x_size * z_size - 1].x << "," 
			// 								<< terrain_generator.cube_positions[x_size * z_size - 1].z << std::endl;
			// std::cout << "height of first cube: " << terrain_generator.getHeight(terrain_generator.cube_positions[0].x, terrain_generator.cube_positions[0].z) << std::endl;
			// std::cout << std::endl;
			// std::cout << "grid[0][0] position = " << glm::to_string(terrain_generator.cube_positions[0]) <<"\n";
			// std::cout << "sky position = " << glm::to_string(terrain_generator.sky_offset) <<"\n";
			// for(int i = 0; i < 1000; i++) {
			// 	std::cout << "cube type: " << (int) terrain_generator.cube_positions[i].w << std::endl;
			// }

			gui.clearPose();
		}
		// std::cout << "curent height: " << terrain_generator.getHeight(gui.getCamera().x, gui.getCamera().z) << std::endl;
	
		if(gui.isTerrainModified()) {
			terrain_generator.generateHeightMap();
			terrain_generator.generateCubes();
			gui.clearTerrainModify();
		}

	

		if(gui.isCharacterJumping()){
			if(gui.isWpressed()){
				// std::cout << "MOVING JUMP" << "\n";	
				glm::vec3 tmp_look = gui.getLook();
				tmp_look.y = 0.0f;
				glm::vec3 eye_move = gui.getZoomSpeed() * tmp_look;
				gui.setJumpingCharacterHeight(eye_move);
				gui.setPoseDirty();				
			}
			else if(gui.isSpressed()){
				// std::cout << "MOVING JUMP" << "\n";	
				glm::vec3 tmp_look = gui.getLook();
				tmp_look.y = 0.0f;
				glm::vec3 eye_move = -gui.getZoomSpeed() * tmp_look;
				gui.setJumpingCharacterHeight(eye_move);
				gui.setPoseDirty();			
			}
			else if(gui.isApressed()){
				// std::cout << "MOVING JUMP" << "\n";	
				glm::vec3 eye_move = -gui.getPanSpeed() * gui.getTangent();
				gui.setJumpingCharacterHeight(eye_move);
				gui.setPoseDirty();
				gui.setPoseDirty();			
			}
			else if(gui.isDpressed()){
				// std::cout << "MOVING JUMP" << "\n";	
				glm::vec3 eye_move = gui.getPanSpeed() * gui.getTangent();
				gui.setJumpingCharacterHeight(eye_move);
				gui.setPoseDirty();			
			}
			gui.doJump();
		} else if(gui.isInLoadingMode()){
			if(gui.isWpressed()){
				// std::cout << "MOVING JUMP" << "\n";	
				glm::vec3 tmp_look = gui.getLook();
				tmp_look.y = 0.0f;
				glm::vec3 eye_move = gui.getZoomSpeed() * tmp_look;
				gui.setCharacterHeightToTerrain(eye_move);
				gui.setPoseDirty();	
			}
			else if(gui.isSpressed()){
				// std::cout << "MOVING JUMP" << "\n";	
				glm::vec3 tmp_look = gui.getLook();
				tmp_look.y = 0.0f;
				glm::vec3 eye_move = -gui.getZoomSpeed() * tmp_look;
				gui.setCharacterHeightToTerrain(eye_move);
				gui.setPoseDirty();		
			}
			else if(gui.isApressed()){
				// std::cout << "MOVING JUMP" << "\n";	
				glm::vec3 eye_move = -gui.getPanSpeed() * gui.getTangent();
				gui.setCharacterHeightToTerrain(eye_move);
				gui.setPoseDirty();			
			}
			else if(gui.isDpressed()){
				// std::cout << "MOVING JUMP" << "\n";	
				glm::vec3 eye_move = gui.getPanSpeed() * gui.getTangent();
				gui.setCharacterHeightToTerrain(eye_move);
				gui.setPoseDirty();	
			}
		}


		if(draw_cube) {
			glVertexAttribDivisor(3, 1);
			cube_pass.setup();
			cube_pass.updateVBO(3, 
							terrain_generator.cube_positions.data(), 
							terrain_generator.cube_positions.size());
			CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES,
					                              terrain_generator.cube_faces.size() * 3,
					                              GL_UNSIGNED_INT, 0,
					                              terrain_generator.cube_positions.size()));
		}


		if(draw_floor) {
			if(gui.dayLightSpeedChanged()){
				gui.resetDayLightSpeed();
				std::cout << "It will take: " << 20 * gui.getDayLightSpeed() << " seconds to get through the day-night cycle\n";
				totalTimeDifference = timeDiff * gui.getDayLightSpeed();
			}
			totalTimeDifference += toc(&timer);
			timeDiff = totalTimeDifference/gui.getDayLightSpeed();
			if(timeDiff > 20.0f){
				timeDiff = 0.0f;
				totalTimeDifference = 0.0f;
			}
			if(timeDiff > 11.5f && timeDiff < 18.5f){
				terrain_generator.updateMoonOffset();
				if(timeDiff < 15.0f){
					terrain_generator.moon_offset.y = 3.0f + (timeDiff - 11.5f) * (timeDiff - 11.5f) * 25.0f/3.5f;
					if(terrain_generator.moon_offset.y > 43.f){
						terrain_generator.moon_offset.y = 43.f;
					}

					// std::cout << "increase" << terrain_generator.moon_offset.y << "\n";
				} else {
					if(timeDiff > 16.1f){
						terrain_generator.moon_offset.y = 3.0f + (timeDiff - 16.1f) * (timeDiff - 16.1f) * 25.0f/3.5f;
						if(terrain_generator.moon_offset.y > 43.f){
							terrain_generator.moon_offset.y = 43.f;
							// std::cout << "decrease" << timeDiff << "\n";
						}
						terrain_generator.moon_offset.y = 43.f - terrain_generator.moon_offset.y;

						// std::cout << "decrease" << terrain_generator.moon_offset.y << "\n";
					}
					else {
						terrain_generator.moon_offset.y = 43.f;
					}
				}
				
				terrain_generator.moon_offset.x += (timeDiff - 11.5f) * terrain_generator.getXSize()/7.0f;
				
				moon_pass.setup();
				CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES,
			                              terrain_generator.moon_cube_faces.size() * 3,
			                              GL_UNSIGNED_INT, 0));
			}

			sky_pass.setup();
			CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES,
			                              terrain_generator.sky_cube_faces.size() * 3,
			                              GL_UNSIGNED_INT, 0));
		}

			
			// std::cout << "offset size: " << cube_positions.size() << std::endl;
		

		// Poll and swap.
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

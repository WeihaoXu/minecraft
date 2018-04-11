#ifndef SKINNING_GUI_H
#define SKINNING_GUI_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "terrain_generator.h"
#include "minecraft_character.h"

struct Mesh;

/*
 * Hint: call glUniformMatrix4fv on thest pointers
 */
struct MatrixPointers {
	const float *projection, *model, *view;
};

class GUI {
public:
	GUI(GLFWwindow*);
	~GUI();

	void keyCallback(int key, int scancode, int action, int mods);
	void mousePosCallback(double mouse_x, double mouse_y);
	void mouseButtonCallback(int button, int action, int mods);
	void updateMatrices();
	MatrixPointers getMatrixPointers() const;

	static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	glm::vec3 getCenter() const { return center_; }
	const glm::vec3& getCamera() const { return eye_; }
	bool isPoseDirty() const { return pose_changed_; }
	void clearPose() { pose_changed_ = false; }
	void setPoseDirty() { pose_changed_ = true; }
	const float* getLightPositionPtr() const { return &light_position_[0]; }

	void assignTerrainGenerator(TerrainGenerator* terrain_generator);


	bool isCharacterJumping() const {return minecraft_character->isJumping(); }
	void doJump();
	glm::vec3 getMinecraftCharacterPosition() const{ return minecraft_character->getCharacterPosition(); }
	void updateMinecraftCharacterYcoordinate(float y_coord) {minecraft_character->setCharacterPosition(glm::vec3(eye_.x, y_coord, eye_.z));}
private:
	GLFWwindow* window_;
	TerrainGenerator* terrain_generator_;
	MinecraftCharacter* minecraft_character;

	int window_width_, window_height_;

	bool drag_state_ = false;
	bool fps_mode_ = false;
	bool pose_changed_ = true;
	bool transparent_ = false;
	// int current_bone_ = -1;
	bool loading_mode_ = false;

	int current_button_ = -1;
	float roll_speed_ = M_PI / 64.0f * 5;
	float last_x_ = 0.0f, last_y_ = 0.0f, current_x_ = 0.0f, current_y_ = 0.0f;
	float camera_distance_ = 30.0;
	float pan_speed_ = 0.1f * 5;
	float rotation_speed_ = 0.02f;
	float zoom_speed_ = 0.1f * 5;
	float aspect_;

	// glm::vec3 eye_ = glm::vec3(0.0f, 0.1f, camera_distance_);
	glm::vec3 eye_ = glm::vec3(0.0f, 10.0f, camera_distance_);
	glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 tangent_ = glm::cross(look_, up_);
	glm::vec3 center_ = eye_ - camera_distance_ * look_;
	glm::mat3 orientation_ = glm::mat3(tangent_, up_, look_);
	glm::vec4 light_position_;

	glm::mat4 view_matrix_ = glm::lookAt(eye_, center_, up_);
	glm::mat4 projection_matrix_;
	glm::mat4 model_matrix_ = glm::mat4(1.0f);

	bool captureWASDUPDOWN(int key, int action);
	bool setCharacterHeightToTerrain();

};

#endif

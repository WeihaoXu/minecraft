#include "gui.h"
#include "config.h"
#include <jpegio.h>

#include <iostream>
#include <debuggl.h>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "tictoc.h"
#include "glm/ext.hpp"


GUI::GUI(GLFWwindow* window)
	:window_(window)
{
	glfwSetWindowUserPointer(window_, this);
	glfwSetKeyCallback(window_, KeyCallback);
	glfwSetCursorPosCallback(window_, MousePosCallback);
	glfwSetMouseButtonCallback(window_, MouseButtonCallback);

	glfwGetWindowSize(window_, &window_width_, &window_height_);
	float aspect_ = static_cast<float>(window_width_) / window_height_;
	projection_matrix_ = glm::perspective((float)(kFov * (M_PI / 180.0f)), aspect_, kNear, kFar);
	minecraft_character = new MinecraftCharacter();
}

GUI::~GUI()
{
}

void GUI::assignTerrainGenerator(TerrainGenerator* terrain_generator)
{
	terrain_generator_ = terrain_generator;
}

void GUI::keyCallback(int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window_, GL_TRUE);
		return ;
	} 
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		if(loading_mode_){
			//JUMP CHARACTER
			minecraft_character->setJumping(terrain_generator_->getHeight(eye_.x, eye_.z));
		}
	}
	if (captureWASDUPDOWN(key, action))
		return ;
	if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
		float roll_speed;
		if (key == GLFW_KEY_RIGHT)
			roll_speed = -roll_speed_;
		else
			roll_speed = roll_speed_;
		// FIXME: actually roll the bone here
	} else if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_RELEASE) {

	} else if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_RELEASE) {
	
	} else if (key == GLFW_KEY_T && action != GLFW_RELEASE) {
		transparent_ = !transparent_;
	} else if ((key == GLFW_KEY_F && (mods & GLFW_MOD_CONTROL)) && action == GLFW_RELEASE) {
		loading_mode_ = !loading_mode_;
		if(loading_mode_){
			minecraft_character->timer = tic();
			setInitCharacterHeight();
			setPoseDirty();
			// eye_ = minecraft_character->getCharacterPosition();
			// setCharacterHeightToTerrain(glm::vec3(0,0,0));
		}
	} else if(key == GLFW_KEY_R && action == GLFW_PRESS) {
		if(loading_mode_) {
			terrain_generator_->deleteCube(eye_, look_);
			terrain_modified = true;
		}
	}
}

void GUI::mousePosCallback(double mouse_x, double mouse_y)
{
	last_x_ = current_x_; // window_width_/2; //
	last_y_ = current_y_; // window_height_/2; //
	current_x_ = mouse_x;
	current_y_ = window_height_ - mouse_y;
	float delta_x = current_x_ - last_x_;
	float delta_y = current_y_ - last_y_;
	if (sqrt(delta_x * delta_x + delta_y * delta_y) < 1e-15)
		return;
	glm::vec3 mouse_direction = glm::normalize(glm::vec3(delta_x, delta_y, 0.0f));
	glm::vec2 mouse_start = glm::vec2(last_x_, last_y_);
	glm::vec2 mouse_end = glm::vec2(current_x_, current_y_);
	glm::uvec4 viewport = glm::uvec4(0, 0, window_width_, window_height_);

	bool drag_camera = drag_state_ && current_button_ == GLFW_MOUSE_BUTTON_RIGHT;


	if (drag_camera) {
		glm::vec3 axis = glm::normalize(
				orientation_ *
				glm::vec3(mouse_direction.y, -mouse_direction.x, 0.0f)
				);
		orientation_ =
			glm::mat3(glm::rotate(rotation_speed_, axis) * glm::mat4(orientation_));
		tangent_ = glm::column(orientation_, 0);
		up_ = glm::column(orientation_, 1);
		look_ = glm::column(orientation_, 2);
	} 

	
}

void GUI::mouseButtonCallback(int button, int action, int mods)
{
	drag_state_ = (action == GLFW_PRESS);
	current_button_ = button;
}

void GUI::updateMatrices()
{
	// Compute our view, and projection matrices.
	if(isInLoadingMode()){
		up_ = glm::vec3(0.0f, 1.0f, 0.0f);
		center_ = eye_ + camera_distance_ * look_;
	}
	else 
	{
		center_ = eye_ + camera_distance_ * look_;
	}

	view_matrix_ = glm::lookAt(eye_, center_, up_);
	light_position_ = glm::vec4(eye_, 1.0f);

	aspect_ = static_cast<float>(window_width_) / window_height_;
	projection_matrix_ =
		glm::perspective((float)(kFov * (M_PI / 180.0f)), aspect_, kNear, kFar);
	model_matrix_ = glm::mat4(1.0f);
}

MatrixPointers GUI::getMatrixPointers() const
{
	MatrixPointers ret;
	ret.projection = &projection_matrix_[0][0];
	ret.model= &model_matrix_[0][0];
	ret.view = &view_matrix_[0][0];
	return ret;
}


void GUI::doJump(){
	minecraft_character->jump();
	eye_.y = minecraft_character->getCharacterPosition().y;
}

bool GUI::captureWASDUPDOWN(int key, int action)
{
	if (key == GLFW_KEY_W) {
		if(action == GLFW_PRESS){
			w_pressed_ = true;
		}
		if(action == GLFW_RELEASE){
			w_pressed_ = false;
		}
		
		if(!isInLoadingMode()){
			eye_ += zoom_speed_ * look_;
		}
		setPoseDirty();
		
		return true;
	} else if (key == GLFW_KEY_S) {
		if(action == GLFW_PRESS){
			s_pressed_ = true;
		}
		if(action == GLFW_RELEASE){
			s_pressed_ = false;
		}

		if(!isInLoadingMode()){
			eye_ -= zoom_speed_ * look_;
		}
		
		setPoseDirty();
		
		return true;
	} else if (key == GLFW_KEY_A) {
		if(action == GLFW_PRESS){
			a_pressed_ = true;
		}
		if(action == GLFW_RELEASE){
			a_pressed_ = false;
		}

		if(!isInLoadingMode()){
			eye_ -= pan_speed_ * tangent_;
		}

		setPoseDirty();
		
		return true;
	} else if (key == GLFW_KEY_D) {
		if(action == GLFW_PRESS){
			d_pressed_ = true;
		}
		if(action == GLFW_RELEASE){
			d_pressed_ = false;
		}
		if(!isInLoadingMode()){
			eye_ += pan_speed_ * tangent_;
		}
		
		setPoseDirty();
		
		return true;
	} else if (key == GLFW_KEY_DOWN) {
		if(isInLoadingMode()){
			return false;
		}
		else {
			eye_ -= pan_speed_ * up_;
		}
		
		setPoseDirty();
		
		return true;
	} else if (key == GLFW_KEY_UP) {

		if(isInLoadingMode()){
			return false;
		}
		else {
			eye_ += pan_speed_ * up_;
		}
		
		setPoseDirty();
		
		return true;
	}

	return false;
}

void GUI::setJumpingCharacterHeight(glm::vec3 eye_move){
	float next_y_coord = terrain_generator_->getHeight(eye_.x + eye_move.x, eye_.z + eye_move.z);
	eye_ += eye_move;
	minecraft_character->position_y_ = (1.75f + next_y_coord);
}

void GUI::setInitCharacterHeight(){
	float next_y_coord = terrain_generator_->getHeight(eye_.x, eye_.z);
	eye_.y = (1.75f + next_y_coord);
}

bool GUI::setCharacterHeightToTerrain(glm::vec3 eye_move){
	float current_y_coord = eye_.y - 1.75f;
	float next_y_coord = terrain_generator_->getHeight(eye_.x + eye_move.x, eye_.z + eye_move.z);

	float max = next_y_coord;
	// std::cout << "Current eye" << glm::to_string(eye_) << "\n";
	// std::cout << "Direction I want to move" << glm::to_string(eye_move) << "\n";
 	if(eye_move.x < 0.0){
		max = fmax(max, terrain_generator_->getHeight(eye_.x + eye_move.x - 0.4, eye_.z + eye_move.z));
	}
	else if (eye_move.x > 0.0) {
		max = fmax(max, terrain_generator_->getHeight(eye_.x + eye_move.x + 0.4, eye_.z + eye_move.z));
	}
	if(eye_move.z < 0.0){
		max = fmax(max, terrain_generator_->getHeight(eye_.x + eye_move.x, eye_.z + eye_move.z - 0.4));	
	}
	else if (eye_move.z > 0.0){
		max = fmax(max, terrain_generator_->getHeight(eye_.x + eye_move.x, eye_.z + eye_move.z + 0.4));	
	}
	

	// std::cout << "CURRENT y position: " << current_y_coord << "\n";
	// std::cout << "NEXT y position: " << next_y_coord << "\n";
	// std::cout << "radius y position: " << max << "\n";

	if(next_y_coord > current_y_coord){
		return false;
	} else if (next_y_coord < 0){
		return false;
	} 
	else if (max > next_y_coord && max > current_y_coord){
		return false;
	}
	else {
		eye_ += eye_move;
		eye_.y = 1.75f + next_y_coord;
		return true;
	}
}

void GUI::deleteCube() {
	terrain_generator_->deleteCube(eye_, look_);
}

// Delegrate to the actual GUI object.
void GUI::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->keyCallback(key, scancode, action, mods);
}

void GUI::MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mousePosCallback(mouse_x, mouse_y);
}

void GUI::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	GUI* gui = (GUI*)glfwGetWindowUserPointer(window);
	gui->mouseButtonCallback(button, action, mods);
}

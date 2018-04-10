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
	} else if (key == GLFW_KEY_C && action != GLFW_RELEASE) {
		fps_mode_ = !fps_mode_;
		std::cout << "toggle fps_mode_, current: " << fps_mode_ << std::endl;
	} else if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_RELEASE) {

	} else if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_RELEASE) {
	
	} else if (key == GLFW_KEY_T && action != GLFW_RELEASE) {
		transparent_ = !transparent_;
	} else if ((key == GLFW_KEY_F && (mods & GLFW_MOD_CONTROL)) && action == GLFW_RELEASE) {
		loading_mode_ = !loading_mode_;
		if(loading_mode_){
			minecraft_character->timer = tic();
			eye_ = minecraft_character->getCharacterPosition();
			setCharacterHeightToTerrain(glm::vec3(0.0f, 0.0f, 0.0f));
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
		//tangent_ = glm::column(orientation_, 0);
		//up_ = glm::column(orientation_, 1);
		look_ = glm::column(orientation_, 2);
		//up_ = glm::cross(tangent_, look_);
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
	if(loading_mode_){
		up_ = glm::vec3(0.0f, 1.0f, 0.0f);
		center_ = eye_ + camera_distance_ * look_;
	}
	else if (fps_mode_){
		center_ = eye_ + camera_distance_ * look_;
	}
	else{
		eye_ = center_ - camera_distance_ * look_;
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
	eye_ = minecraft_character->getCharacterPosition();
}

bool GUI::captureWASDUPDOWN(int key, int action)
{
	if (key == GLFW_KEY_W) {
		//std::cout << glm::to_string(eye_) << "\n";
		if(loading_mode_){
			//MOVE CHARACTER FORWARD
			glm::vec3 tmp_look = glm::vec3(look_.x, 0.0f, look_.z);
			glm::vec3 eye_movement = zoom_speed_ * tmp_look;;
			eye_ += zoom_speed_ * tmp_look;
			setCharacterHeightToTerrain(eye_movement);
		} else if (fps_mode_){
			eye_ += zoom_speed_ * look_;
		} else{
			camera_distance_ -= zoom_speed_;
		}
		
		pose_changed_ = true;
		
		return true;
	} else if (key == GLFW_KEY_S) {
		if(loading_mode_){
			//MOVE CHARACTER BACKWARD
			glm::vec3 tmp_look = glm::vec3(look_.x, 0.0f, look_.z);
			glm::vec3 eye_movement = -zoom_speed_ * tmp_look;;
			eye_ -= zoom_speed_ * tmp_look;
			setCharacterHeightToTerrain(eye_movement);
		} else if (fps_mode_){
			eye_ -= zoom_speed_ * look_;
		} else {
			camera_distance_ += zoom_speed_;
		}
		
		pose_changed_ = true;
		
		return true;
	} else if (key == GLFW_KEY_A) {
		if(loading_mode_){
			//STRAFE CHARACTER
			glm::vec3 eye_movement = -pan_speed_ * tangent_;
			eye_ -= pan_speed_ * tangent_;
			setCharacterHeightToTerrain(eye_movement);
		} else if (fps_mode_){
			eye_ -= pan_speed_ * tangent_;
		} else{
			center_ -= pan_speed_ * tangent_;
		}

		pose_changed_ = true;
		
		return true;
	} else if (key == GLFW_KEY_D) {
		if(loading_mode_){
			//STRAFE CHARACTER
			glm::vec3 eye_movement = pan_speed_ * tangent_;
			eye_ += pan_speed_ * tangent_;
			setCharacterHeightToTerrain(eye_movement);
		} else if (fps_mode_){
			eye_ += pan_speed_ * tangent_;
		} else {
			center_ += pan_speed_ * tangent_;
		}
		
		pose_changed_ = true;
		
		return true;
	} else if (key == GLFW_KEY_DOWN) {
		if(loading_mode_){
			return false;
		} if (fps_mode_)
			eye_ -= pan_speed_ * up_;
		else
			center_ -= pan_speed_ * up_;
		
		pose_changed_ = true;
		
		return true;
	} else if (key == GLFW_KEY_UP) {
		if(loading_mode_){
			return false;
		} if (fps_mode_)
			eye_ += pan_speed_ * up_;
		else
			center_ += pan_speed_ * up_;
		
		pose_changed_ = true;
		
		return true;
	}

	return false;
}


void GUI::setCharacterHeightToTerrain(glm::vec3 eye_movement){
	float y_coord = terrain_generator_->getHeight(eye_.x, eye_.z);
	
	float max = y_coord;
	max = fmax(max, terrain_generator_->getHeight(eye_.x + 0.5, eye_.z));
	max = fmax(max, terrain_generator_->getHeight(eye_.x - 0.5, eye_.z));
	max = fmax(max, terrain_generator_->getHeight(eye_.x, eye_.z + 0.5));
	max = fmax(max, terrain_generator_->getHeight(eye_.x, eye_.z - 0.5));
	
	if(! minecraft_character->isJumping()){	
		if(max > y_coord && eye_.y < (1.75f + max)){
			minecraft_character->setJumping(max);
		}
		eye_.y = 1.75f + max;
	} else {
		minecraft_character->position_y_ = 1.75f + max;
	}
	minecraft_character->setCharacterPosition(eye_);
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

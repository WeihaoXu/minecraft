#ifndef MINECRAFT_CHARACTER_H
#define MINECRAFT_CHARACTER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "tictoc.h"
#include <iostream>
#include <iomanip>

class MinecraftCharacter {
public:
	MinecraftCharacter(){}
	~MinecraftCharacter(){}

	TicTocTimer timer;
	bool isJumping() const {return is_jumping_;}
	void setJumping() {
		is_jumping_ = true;
		position_y_ = position_.y;
		// std::cout << position_y_ << "\n";
		timeDiff_ = toc(&timer);
		move_up = true;
	}
	void stopJumping() {is_jumping_ = false;}
	bool jump() {
		timeDiff_ = toc(&timer);
		velocity_ += gravity_ * timeDiff_;

		if(move_up){

			if(fabs(velocity_) > 2.0f){
				velocity_ = 0.0f;
				move_up = false;
			} else {
				position_.y -= velocity_;
			}

		} else {
			position_.y += velocity_;
			if(position_y_ > position_.y){
				position_.y = position_y_;
				velocity_ = 0.0f;
				is_jumping_ = false;
				return false;
			}
		}
		// std::cout << move_up << "\n";
		// std::cout << std::fixed << std::setprecision(3) << "Time Difference: " << timeDiff_ << "\n";
		// std::cout << "Current Velocity: " << velocity_ << "\n";
		// std::cout << "Y Position: " << position_.y << "\n";
		// std::cout <<  "OG Y position: " << position_y_ << "\n";

		return true;
	}

	void setCharacterPosition(glm::vec3 character_eye) { position_ = character_eye;}
	glm::vec3 getCharacterPosition() const {return position_;}
	float position_y_ = 0.0f;
private:
	GLFWwindow* window_;

	glm::mat4 cylinder_ = glm::scale(glm::mat4(1.0), glm::vec3(0.5, 1.75, 0.5));
	glm::vec3 position_ =  glm::vec3(0.0f, 30.0f, 100.0f);

	
	float velocity_ = 0.0f;
	float gravity_ = -9.81f;
	float timeDiff_ = 0.0f;
	bool move_up = true;

	bool is_jumping_ = false;
};

#endif
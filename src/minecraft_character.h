#ifndef MINECRAFT_CHARACTER_H
#define MINECRAFT_CHARACTER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include "tictoc.h"

class MinecraftCharacter {
public:
	MinecraftCharacter(){}
	~MinecraftCharacter(){}

	TicTocTimer timer;
	bool isJumping() const {return is_jumping_;}
	void stopJumping() {is_jumping_ = false;}
	void setJumping(float y_coord);
	bool jump();

	void setCharacterPosition(glm::vec3 character_eye) { position_ = character_eye;}
	glm::vec3 getCharacterPosition() const {return position_;}
	float position_y_ = 0.0f;
private:
	
	glm::vec3 position_ =  glm::vec3(0.0f, 30.0f, 100.0f);
	float velocity_ = 0.0f;
	float gravity_ = -9.81f;
	float timeDiff_ = 0.0f;
	float jump_height = 0.0f;
	bool move_up = true;

	bool is_jumping_ = false;
};

#endif
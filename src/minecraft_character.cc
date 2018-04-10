#include <iostream>
#include <iomanip>

#include "minecraft_character.h"

void MinecraftCharacter::setJumping(float y_coord) {
	is_jumping_ = true;
	position_y_ = 1.75f + y_coord; //position_.y;
	// std::cout << position_y_ << "\n";
	if(jump_height < 2.5f){
		jump_height += 1.0f;
		timeDiff_ = toc(&timer);
		move_up = true;
	}
}

bool MinecraftCharacter::jump() {
	timeDiff_ = toc(&timer);
	velocity_ += gravity_ * timeDiff_;

	if(move_up){

		if(fabs(velocity_) > jump_height){
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
			jump_height = 0.0f;
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
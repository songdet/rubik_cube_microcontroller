/*
 * arm_controls.cpp
 *
 * Created: 4/14/2022 3:43:35 PM
 *  Author: Songdet Nillasithanukroh
 */

#ifndef ARM_CONTROLS_CPP_
#define ARM_CONTROLS_CPP_

#include <stddef.h>
#include <avr/io.h>
#include "arm.h"
#include "servo_arm.h"
#include "arm_controls.h"

ArmControls::ArmControls(Arm left_arm, Arm right_arm, ServoArm top_arm, ServoArm bottom_arm) : left_arm(left_arm), right_arm(right_arm), top_arm(top_arm), bottom_arm(bottom_arm) {
	
}

void ArmControls::spin(bool is_extend, uint8_t arm_mask) {
	
	// Check which arms need to be spun
	bool left_arm_selected = (arm_mask & ARM_CONTROLS_LEFT) == ARM_CONTROLS_LEFT;
	bool right_arm_selected = (arm_mask & ARM_CONTROLS_RIGHT) == ARM_CONTROLS_RIGHT;
	bool top_arm_selected = (arm_mask & ARM_CONTROLS_TOP) == ARM_CONTROLS_TOP;
	bool bottom_arm_selected = (arm_mask & ARM_CONTROLS_BOTTOM) == ARM_CONTROLS_BOTTOM;
	
	// Enable the motor first. Pull low to enable motor
	if (left_arm_selected || right_arm_selected) {
		PORTD &= ~0b00000100;	
	}

	// Control the top arm
	if (top_arm_selected) {
		if (is_extend) {
			this->top_arm.extend();
		} else {
			this->top_arm.retract();
		}
	}
	
	// Control the bottom arm
	if (bottom_arm_selected) {
		if (is_extend) {
			this->bottom_arm.extend();
		} else {
			this->bottom_arm.retract();
		}
	}
	
	// Set direction based on bit mask
	if (left_arm_selected) { 
		left_arm.set_direction(is_extend);
	}
	if (right_arm_selected) {
		right_arm.set_direction(is_extend);
	}
	
	// Spin num_steps time before stopping
	uint8_t num_steps = 0;
	uint8_t left_arm_steps = is_extend ? left_arm.get_num_steps_extend() : left_arm.get_num_steps_retract();
	uint8_t right_arm_steps = is_extend ? right_arm.get_num_steps_extend() : right_arm.get_num_steps_retract();
	if (left_arm_selected && left_arm_steps > num_steps) {
		num_steps = left_arm_steps;
	}
	if (right_arm_selected && right_arm_steps > num_steps) {
		num_steps = right_arm_steps;
	}
	
	for (uint8_t i = 0; i < num_steps; i++) {
		
		if (left_arm_selected && i < left_arm_steps) {
			left_arm.step_once();
		}
		if (right_arm_selected && i < right_arm_steps) {
			right_arm.step_once();
		}
	}
	
	// Disable the motor after operation is done. Pull high to disable motor
	if (left_arm_selected || right_arm_selected) {
		PORTD |= 0b00000100;		
	}
}

void ArmControls::extend(uint8_t arm_mask) {
	this->spin(true, arm_mask);
}

void ArmControls::retract(uint8_t arm_mask) {
	this->spin(false, arm_mask);
}

void ArmControls::detect(uint8_t arm_mask) {
	// Check which arms need to be locked
	bool top_arm_selected = (arm_mask & ARM_CONTROLS_TOP) == ARM_CONTROLS_TOP;
	bool bottom_arm_selected = (arm_mask & ARM_CONTROLS_BOTTOM) == ARM_CONTROLS_BOTTOM;
	
	// Set to detect position
	if (top_arm_selected) {
		this->top_arm.detect();
	}
	if (bottom_arm_selected) {
		this->bottom_arm.detect();
	}
}

void ArmControls::lock_arms(uint8_t arm_mask) {
	
	// Enable the motor first. Pull low to enable motor
	PORTD &= ~0b00000100;
		
	// Check which arms need to be locked
	bool left_arm_selected = (arm_mask & ARM_CONTROLS_LEFT) == ARM_CONTROLS_LEFT;
	bool right_arm_selected = (arm_mask & ARM_CONTROLS_RIGHT) == ARM_CONTROLS_RIGHT;
	
	// Spin a few times to make sure the arms are secure
	for (uint8_t i = 0; i < 15; i++) {
		if (left_arm_selected) {
			left_arm.step_once();
		}
		if (right_arm_selected) {
			right_arm.step_once();
		}
	}
}

void ArmControls::unlock_arms(uint8_t arm_mask) {
	// Disable the motor. Pull high to disable motor
	PORTD |= 0b00000100;	
}

#endif /* ARM_CONTROLS_CPP_ */
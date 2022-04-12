/*
 * gripper.cpp
 *
 * Created: 4/12/2022 10:50:58 AM
 *  Author: Songdet Nillasithanukroh
 */ 

#ifndef GRIPPER_CPP_
#define GRIPPER_CPP_

#include <stdint.h>
#include "gripper.h"
#include "SWseriale.h"

uint8_t get_low_byte(uint16_t input) {
	return input & 0x7F;
}

uint8_t get_high_byte(uint16_t input) {
	return (input >> 7) & 0x7F;
}

Gripper::Gripper(uint8_t channel, uint16_t horizontal_left_pos, uint16_t vertical_pos, uint16_t horizontal_right_pos) {
	this->channel = channel;
	
	this->hl_low_byte = get_low_byte(horizontal_left_pos*4);
	this->hl_high_byte = get_high_byte(horizontal_left_pos*4);
	
	this->v_low_byte  = get_low_byte(vertical_pos*4);
	this->v_high_byte = get_high_byte(vertical_pos*4);
	
	this->hr_low_byte = get_low_byte(horizontal_right_pos*4);
	this->hr_high_byte = get_high_byte(horizontal_right_pos*4);
}

void Gripper::set_target(uint8_t high_byte, uint8_t low_byte) {
	uint8_t serial_bytes[4];
	serial_bytes[0] = 0x84; // Set command to set target
	serial_bytes[1] = this->channel; // Select channel
	serial_bytes[2] = low_byte; // Set target low byte
	serial_bytes[3] = high_byte; // Set target high byte
	SWseriale_write(serial_bytes, 4);
}

void Gripper::set_horizontal_left() {
	this->set_target(hl_high_byte, hl_low_byte);
}

void Gripper::set_vertical() {
	this->set_target(v_high_byte, v_low_byte);
}

void Gripper::set_horizontal_right() {
	this->set_target(hr_high_byte, hr_low_byte);	
}

#endif /* GRIPPER_CPP_ */
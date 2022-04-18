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
#include "wait.h"

static uint8_t get_low_byte(uint16_t input) {
	return input & 0x7F;
}

static uint8_t get_high_byte(uint16_t input) {
	return (input >> 7) & 0x7F;
}

Gripper::Gripper(uint8_t channel, uint16_t vertical_left_pos, uint16_t horizontal_pos, uint16_t vertical_right_pos, uint16_t detect_pos) {
	this->channel = channel;
	
	this->vl_low_byte = get_low_byte(vertical_left_pos*4);
	this->vl_high_byte = get_high_byte(vertical_left_pos*4);
	
	this->h_low_byte  = get_low_byte(horizontal_pos*4);
	this->h_high_byte = get_high_byte(horizontal_pos*4);
	
	this->vr_low_byte = get_low_byte(vertical_right_pos*4);
	this->vr_high_byte = get_high_byte(vertical_right_pos*4);
	
	this->dt_low_byte = get_low_byte(detect_pos*4);
	this->dt_high_byte = get_high_byte(detect_pos*4);
}

void Gripper::set_target(uint8_t high_byte, uint8_t low_byte) {
	uint8_t serial_bytes[4];
	serial_bytes[0] = 0x84; // Set command to set target
	serial_bytes[1] = this->channel; // Select channel
	serial_bytes[2] = low_byte; // Set target low byte
	serial_bytes[3] = high_byte; // Set target high byte
	SWseriale_write(serial_bytes, 4);
	wait(GRIPPER_CMD_WAIT_TIME_MS, 2);
}

void Gripper::set_vertical_left() {
	this->set_target(vl_high_byte, vl_low_byte);
}

void Gripper::set_horizontal() {
	this->set_target(h_high_byte, h_low_byte);
}

void Gripper::set_vertical_right() {
	this->set_target(vr_high_byte, vr_low_byte);
}

void Gripper::set_detect() {
	this->set_target(dt_high_byte, dt_low_byte);
}

#endif /* GRIPPER_CPP_ */
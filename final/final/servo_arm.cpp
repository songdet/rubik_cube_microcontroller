/*
 * servo_arm.cpp
 *
 * Created: 4/15/2022 3:21:55 PM
 *  Author: Songdet Nillasithanukroh
 */

#ifndef SERVO_ARM_CPP_
#define SERVO_ARM_CPP

#include "servo_arm.h"
#include "SWseriale.h"
#include "wait.h"

static uint8_t get_low_byte(uint16_t input) {
	return input & 0x7F;
}

static uint8_t get_high_byte(uint16_t input) {
	return (input >> 7) & 0x7F;
}

ServoArm::ServoArm(uint8_t channel, uint16_t extend_pos, uint16_t retract_pos, uint16_t detect_pos) {
	this->channel = channel;
	this->extend_high_byte = get_high_byte(extend_pos*4);
	this->extend_low_byte = get_low_byte(extend_pos*4);
	this->retract_high_byte = get_high_byte(retract_pos*4);
	this->retract_low_byte = get_low_byte(retract_pos*4);
	this->detect_high_byte = get_high_byte(detect_pos*4);
	this->detect_low_byte = get_low_byte(detect_pos*4);
}

void ServoArm::set_target(uint8_t high_byte, uint8_t low_byte) {
	uint8_t serial_bytes[4];
	serial_bytes[0] = 0x84; // Set command to set target
	serial_bytes[1] = this->channel; // Select channel
	serial_bytes[2] = low_byte; // Set target low byte
	serial_bytes[3] = high_byte; // Set target high byte
	SWseriale_write(serial_bytes, 4);
	wait(SERVO_ARM_CMD_WAIT_TIME_MS, 2);
}

void ServoArm::retract() {
	this->set_target(retract_high_byte, retract_low_byte);
}

void ServoArm::extend() {
	this->set_target(extend_high_byte, extend_low_byte);
}

void ServoArm::detect() {
	this->set_target(detect_high_byte, detect_low_byte);
}

#endif /* SERVO_ARM_CPP_ */

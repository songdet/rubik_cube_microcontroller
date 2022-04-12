/*
 * arm.cpp
 *
 * Created: 4/12/2022 1:25:59 PM
 *  Author: Songdet Nillasithanukroh
 */ 

#ifndef ARM_CPP_
#define ARM_CPP_

#include <avr/io.h>
#include "arm.h"
#include "wait.h"

Arm::Arm(volatile uint8_t* port, volatile uint8_t* ddr, uint8_t step_pin_mask, uint8_t dir_pin_mask) {
	
	// Initialize variables
	this->port = port;
	this->step_pin_mask = step_pin_mask;
	this->dir_pin_mask = dir_pin_mask;
	
	// Set up pin for stepping control and turn it off
	*ddr |= step_pin_mask;
	*port &= ~step_pin_mask;

	
	// Set up pin for direction control and turn it off
	*ddr |= dir_pin_mask;
	*port &= ~dir_pin_mask;
	
	// Set up the arm controls for full stepping
	DDRC |= 0b00111000; // Set PC3-PC5 as output ports
	PORTC &= ~0b00111000;  // Set stepper motor to use full stepping
	
	// Set up arm controls for enabling and disabling the motor
	DDRD |= 0b00000100; // Set PD2 as output port
	PORTD |= 0b00000100; // Pull high to disable motor
	
}

void Arm::spin(bool is_extend, uint8_t num_steps) {
	
	// Enable the motor first
	PORTD &= ~0b00000100; // Pull low to enable motor

	
	if (is_extend) {
		*this->port |= this->dir_pin_mask; // Pull high to extend
	} else {
		*this->port &= ~this->dir_pin_mask; // Pull low to retract
	}
	
	// Spin num_steps time before stopping
	for (uint8_t i = 0; i < num_steps; i++) {
		*this->port |= this->step_pin_mask; // Trigger one step forward
		wait(1, 2);
		*this->port &= ~this->step_pin_mask; // Pull low so it can be triggered again
		wait(1, 2);
	}
	
	// Disable the motor after operation is done
	PORTD |= 0b00000100; // Pull high to disable motor
}

void Arm::extend() {
	this->spin(true, 50);
}

void Arm::retract() {
	this->spin(false, 50);
}

#endif /* ARM_CPP_ */
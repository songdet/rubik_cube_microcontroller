/*
 * button.cpp
 *
 * Created: 4/11/2022 2:32:05 PM
 *  Author: Songdet Nillasithanukroh
 */ 

#ifndef BUTTON_cpp
#define BUTTON_cpp

#include <stddef.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include "button.h"
#include "wait.h"

// Keep track of button that was pushed
volatile ButtonType pushed_button = UNSET;

// Initialize static variables to empty
uint8_t Button::num_buttons = 0;
ButtonInfo Button::all_buttons[MAX_BUTTONS] = {};

// Functions to read buttons that was pushed
bool is_button_pushed() {
	return pushed_button != UNSET;
}

ButtonType get_pushed_button() {
	ButtonType prev_pushed_button = pushed_button;
	pushed_button = UNSET;
	return prev_pushed_button;
}

// Constructor definition
Button::Button(uint8_t pin_mask, ButtonType button_type) {
	
	// Set up selected pin as input with no pull-up pull-down for button control
	DDRC &= ~pin_mask;  // Set selected pin as input
	PORTC &= ~pin_mask; // Set selected pin to be no pull-up no pull-down state
	
	// Set up pin change interrupt on selected pins
	PCICR =  0b00000010; // Enable pin change interrupt on port C (bit 1)
	PCMSK1 |= pin_mask; // Turn on the interrupt on selected pin
	
	// Keep track of pins being used
	ButtonInfo new_button = {pin_mask, button_type};
	Button::all_buttons[Button::num_buttons++] = new_button;
}

#endif /* BUTTON_cpp */

ISR(PCINT1_vect) {
	
	// Wait 100ms for debouncing
	wait(100, 2);
	
	// Make sure the execution only occurs on falling edge
	bool falling_edge_found = false;
	ButtonInfo falling_edge_button = {};
	for (int i = 0; i < Button::num_buttons; i++) {
		ButtonInfo current_button = Button::all_buttons[i];
		if ((PINC & current_button.pin_mask) == 0) {
			falling_edge_found = true;
			falling_edge_button = current_button;
			break;
		} 
	}
	
	// If no falling edge was found, make sure interrupt is not raised again and return
	if (!falling_edge_found) {		
		PCIFR = 0b00000001;
		return;
	}
	
	// Set the button that was pushed so that it can be read by main later
	pushed_button = falling_edge_button.button_type;
	
	// Make sure button bounce doesn't cause interrupt to execute again
	PCIFR = 0b00000001;
}
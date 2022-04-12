/*
 * button.h
 *
 * Created: 4/11/2022 2:12:46 PM
 *  Author: Songdet Nillasithanukroh
 */ 


#ifndef BUTTON_H_
#define BUTTON_H_
#define MAX_BUTTONS 10

#include <stdint.h>

struct button_info {
	uint8_t pin_mask;
	char command;	
};

class Button
{
	private:
		uint8_t pin_mask;
	public:
		Button(uint8_t pin_mask, char command);
		static button_info all_buttons[MAX_BUTTONS];
		static uint8_t num_buttons;
};

#endif /* BUTTON_H_ */
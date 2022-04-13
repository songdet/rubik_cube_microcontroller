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

enum ButtonType {
	UNSET = 0,
	START = 1,
	STOP = 2,
	RESET = 3
};

struct ButtonInfo {
	uint8_t pin_mask;
	ButtonType button_type;	
};

bool is_button_pushed();
ButtonType get_pushed_button();

class Button
{
	public:
		Button(uint8_t pin_mask, ButtonType button_type);
		static ButtonInfo all_buttons[MAX_BUTTONS];
		static uint8_t num_buttons;
};

#endif /* BUTTON_H_ */
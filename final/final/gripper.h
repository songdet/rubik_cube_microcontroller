/*
 * gripper.h
 *
 * Created: 4/12/2022 10:50:38 AM
 *  Author: Songdet Nillasithanukroh
 */ 


#ifndef GRIPPER_H_
#define GRIPPER_H_

#include <stdint.h>

class Gripper {
	private:
		uint8_t channel;
		uint8_t hl_high_byte;
		uint8_t hl_low_byte;
		uint8_t v_high_byte;
		uint8_t v_low_byte;
		uint8_t hr_high_byte;
		uint8_t hr_low_byte;
		void set_target(uint8_t high_byte, uint8_t low_byte);
	public:
		void set_horizontal_left();
		void set_horizontal_right();
		void set_vertical();
		Gripper(uint8_t channel, uint16_t horizontal_left_pos, uint16_t vertical_pos, uint16_t horizontal_right_pos);	
};


#endif /* GRIPPER_H_ */
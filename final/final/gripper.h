/*
 * gripper.h
 *
 * Created: 4/12/2022 10:50:38 AM
 *  Author: Songdet Nillasithanukroh
 */ 


#ifndef GRIPPER_H_
#define GRIPPER_H_

#include <stdint.h>

#define GRIPPER_CMD_WAIT_TIME_MS 100

class Gripper {
	private:
		uint8_t channel;
		uint8_t vl_high_byte;
		uint8_t vl_low_byte;
		uint8_t h_high_byte;
		uint8_t h_low_byte;
		uint8_t vr_high_byte;
		uint8_t vr_low_byte;
		uint8_t dt_high_byte;
		uint8_t dt_low_byte;
		void set_target(uint8_t high_byte, uint8_t low_byte);
	public:
		void set_vertical_left();
		void set_vertical_right();
		void set_horizontal();
		void set_detect();
		Gripper(uint8_t channel, uint16_t vertical_left_pos, uint16_t horizontal_pos, uint16_t vertical_right_pos, uint16_t detect_pos);	
};


#endif /* GRIPPER_H_ */
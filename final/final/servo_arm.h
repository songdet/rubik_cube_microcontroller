/*
 * servo_arm.h
 *
 * Created: 4/15/2022 3:21:42 PM
 *  Author: Songdet Nillasithanukroh
 */ 


#ifndef SERVO_ARM_H_
#define SERVO_ARM_H_

#define SERVO_ARM_CMD_WAIT_TIME_MS 100

#include <stdint.h>

class ServoArm {
	private:
		uint8_t channel;
		uint8_t retract_low_byte;
		uint8_t retract_high_byte;
		uint8_t extend_low_byte;
		uint8_t extend_high_byte;
		uint8_t detect_low_byte;
		uint8_t detect_high_byte;
		void set_target(uint8_t high_byte, uint8_t low_byte);
	public:
		ServoArm(uint8_t channel, uint16_t extend_pos, uint16_t retract_pos, uint16_t detect_pos);
		void retract();
		void extend();
		void detect();
};



#endif /* SERVO_ARM_H_ */
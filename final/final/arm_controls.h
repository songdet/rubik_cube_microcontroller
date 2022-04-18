/*
 * arm_controls.h
 *
 * Created: 4/14/2022 3:43:20 PM
 *  Author: Songdet Nillasithanukroh
 */ 


#ifndef ARM_CONTROLS_H_
#define ARM_CONTROLS_H_

#define ARM_CONTROLS_LEFT 0x01
#define ARM_CONTROLS_RIGHT 0x02
#define ARM_CONTROLS_TOP 0x04
#define ARM_CONTROLS_BOTTOM 0x08
#define ARM_CONTROLS_LEFT_RIGHT 0x03
#define ARM_CONTROLS_TOP_BOTTOM 0x0C
#define ARM_CONTROLS_ALL 0x0F

#include "arm.h"
#include "servo_arm.h"

class ArmControls {
	private:
		Arm left_arm;
		Arm right_arm;
		ServoArm top_arm;
		ServoArm bottom_arm;
		void spin(bool is_extend, uint8_t arm_mask);
	public:
		void extend(uint8_t arm_mask);
		void retract(uint8_t arm_mask);
		void detect(uint8_t arm_mask);
		void lock_arms(uint8_t arm_mask);
		void unlock_arms(uint8_t arm_mask);
		ArmControls(Arm left_arm, Arm right_arm, ServoArm top_arm, ServoArm bottom_arm);
};


#endif /* ARM_CONTROLS_H_ */
/*
 * arm.h
 *
 * Created: 4/12/2022 1:25:46 PM
 *  Author: Songdet Nillasithanukroh
 */ 


#ifndef ARM_H_
#define ARM_H_

#include <stdint.h>
class Arm {
	private:
		volatile uint8_t* port;
		uint8_t step_pin_mask;
		uint8_t dir_pin_mask;
		uint8_t num_steps_extend;
		uint8_t num_steps_retract;
	public:
		Arm(volatile uint8_t* port, volatile uint8_t* ddr, uint8_t step_pin_mask, uint8_t dir_pin_mask, uint8_t num_steps_extend, uint8_t num_steps_retract);
		void set_direction(bool is_extend);
		void step_once();
		uint8_t get_num_steps_extend();
		uint8_t get_num_steps_retract();
};

#endif /* ARM_H_ */
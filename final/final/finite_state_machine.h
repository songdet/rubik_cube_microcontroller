/*
 * finite_state_machine.h
 *
 * Created: 4/12/2022 5:24:02 PM
 *  Author: Songdet Nillasithanukroh
 */ 

#ifndef FINITE_STATE_MACHINE_H_
#define FINITE_STATE_MACHINE_H_

#include "arm.h"
#include "gripper.h"

#define FSM_WAIT_ROTATION_MS 1000
#define FSM_WAIT_ARM_MS 1000

enum Isa {
	ST = 0,
	GR = 1,
	HH = 2,
	HV = 3,
	MV = 4,
	MH = 5,
	MHCCW = 6,
	RLC = 7,
	RLCCW = 8,
	RRC = 9,
	RRCCW = 10,
	RBC = 11,
	RBCCW = 12,
	RTC = 13,
	RTCCW = 14
};

struct ArmControls {
	Arm left_arm;
	Arm right_arm;
	Arm top_arm;
	Arm bottom_arm;
};

struct GripperControls {
	Gripper left_gripper;
	Gripper right_gripper;
	Gripper top_gripper;
	Gripper bottom_gripper;	
};

class FiniteStateMachine {
	private:
		GripperControls grippers;
		ArmControls arms;
		Isa current_state;
		void st_to_gr();
		void gr_to_hh();
		void gr_to_hv();
		void hh_to_gr();
		void hv_to_gr();
		void gr_to_st();
		void hh_to_st();
		void hv_to_st();
		void gr_to_rlc();
		void gr_to_rlccw();
		void gr_to_rrc();
		void gr_to_rrccw();
		void gr_to_rbc();
		void gr_to_rbccw();
		void gr_to_rtc();
		void gr_to_rtccw();
		void gr_to_mv();
		void gr_to_mh();
		void gr_to_mhccw();
		void reset_rotate_to_gr(Gripper* gripper, Arm* arm);
	public:
		FiniteStateMachine(GripperControls grippers, ArmControls arms);
		void transition(Isa isa);
		bool machine_started();
};

#endif /* FINITE_STATE_MACHINE_H_ */
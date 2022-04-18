/*
 * finite_state_machine.cpp
 *
 * Created: 4/12/2022 5:24:17 PM
 *  Author: Songdet Nillasithanukroh
 */

#ifndef FINITE_STATE_MACHINE_CPP_
#define FINITE_STATE_MACHINE_CPP_

#include "finite_state_machine.h"
#include "arm_controls.h"
#include "wait.h"

FiniteStateMachine::FiniteStateMachine(GripperControls grippers, ArmControls arms) : grippers(grippers), arms(arms), current_state(ST) {
	
}

void FiniteStateMachine::transition(Isa isa) {
	
	if (this->current_state == ST) {
		// Handle all the different transitions from ST to other states
		if (isa == GR) {
			this->st_to_gr();
			this->current_state = GR;
		} else if (isa == DT) {
			this->st_to_dt();
			this->current_state = DT;
		}
	} else if (this->current_state == GR) {
		// Handle all the different transitions from GR to other states
		if (isa == HV) {
			this->gr_to_hv();
			this->current_state = HV;
		} else if (isa == HH) {
			this->gr_to_hh();
			this->current_state = HH;
		} else if (isa == ST) {
			this->gr_to_st();
			this->current_state = ST;
		} else if (isa == RLC) {
			this->gr_to_rlc();
		} else if (isa == RLCCW) {
			this->gr_to_rlccw();
		} else if (isa == RRC) {
			this->gr_to_rrc();
		} else if (isa == RRCCW) {
			this->gr_to_rrccw();
		} else if (isa == RBC) {
			this->gr_to_rbc();
		} else if (isa == RBCCW) {
			this->gr_to_rbccw();
		} else if (isa == RTC) {
			this->gr_to_rtc();
		} else if (isa == RTCCW) {
			this->gr_to_rtccw();
		} else if (isa == MV) {
			this->gr_to_mv();
		} else if (isa == MH) {
			this->gr_to_mh();
		} else if (isa == MHCCW) {
			this->gr_to_mhccw();
		}
	} else if (this->current_state == HV) {
		// Handle all the different transitions from HV to other states
		if (isa == GR) {
			this->hv_to_gr();
			this->current_state = GR;
		} else if (isa == ST) {
			this->hv_to_st();
			this->current_state = ST;
		}
	} else if (this->current_state == HH) {
		// Handle all the different transitions from HV to other states
		if (isa == GR) {
			this->hh_to_gr();
			this->current_state = GR;
		} else if (isa == ST) {
			this->hh_to_st();
			this->current_state = ST;
		}
	} else if (this->current_state == DT) {
		// Handle all the different transitions from DT to other states
		if (isa == ST) {
			this->dt_to_st();
			this->current_state = ST;
		}
	}
}

bool FiniteStateMachine::machine_started() {
	return this->current_state != ST;
}

void FiniteStateMachine::st_to_gr() {
	this->arms.extend(ARM_CONTROLS_TOP_BOTTOM);
	wait(500, 2);
	this->arms.extend(ARM_CONTROLS_LEFT_RIGHT);
}

void FiniteStateMachine::st_to_dt() {
	this->grippers.bottom_gripper.set_detect();
	this->grippers.top_gripper.set_detect();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->arms.detect(ARM_CONTROLS_TOP_BOTTOM);
}

void FiniteStateMachine::gr_to_hv() {
	this->arms.retract(ARM_CONTROLS_LEFT_RIGHT);
}

void FiniteStateMachine::gr_to_hh() {
	this->arms.lock_arms(ARM_CONTROLS_LEFT_RIGHT);
	this->arms.retract(ARM_CONTROLS_TOP_BOTTOM);
}

void FiniteStateMachine::hv_to_gr() {
	this->arms.extend(ARM_CONTROLS_LEFT_RIGHT);
}

void FiniteStateMachine::hh_to_gr() {
	this->arms.extend(ARM_CONTROLS_TOP_BOTTOM);
	wait(FSM_WAIT_ARM_MS, 2);
	this->arms.unlock_arms(ARM_CONTROLS_LEFT_RIGHT);
}

void FiniteStateMachine::gr_to_st() {
	this->arms.retract(ARM_CONTROLS_ALL);
}

void FiniteStateMachine::hh_to_st() {
	this->arms.retract(ARM_CONTROLS_LEFT_RIGHT);
	wait(FSM_WAIT_ARM_MS, 2);
	this->arms.unlock_arms(ARM_CONTROLS_LEFT_RIGHT);
}

void FiniteStateMachine::hv_to_st() {
	this->arms.retract(ARM_CONTROLS_TOP_BOTTOM);
}

void FiniteStateMachine::gr_to_rlc() {
	this->grippers.left_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.left_gripper), ARM_CONTROLS_LEFT);
}

void FiniteStateMachine::gr_to_rlccw() {
	this->grippers.left_gripper.set_vertical_left();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.left_gripper), ARM_CONTROLS_LEFT);
}

void FiniteStateMachine::gr_to_rrc() {
	this->grippers.right_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.right_gripper), ARM_CONTROLS_RIGHT);
}

void FiniteStateMachine::gr_to_rrccw() {
	this->grippers.right_gripper.set_vertical_left();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.right_gripper), ARM_CONTROLS_RIGHT);
}

void FiniteStateMachine::gr_to_rbc() {
	this->grippers.bottom_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->arms.lock_arms(ARM_CONTROLS_LEFT_RIGHT);
	this->reset_rotate_to_gr(&(this->grippers.bottom_gripper), ARM_CONTROLS_BOTTOM);
	this->arms.unlock_arms(ARM_CONTROLS_LEFT_RIGHT);
}

void FiniteStateMachine::gr_to_rbccw() {
	this->grippers.bottom_gripper.set_vertical_left();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->arms.lock_arms(ARM_CONTROLS_LEFT_RIGHT);
	this->reset_rotate_to_gr(&(this->grippers.bottom_gripper), ARM_CONTROLS_BOTTOM);
	this->arms.unlock_arms(ARM_CONTROLS_LEFT_RIGHT);
}

void FiniteStateMachine::gr_to_rtc() {
	this->grippers.top_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.top_gripper), ARM_CONTROLS_TOP);
}

void FiniteStateMachine::gr_to_rtccw() {
	this->grippers.top_gripper.set_vertical_left();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.top_gripper), ARM_CONTROLS_TOP);
}

void FiniteStateMachine::gr_to_mv() {
	this->arms.retract(ARM_CONTROLS_LEFT);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.left_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.extend(ARM_CONTROLS_LEFT);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->arms.retract(ARM_CONTROLS_TOP_BOTTOM);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.left_gripper.set_horizontal();
	this->grippers.right_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.extend(ARM_CONTROLS_TOP_BOTTOM);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->arms.retract(ARM_CONTROLS_RIGHT);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.right_gripper.set_horizontal();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.extend(ARM_CONTROLS_RIGHT);
	wait(FSM_WAIT_ARM_MS, 2);
}

void FiniteStateMachine::gr_to_mh() {
	this->arms.retract(ARM_CONTROLS_TOP);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.top_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);

	this->arms.extend(ARM_CONTROLS_TOP);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->arms.retract(ARM_CONTROLS_LEFT_RIGHT);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.top_gripper.set_horizontal();
	this->grippers.bottom_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.extend(ARM_CONTROLS_LEFT_RIGHT);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->arms.lock_arms(ARM_CONTROLS_LEFT_RIGHT);
	this->arms.retract(ARM_CONTROLS_BOTTOM);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.bottom_gripper.set_horizontal();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.extend(ARM_CONTROLS_BOTTOM);
	wait(FSM_WAIT_ARM_MS, 2);
	this->arms.unlock_arms(ARM_CONTROLS_LEFT_RIGHT);
}

void FiniteStateMachine::gr_to_mhccw() {
	this->arms.retract(ARM_CONTROLS_TOP);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.top_gripper.set_vertical_left();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.extend(ARM_CONTROLS_TOP);
	wait(FSM_WAIT_ARM_MS, 2);

	this->arms.retract(ARM_CONTROLS_LEFT_RIGHT);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.top_gripper.set_horizontal();
	this->grippers.bottom_gripper.set_vertical_left();
	wait(FSM_WAIT_ROTATION_MS, 2);

	this->arms.extend(ARM_CONTROLS_LEFT_RIGHT);
	wait(FSM_WAIT_ARM_MS, 2);

	this->arms.retract(ARM_CONTROLS_BOTTOM);
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.bottom_gripper.set_horizontal();
	wait(FSM_WAIT_ROTATION_MS, 2);

	this->arms.extend(ARM_CONTROLS_BOTTOM);
	wait(FSM_WAIT_ARM_MS, 2);
}

void FiniteStateMachine::reset_rotate_to_gr(Gripper* gripper, uint8_t arm_mask) {
	this->arms.retract(arm_mask);
	wait(FSM_WAIT_ARM_MS, 2);
	
	(*gripper).set_horizontal();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.extend(arm_mask);
	wait(FSM_WAIT_ARM_MS, 2);
}

void FiniteStateMachine::dt_to_st() {
	this->arms.retract(ARM_CONTROLS_TOP_BOTTOM);
	wait(FSM_WAIT_ARM_MS, 2);
	this->grippers.bottom_gripper.set_horizontal();
	this->grippers.top_gripper.set_horizontal();
}

#endif /* FINITE_STATE_MACHINE_CPP_ */

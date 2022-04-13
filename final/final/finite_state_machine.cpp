/*
 * finite_state_machine.cpp
 *
 * Created: 4/12/2022 5:24:17 PM
 *  Author: Songdet Nillasithanukroh
 */

#ifndef FINITE_STATE_MACHINE_CPP_
#define FINITE_STATE_MACHINE_CPP_

#include "finite_state_machine.h"

FiniteStateMachine::FiniteStateMachine(GripperControls grippers, ArmControls arms) : grippers(grippers), arms(arms), current_state(ST) {
	
}

void FiniteStateMachine::transition(Isa isa) {
	
	if (this->current_state == ST) {
		// Handle all the different transitions from ST to other states
		if (isa == GR) {
			this->st_to_gr();
			this->current_state = GR;
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
	}
}

bool FiniteStateMachine::machine_started() {
	return this->current_state != ST;
}

void FiniteStateMachine::st_to_gr() {
	this->arms.left_arm.extend();
	this->arms.right_arm.extend();
	this->arms.top_arm.extend();
	this->arms.bottom_arm.extend();
}

void FiniteStateMachine::gr_to_hv() {
	this->arms.left_arm.retract();
	this->arms.right_arm.retract();
}

void FiniteStateMachine::gr_to_hh() {
	this->arms.top_arm.retract();
	this->arms.bottom_arm.retract();
}

void FiniteStateMachine::hv_to_gr() {
	this->arms.left_arm.extend();
	this->arms.right_arm.extend();
}

void FiniteStateMachine::hh_to_gr() {
	this->arms.top_arm.extend();
	this->arms.bottom_arm.extend();	
}

void FiniteStateMachine::gr_to_st() {
	this->arms.left_arm.retract();
	this->arms.right_arm.retract();
	this->arms.top_arm.retract();
	this->arms.right_arm.retract();
}

void FiniteStateMachine::hh_to_st() {
	this->arms.left_arm.retract();
	this->arms.right_arm.retract();
}

void FiniteStateMachine::hv_to_st() {
	this->arms.top_arm.retract();
	this->arms.right_arm.retract();
}

void FiniteStateMachine::gr_to_rlc() {
	this->grippers.left_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.left_arm.retract();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.left_gripper.set_horizontal();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.left_arm.retract();
	wait(FSM_WAIT_ARM_MS, 2);
}

#endif /* FINITE_STATE_MACHINE_CPP_ */

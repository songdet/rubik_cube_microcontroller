/*
 * finite_state_machine.cpp
 *
 * Created: 4/12/2022 5:24:17 PM
 *  Author: Songdet Nillasithanukroh
 */

#ifndef FINITE_STATE_MACHINE_CPP_
#define FINITE_STATE_MACHINE_CPP_

#include "finite_state_machine.h"
#include "wait.h"

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
	this->reset_rotate_to_gr(&(this->grippers.left_gripper), &(this->arms.left_arm));
}

void FiniteStateMachine::gr_to_rlccw() {
	this->grippers.left_gripper.set_vertical_left();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.left_gripper), &(this->arms.left_arm));
}

void FiniteStateMachine::gr_to_rrc() {
	this->grippers.right_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.right_gripper), &(this->arms.right_arm));
}

void FiniteStateMachine::gr_to_rrccw() {
	this->grippers.right_gripper.set_vertical_left();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.right_gripper), &(this->arms.right_arm));
}

void FiniteStateMachine::gr_to_rbc() {
	this->grippers.bottom_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.bottom_gripper), &(this->arms.bottom_arm));
}

void FiniteStateMachine::gr_to_rbccw() {
	this->grippers.bottom_gripper.set_vertical_left();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.bottom_gripper), &(this->arms.bottom_arm));
}

void FiniteStateMachine::gr_to_rtc() {
	this->grippers.top_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.top_gripper), &(this->arms.top_arm));
}

void FiniteStateMachine::gr_to_rtccw() {
	this->grippers.top_gripper.set_vertical_left();
	wait(FSM_WAIT_ROTATION_MS, 2);
	this->reset_rotate_to_gr(&(this->grippers.bottom_gripper), &(this->arms.bottom_arm));
}

void FiniteStateMachine::gr_to_mv() {
	this->arms.left_arm.retract();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.left_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.left_arm.extend();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->arms.top_arm.retract();
	this->arms.bottom_arm.retract();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.left_gripper.set_horizontal();
	this->grippers.right_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.top_arm.extend();
	this->arms.bottom_arm.extend();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->arms.right_arm.retract();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.right_gripper.set_horizontal();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.right_arm.extend();
	wait(FSM_WAIT_ARM_MS, 2);
}

void FiniteStateMachine::gr_to_mh() {
	this->arms.top_arm.retract();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.top_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.top_arm.extend();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->arms.left_arm.retract();
	this->arms.right_arm.retract();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.top_gripper.set_horizontal();
	this->grippers.bottom_gripper.set_vertical_right();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.left_arm.extend();
	this->arms.right_arm.extend();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->arms.bottom_arm.retract();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.bottom_gripper.set_horizontal();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.bottom_arm.extend();
	wait(FSM_WAIT_ARM_MS, 2);
}

void FiniteStateMachine::gr_to_mhccw() {
	this->arms.top_arm.retract();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.top_gripper.set_vertical_left();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.top_arm.extend();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->arms.left_arm.retract();
	this->arms.right_arm.retract();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.top_gripper.set_horizontal();
	this->grippers.bottom_gripper.set_vertical_left();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.left_arm.extend();
	this->arms.right_arm.extend();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->arms.bottom_arm.retract();
	wait(FSM_WAIT_ARM_MS, 2);
	
	this->grippers.bottom_gripper.set_horizontal();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	this->arms.bottom_arm.extend();
	wait(FSM_WAIT_ARM_MS, 2);
}

void FiniteStateMachine::reset_rotate_to_gr(Gripper* gripper, Arm* arm) {
	(*arm).retract();
	wait(FSM_WAIT_ARM_MS, 2);
	
	(*gripper).set_horizontal();
	wait(FSM_WAIT_ROTATION_MS, 2);
	
	(*arm).extend();
	wait(FSM_WAIT_ARM_MS, 2);
}

#endif /* FINITE_STATE_MACHINE_CPP_ */

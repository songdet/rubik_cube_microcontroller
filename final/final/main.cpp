/*
 * final.cpp
 *
 * Created: 4/11/2022 1:53:00 PM
 * Author : Songdet Nillasithanukroh
 */ 

#include "SWSeriale.h"
#include "button.h"
#include "gripper.h"
#include "arm.h"
#include "control_communication.h"
#include "finite_state_machine.h"

int main(void)
{		

	// Set up the buttons that are used to control the operation of the machine
	Button(0x01, START); // The start button is on PC0 and when pressed, send 'O' command
	Button(0x02, STOP); // The stop button is on PC1 and when pressed, send 'S' command
	Button(0x04, RESET); // The reset button is on PC2 and when pressed, send the 'R' command
	
	// Set up the gripper objects that are used to control the gripper
	Gripper left_gripper = Gripper(0, 550, 1200, 1840);
	Gripper right_gripper = Gripper(1, 550, 1200, 1840);
	Gripper top_gripper = Gripper(2, 550, 1150, 1800);
	Gripper bottom_gripper = Gripper(3, 600, 1250, 1900);
	GripperControls gripper_controls = { left_gripper, right_gripper, top_gripper, bottom_gripper };
	
	// Set up the arm objects used to control the arms
	Arm left_arm = Arm(&PORTB, &DDRB, 0x01, 0x02);
	Arm right_arm = Arm(&PORTB, &DDRB, 0x04, 0x08);
	Arm top_arm = Arm(&PORTB, &DDRB, 0x10, 0x20);
	Arm bottom_arm = Arm(&PORTD, &DDRD, 0x20, 0x40);
	ArmControls arm_controls = { left_arm, right_arm, top_arm, bottom_arm };
		
	// Set up finite state machine that will keep track of state and does operation during transition
	FiniteStateMachine machine_state = FiniteStateMachine(gripper_controls, arm_controls);
	
	// Set up control server communication through RX and TX ports on PD0-PD1
	init_control_communication();
	
	// Set up software serial using INT1 with PD3 as RX and PD4 as TX
	SWseriale_begin();
	
	// Enable global interrupt
	sei();
	
    while (1) 
    {
		// Check if button has been pushed during transition
		ButtonType pushed_button = UNSET;
		if (is_button_pushed()) {
			pushed_button = get_pushed_button();
		}

		// Send command to control server based on which button was pushed and whether machine is started		
		bool machine_started = machine_state.machine_started();
		if (!machine_started && pushed_button == START) {
			send_command('O');
			machine_started = true;
		} else if (machine_started && pushed_button == STOP) {
			send_command('S');
		} else if (machine_started && pushed_button == RESET) {
			send_command('R');
		} else if (machine_started) {
			send_command('O');
		}
		
		// Hang wait until we get new command from control server
		if (machine_started) {
			while (!has_new_command());
			uint8_t new_command = receive_command();
			machine_state.transition(static_cast<Isa>(new_command));	
		}
		
		// We don't really need the returned data, but read to clear buffer
		while (SWseriale_available()) {
			SWseriale_read();
		}
    }
}


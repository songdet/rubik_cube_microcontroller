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

int main(void)
{		

	// Set up the buttons that are used to control the operation of the machine
	__attribute__((__unused__)) Button start_button = Button(0x01, 'O'); // The start button is on PC0 and when pressed, send 'O' command
	__attribute__((__unused__)) Button stop_button = Button(0x02, 'S'); // The stop button is on PC1 and when pressed, send 'S' command
	//__attribute__((__unused__)) Button reset_button = Button(0x04, 'R'); // The reset button is on PC2 and when pressed, send the 'R' command
	
	// Set up the gripper objects that are used to control the gripper
	Gripper left_gripper = Gripper(0, 900, 1600, 2200);
	Gripper right_gripper = Gripper(1, 700, 1400, 2050);
	Gripper top_gripper = Gripper(2, 1175, 1900, 2600);
	Gripper bottom_gripper = Gripper(3, 700, 1400, 2050);
	
	// Set up the arm objects used to control the arms
	Arm left_arm = Arm(&PORTB, &DDRB, 0x01, 0x02);
	Arm right_arm = Arm(&PORTB, &DDRB, 0x04, 0x08);
	Arm top_arm = Arm(&PORTB, &DDRB, 0x10, 0x20);
	Arm bottom_arm = Arm(&PORTD, &DDRD, 0x20, 0x40);
	
	// Set up control server communication through RX and TX ports on PD0-PD1
	init_control_communication();
	
	// Set up software serial using INT1 with PD3 as RX and PD4 as TX
	SWseriale_begin();
	
	// Enable global interrupt
	sei();
	
    while (1) 
    {
		// Read in the commands from control server and do state transitions
		while (has_new_command()) {
			uint8_t command = receive_command();
			if (command == 0x00) {
				left_gripper.set_horizontal_left();
			} else if (command == 0x01) {
				left_gripper.set_vertical();
			} else if (command == 0x02) {
				left_gripper.set_horizontal_right();
			} else if (command == 0x03) {
				left_arm.extend();
			} else if (command == 0x04) {
				left_arm.retract();
			}
		}
		
		// We don't really need the returned data, but read to clear buffer
		while (SWseriale_available()) {
			SWseriale_read();
		}
    }
}


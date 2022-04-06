/*
 * test_send_receive.cpp
 *
 * Created: 3/29/2022 9:30:36 PM
 * Author : Songdet
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "SWseriale.h"

void wait(volatile int multiple, volatile char time_choice);
void delay_T_msec_timer0(char choice);

int current_button = 0;
int gripper_position = 0;
int arm_position = 0;

int main(void)
{
	/// Set up PB0-1 as output for controlling motor 1 and turn it off
	DDRB |= 0b00000011;
	PORTB &= ~0b00000011;
	
	// Set up PC0-1 as input with no pull-up pull-down state for button control
	DDRC &= ~0b00000011; // Set PC0-1 as input
	PORTC &= ~0b00000011; // set the input pins as no pull up no pull down state
	
	// Set up PC2-PC5 as output for controlling the stepper motor.
	DDRC |= 0b00111100; // Set PC2-PC5 as output ports
	PORTC |= 0b00100000; // Turn off stepper motor until it's used
	PORTC &= ~0b00011100;  // Set stepper motor to use full stepping
	
	// Set up pin change interrupt on PC0-1
	PCICR = 0b00000010; // Enable pin change interrupt on port C (bit 1)
	PCMSK1 = 0b00000011; // Turn on the interrupt on PC0-1
	sei(); // Enable global interrupt
	
	// Set up serial receiver and transmitter
	UCSR0B = 0b10011000; // Enable receive complete interrupt (bit 7), reception (bit 4), and transmission (bit 3)
	UCSR0C = 0b00000110; //(1<<UCSZ01) | (1<<UCSZ00); // Asynchronous mode, 8-bit data; no parity; 1 stop bit
	UBRR0L = 0x67; // 9,600 baud if Fosc = 16MHz
	
	// Set up software serial using INT1 with PD3 as RX and PD4 as TX
	SWseriale_begin();
	
    while (1) 
    {	
		// We don't really need the returned data, but read to clear buffer
		while (SWseriale_available()) {
			SWseriale_read();
		}
    }
}

ISR(PCINT1_vect) {
	
	// Wait 100ms for debouncing
	wait(100, 2);
	
	// Make sure the execution only occurs on falling edge
	if ((PINC & 0b00000001) != 0 && (PINC & 0b00000010) != 0) {
		return;
	}
	
	// Send either A or B depending on which button is pressed
	if ((PINC & 0b00000001) == 0) {
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = 'A';
		current_button = 0;
	} else {
		while (!(UCSR0A & (1<<UDRE0)));
		UDR0 = 'B';
		current_button = 1;
	}
	
	// End the line with \n
	while (!( UCSR0A & (1<<UDRE0)));
	UDR0 = '\n';
	
	// Make sure button bounce doesn't cause interrupt to execute again
	PCIFR = 0b00000001;
}

ISR(USART_RX_vect) {
	
	// Get the byte specifying the speed
	char received_data = UDR0;
	
	uint8_t serial_bytes[32];	
	if (current_button == 0) {
		
		// Set up the speed for first target channel
		serial_bytes[0] = 0x87; // Set command to set speed
		serial_bytes[1] = 0x00; // Select channel 0
		serial_bytes[2] = received_data; // Not used since speed is between 0-100
		serial_bytes[3] = 0x00; // Set speed to received data
			
		// Set up the speed for second target channel
		serial_bytes[4] = 0x87; // Set command to set speed
		serial_bytes[5] = 0x01; // Select channel 1
		serial_bytes[6] = received_data; // Not used since speed is between 0-100
		serial_bytes[7] = 0x00; // Set speed to received data
		
		// Set up the speed for second target channel
		serial_bytes[8] = 0x87; // Set command to set speed
		serial_bytes[9] = 0x02; // Select channel 2
		serial_bytes[10] = received_data; // Not used since speed is between 0-100
		serial_bytes[11] = 0x00; // Set speed to received data
		
		// Set up the speed for second target channel
		serial_bytes[12] = 0x87; // Set command to set speed
		serial_bytes[13] = 0x03; // Select channel 3
		serial_bytes[14] = received_data; // Not used since speed is between 0-100
		serial_bytes[15] = 0x00; // Set speed to received data
		
		if (gripper_position == 0) {
			
			// Set top position
			serial_bytes[16] = 0x84; // Set command to set target
			serial_bytes[17] = 0x00; // Select channel 0
			serial_bytes[18] = 0x3B; // Set target low bits
			serial_bytes[19] = 0x1E; // Set target high bits
			
			// Set bottom position
			serial_bytes[20] = 0x84; // Set command to set target
			serial_bytes[21] = 0x01; // Select channel 0
			serial_bytes[22] = 0x51; // Set target low bits
			serial_bytes[23] = 0x18; // Set target high bits
			
			// Set left position
			serial_bytes[24] = 0x84; // Set command to set target
			serial_bytes[25] = 0x02; // Select channel 2
			serial_bytes[26] = 0x7A; // Set target low bits
			serial_bytes[27] = 0x26; // Set target high bits
			
			// Set right position
			serial_bytes[28] = 0x84; // Set command to set target
			serial_bytes[29] = 0x03; // Select channel 3
			serial_bytes[30] = 0x6C; // Set target low bits
			serial_bytes[31] = 0x19; // Set target high bits
						
			// Alternate current gripper position
			gripper_position = 1;
		} else {
			
			// Set top position
			serial_bytes[16] = 0x84; // Set command to set target
			serial_bytes[17] = 0x00; // Select channel 0
			serial_bytes[18] = 0x4D; // Set target low bits
			serial_bytes[19] = 0x32; // Set target high bits
			
			// Set bottom position
			serial_bytes[20] = 0x84; // Set command to set target
			serial_bytes[21] = 0x01; // Select channel 0
			serial_bytes[22] = 0x67; // Set target low bits
			serial_bytes[23] = 0x2B; // Set target high bits
			
			// Set left position
			serial_bytes[24] = 0x84; // Set command to set target
			serial_bytes[25] = 0x02; // Select channel 2
			serial_bytes[26] = 0x4B; // Set target low bits
			serial_bytes[27] = 0x3B; // Set target high bits
			
			// Set right position
			serial_bytes[28] = 0x84; // Set command to set target
			serial_bytes[29] = 0x03; // Select channel 3
			serial_bytes[30] = 0x60; // Set target low bits
			serial_bytes[31] = 0x2D; // Set target high bits
			
			// Alternate current gripper position
			gripper_position = 0;
		}
		
		SWseriale_write(serial_bytes, 32);
		
	} else {
		
		PORTC &= ~0b00100000; // Turn on stepper motor
		
		if (received_data == 1) {
			PORTB &= ~0b00000010; // Pull direction pin low to extend arm
		} else {
			PORTB |= 0b00000010; // Pull direction pin high to retract arm
		}
		
		
		// Rotate a few times so result can be seen
		for (int i = 0; i < 60; i++) {
			PORTB |= 0b00000001; // Trigger one step forward
			wait(10, 2);
			PORTB &= ~0b00000001; // Pull low so it can be triggered again
			wait(10, 2);
		}
		
		PORTC |= 0b00100000; // Turn off stepper motor
	}
	
}

void wait(volatile int multiple, volatile char time_choice) {
	/* This subroutine calls others to create a delay.
		 Total delay = multiple*T, where T is in msec and is the delay created by the called function.
	
		Inputs: multiple = number of multiples to delay, where multiple is the number of times an actual delay loop is called.
		Outputs: None
	*/
	
	while (multiple > 0) {
		delay_T_msec_timer0(time_choice); 
		multiple--;
	}
} // end wait()

void delay_T_msec_timer0(volatile char choice) {
    //*** delay T ms **
    /* This subroutine creates a delay of T msec using TIMER0 with prescaler on clock, where, for a 16MHz clock:
    		for Choice = 1: T = 0.125 msec for prescaler set to 8 and count of 250 (preload counter with 5)
    		for Choice = 2: T = 1 msec for prescaler set to 64 and count of 250 (preload counter with 5)
    		for Choice = 3: T = 4 msec for prescaler set to 256 and count of 250 (preload counter with 5)
    		for Choice = 4: T = 16 msec for prescaler set to 1,024 and count of 250 (preload counter with 5)
			for Choice = Default: T = .0156 msec for no prescaler and count of 250 (preload counter with 5)
	
			Inputs: None
			Outputs: None
	*/
	
	TCCR0A = 0x00; // clears WGM00 and WGM01 (bits 0 and 1) to ensure Timer/Counter is in normal mode.
	TCNT0 = 0;  // preload value for testing on count = 250
	// preload value for alternate test on while loop: TCNT0 = 5;  // preload load TIMER0  (count must reach 255-5 = 250)
	
	switch ( choice ) { // choose prescaler
		case 1:
			TCCR0B = 0b00000010; //1<<CS01;	TCCR0B = 0x02; // Start TIMER0, Normal mode, crystal clock, prescaler = 8
		break;
		case 2:
			TCCR0B =  0b00000011; //1<<CS01 | 1<<CS00;	TCCR0B = 0x03;  // Start TIMER0, Normal mode, crystal clock, prescaler = 64
		break;
		case 3:
			TCCR0B = 0b00000100; //1<<CS02;	TCCR0B = 0x04; // Start TIMER0, Normal mode, crystal clock, prescaler = 256
		break; 
		case 4:
			TCCR0B = 0b00000101; //1<<CS02 | 1<<CS00; TCCR0B = 0x05; // Start TIMER0, Normal mode, crystal clock, prescaler = 1024
		break;
		default:
			TCCR0B = 0b00000001; //1<<CS00; TCCR0B = 0x01; Start TIMER0, Normal mode, crystal clock, no prescaler
		break;
	}
	
	while (TCNT0 < 0xFA); // exits when count = 250 (requires preload of 0 to make count = 250)
	// alternate test on while loop: while ((TIFR0 & (0x1<<TOV0)) == 0); // wait for TOV0 to roll over:
	// How does this while loop work?? See notes
	
	TCCR0B = 0x00; // Stop TIMER0
	//TIFR0 = 0x1<<TOV0;  // Alternate while loop: Clear TOV0 (note that this is a nonintuitive bit in that it is cleared by writing a 1 to it)
	
} // end delay_T_msec_timer0()
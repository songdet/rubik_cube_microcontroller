/*
 * control_communication.cpp
 *
 * Created: 4/11/2022 10:23:52 PM
 *  Author: Songdet Nillasithanukroh
 */ 

#ifndef CONTROL_COMMUNICATION_CPP_
#define CONTROL_COMMUNICATION_CPP_

#include <stddef.h>
#include <avr/interrupt.h>
#include "control_communication.h"

volatile uint8_t receive_buffer[CONTROL_RECEIVE_BUF_SIZE];
volatile uint8_t receive_buffer_last_log_item = 0; // last logged item received from control server
volatile uint8_t receive_buffer_last_read_item = 0; // last read item (from program Main)

void init_control_communication() {
	UCSR0B = 0b10011000; // Enable receive complete interrupt (bit 7), reception (bit 4), and transmission (bit 3)
	UCSR0C = 0b00000110; //(1<<UCSZ01) | (1<<UCSZ00); // Asynchronous mode, 8-bit data; no parity; 1 stop bit
	UBRR0L = 0x67; // 9,600 baud if Fosc = 16MHz
}

void send_command(char command) {
	
	// Send command to control server
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = command;
		
	// End the line with \n
	while (!(UCSR0A & (1<<UDRE0)));
	UDR0 = '\n';
}

bool has_new_command() {
	uint8_t temp_last_log_item = receive_buffer_last_log_item; // for buffering, to prevent changes from interrupt
	uint8_t temp_last_pro_item = receive_buffer_last_read_item; // for buffering, to prevent changes from interrupt
	return temp_last_log_item != temp_last_pro_item; // We have new command if read item is not the same as logged item
}

uint8_t receive_command() {
	uint8_t new_data = receive_buffer[receive_buffer_last_read_item++];
	if (receive_buffer_last_read_item == CONTROL_RECEIVE_BUF_SIZE) receive_buffer_last_read_item = 0; // rollover
	return new_data;	
}

ISR(USART_RX_vect) {
	receive_buffer[receive_buffer_last_log_item++] = UDR0;
	if (receive_buffer_last_log_item == CONTROL_RECEIVE_BUF_SIZE) receive_buffer_last_log_item=0; // rollover
}

#endif
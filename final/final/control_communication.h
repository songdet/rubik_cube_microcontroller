/*
 * control_communication.h
 *
 * Created: 4/11/2022 10:20:21 PM
 *  Author: Songdet Nillasithanukroh
 */ 


#ifndef CONTROL_COMMUNICATION_H_
#define CONTROL_COMMUNICATION_H_

#include <stdint.h>

#define CONTROL_RECEIVE_BUF_SIZE (uint8_t)64 // buffer size for serial data receive

void init_control_communication();
void send_command(char command);
bool has_new_command();
uint8_t receive_command();

#endif /* CONTROL_COMMUNICATION_CPP_ */
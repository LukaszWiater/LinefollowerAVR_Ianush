/*
 * UART.h
 *
 * Created: 2017-05-31 02:00:14
 *  Author: User
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <util/delay.h>

#define UBRR 6
void uartInit();
char uartReceive();
void uartReceiveFrame(char frame[], int bytes);
void uartSendByte(uint8_t u8Data);
void uartSendFrame(char frame[], uint8_t bytes);

#endif /* UART_H_ */
/*
 * UART.c
 *
 * Created: 2017-05-31 02:03:35
 *  Author: User
 */ 
 #include "UART.h"

 void uartInit()
 {
	DDRD|=(1<<PD1);
	DDRD&=~(1<<PD0); PORTD|=(1<<PD0);

	UBRRL = 103;              //103 9600 baud 16MHz
	//UBRRL = 12;                 //6 19200 baud at 1Mhz
	//UCSRA|=(1<<U2X);
	//Enable receiver and transmitter
	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE);
	/* Set frame format: 8data, 1stop bit */
	UCSRC =(1<<URSEL)|(3<<UCSZ0);
 }

 char uartReceive()
 {
	 while(!(bit_is_set(UCSRA,RXC)))
	 {
		 PORTB|=(1<<PB3);
		 _delay_ms(1000);
		 PORTB&=~(1<<PB3);
		 _delay_ms(1000);
	 }
	 
	 /* Get and return received data from buffer */
	 return UDR;
 }
 
 void uartReceiveFrame(char frame[], int bytes)
 {
	for(int i=0;i<bytes;i++)
	{
		frame[i]=uartReceive();
	}
 }

 void uartSendByte(uint8_t u8Data){

	 // Wait until last byte has been transmitted
	 while((UCSRA &(1<<UDRE)) == 0);
	
	 // Transmit data
	 UDR = u8Data;
 }

void uartSendFrame(char frame[], uint8_t bytes)
{
	for(int i=0;i<bytes;i++)
		uartSendByte(frame[i]);
}
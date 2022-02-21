/*
 * timer0.c
 *
 * Created: 2018-03-05 12:42:47
 *  Author: wooca
 */ 

#include "timer0.h"

void timer0Init()
{
	TCCR0|=(1<<WGM01)|(5<<CS00);       //CTC, preskaler 1024
	TIMSK|=(1<<OCIE0);                 //przerwanie przy compare match
	OCR0=10;                          // normalnie OCR0=10; 
}
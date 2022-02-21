/*
 * ADC.c
 *
 * Created: 2018-03-01 01:18:12
 *  Author: wooca
 */ 

#include "ADC.h"

void adcInit()
{
	ADMUX|=(1<<REFS0);
	ADCSRA|=(1<<ADEN)|(3<<ADPS0);
}

void adcLedInit()
{
	MCUCSR |=(1<<JTD);
	MCUCSR |=(1<<JTD);
	
	DDRC|=0xF0;            //diody
	DDRB|=0x0F;
	
}

void adcReadSensors(uint8_t sensor_state[])
{
	uint8_t measurement;
	uint8_t threshold=3;
	
	for(int i=0;i<8;i++)
	{
		ADMUX&= 0b11100000;
		ADMUX|= i;
		ADCSRA|=(1<<ADSC);
		while(bit_is_set(ADCSRA,ADSC)) {};
		measurement=ADCH;
		if(measurement>=threshold)
			sensor_state[i]=1;
		else
			sensor_state[i]=0;
	}
}

void adcUpdateLedState(uint8_t sensor_state[])
{
	for(int i=0;i<4;i++)
	{
		if(sensor_state[i]==1)
			PORTB|=(1<<(3-i));
		else
			PORTB&=~(1<<(3-i));
	}
	
	for(int i=4;i<8;i++)
	{
		if(sensor_state[i]==1)
			PORTC|=(1<<(11-i));
		else
			PORTC&=~(1<<(11-i));
	}
}
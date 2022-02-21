/*
 * Ianush_program.c
 *
 * Created: 2018-03-01 00:52:44
 * Author : wooca
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include "ADC.h"
#include "UART.h"
#include "pwm.h"

//----------LEDY-------------------------
#define LED0 PB3
#define LED1 PB2
#define LED2 PB1
#define LED3 PB0
#define LED4 PC7
#define LED5 PC6
#define LED6 PC5
#define LED7 PC4

//---------------------flags-----------------------
volatile uint8_t ride=0, run_program=0;  //volatile uint8_t ride=0, counter=0, run_program=0;

//-----------------------PID-----------------------
volatile int Kp=2, Kd=4, prev_error=0, y_zad=0, outside_error=50;

//---------------------basic_settings--------------------
volatile uint8_t basic_velocity=30, turn_velocity=20, treshold_u=10, max_pwm=150;
volatile int rates[8]={-40,-20,-10,-5,5,10,20,40};




ISR(INT0_vect)
{
	GICR&=~(1<<INT0);
	_delay_ms(80);
	if(ride==1)
		ride=0;
	else
		ride=1;
	GICR|=(1<<INT0);
}

ISR(TIMER0_COMP_vect)
{
	//run_program=1;
	if(ride)
		robotMainLoop();
		
	else
	{
		PORTC&=0x0F;
		PORTB&=0xF0;
		
		PWML=0;
		PWMR=0; 
	}
}

ISR(USART_RXC_vect, ISR_BLOCK)
{
	 robotMessageHandler();
}


int main(void)
{
	volatile int error=0, control_signal=0;
	volatile uint8_t velocity;
	
	uint8_t sensors[7];
	velocity=basic_velocity;
	
	uartInit();
	adcInit();
	adcLedInit();
	pwmInit();
	robotSwitchInit();
	timer0Init();
	robotBridgeInit();
	
	sei();
	
    while (1) 
	{
		//doing nothing! :D
	}
}


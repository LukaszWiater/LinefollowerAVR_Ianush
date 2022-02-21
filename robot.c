/*
 * robot.c
 *
 * Created: 2018-03-05 23:11:34
 *  Author: wooca
 */ 

#include "robot.h"

extern volatile uint8_t ride, run_program, basic_velocity, turn_velocity, treshold_u, max_pwm;
extern volatile int Kp, Kd, prev_error, y_zad, outside_error;
extern volatile int rates[8];

void robotBridgeInit()
{
	DDRD|=(1<<AIN1)|(1<<AIN2);
	DDRC|=(1<<BIN1)|(1<<BIN2);
	
	PORTD|=(1<<AIN1);
	PORTD&=~(1<<AIN2);
	PORTC|=(1<<BIN1);
	PORTC&=~(1<<BIN2);
}

void robotSwitchInit()
{
	PORTD|=(1<<PD2);           //przycisk (we z podciagnieciem)
	MCUCR|=(1<<ISC01);
	GICR|=(1<<INT0);
	GIFR|=(1<<INTF0);
}

int robotCalculateError(uint8_t sensor_state[])
{
	int c_error=0;
	int quantity=0;
	
	for(int i=0;i<8;i++)
	{
		c_error=(c_error+(sensor_state[i]*rates[i]));
		quantity+=sensor_state[i];
	}
	
	if(quantity!=0)
	{
		c_error=c_error/quantity;
		prev_error=c_error;
	}
	else
	{
		if(prev_error>0)
			c_error=outside_error;
		else
			c_error=-outside_error;
			
		//c_error=prev_error;
	}
	
	return c_error;
}

void robotSetVelocity(int16_t control_signal, int sensor_error)
{
	int16_t left=0, right=0;
	
	if((abs(sensor_error))>=treshold_u)  //if((abs(control_signal))>=treshold_u)
	{
		left=turn_velocity+control_signal;
		right=turn_velocity-control_signal;
	}
	else
	{
		left=basic_velocity+control_signal;
		right=basic_velocity-control_signal;
	}
	
	if(left >= 0)
	{
		if(left>max_pwm)
			left = max_pwm;
		PORTC|=(1<<BIN1);
		PORTC&=~(1<<BIN2);
	}
	else
	{
		if(left<-max_pwm)
			left = -max_pwm;
		PORTC|=(1<<BIN2);
		PORTC&=~(1<<BIN1);
	}
	
	if(right >= 0)
	{
		if(right>max_pwm)
			right = max_pwm;
		PORTD|=(1<<AIN2);
		PORTD&=~(1<<AIN1);
	}
	else
	{
		if(right<-max_pwm)
			right = -max_pwm;
		PORTD|=(1<<AIN1);
		PORTD&=~(1<<AIN2);
	}
	
	PWML = abs(left);
	PWMR = abs(right);
}

/*
int robotRegPD(int error)
{
	int result;
	int differential=error-prev_error;
	prev_error=error;
	result=((Kp*error)+(Kd*differential));
	return result;
}
*/

/*
void robotSetPWM(int left, int right)
{
	int max_pwm=150;
	
	if(left >= 0)
	{
		if(left>max_pwm)
			left = max_pwm;
		PORTC|=(1<<BIN1);
		PORTC&=~(1<<BIN2);
	}
	else
	{
		if(left<-max_pwm)
			left = -max_pwm;
		PORTC|=(1<<BIN2);
		PORTC&=~(1<<BIN1);
	}
	
	if(right >= 0)
	{
		if(right>max_pwm)
			right = max_pwm;
		PORTD|=(1<<AIN2);
		PORTD&=~(1<<AIN1);
	}
	else
	{
		if(right<-max_pwm)
			right = -max_pwm;
		PORTD|=(1<<AIN1);
		PORTD&=~(1<<AIN2);
	}
	
	PWML = abs(left);
	PWMR = abs(right);
}
*/

void robotMessageHandler()
{
	char text[50];
	char end_char='@';
	uint8_t numof_bytes_received=0;
	
	
	do 
	{
		while(bit_is_set(UCSRA,RXC))
		{
			text[numof_bytes_received]=UDR;
			++numof_bytes_received;
		}
		
	} while (text[numof_bytes_received-1]!=end_char);
	
	numof_bytes_received=numof_bytes_received-1;
	
	//if(numof_bytes_received<5)
		//return;
		
	switch (text[0])
	{
		case 's':
			ride=1;
		break;
		
		case 't':
			ride=0;
		break;
		
		case 'K':
			if(text[1]=='p')
			Kp=robotLettersToNumber(&text[2],NUMBER_LENGTH);

			if(text[1]=='d')
			Kd=robotLettersToNumber(&text[2],NUMBER_LENGTH);

			if(text[1]=='y')
			y_zad=robotLettersToNumber(&text[2],NUMBER_LENGTH);

		break;
		
		case 'V':
			if(text[1]=='b')
			basic_velocity=robotLettersToNumber(&text[2],NUMBER_LENGTH);
			
			if(text[1]=='t')
			turn_velocity=robotLettersToNumber(&text[2],NUMBER_LENGTH);

			if(text[1]=='h')
			treshold_u=robotLettersToNumber(&text[2],NUMBER_LENGTH);

		break;
		
		case 'E':
		if(text[1]=='o')
		outside_error=robotLettersToNumber(&text[2],NUMBER_LENGTH);

		break;
	}
}

uint8_t robotLettersToNumber(char letters[], uint8_t bytes)
{
	uint8_t number;
	if(bytes==3)
		number=((letters[0]-48)*100)+((letters[1]-48)*10)+(letters[2]-48);
		
	if(bytes==2)
		number=((letters[0]-48)*10)+(letters[1]-48);
	
	return number;
}

void robotMainLoop()
{
	uint8_t sensors[7];
	int16_t y=0, u=0, up=0, ud=0;
	static int16_t prev_e=0, e=0;
	
	adcReadSensors(sensors);
	adcUpdateLedState(sensors);
	
	y=robotCalculateError(sensors);
	e=y_zad-y;
	
	//czlon proporcjonalny
	up=Kp*e;
	
	//czlon rozniczkujacy
	ud=Kd*(e-prev_e);
	prev_e=e;
	
	u=up+ud;
	
	robotSetVelocity(u,y);
}
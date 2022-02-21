/*
 * pwm.c
 *
 * Created: 2017-05-08 01:06:35
 *  Author: User
 */ 

 #include "pwm.h"

 void pwmInit()
 {
	 global_PWM_value=0;

	 TCCR1A|=(1<<WGM10)|(1<<COM1A1)|(1<<COM1B1);                //fast PWM mode (non-inverted)
	 //TCCR1A&=~(1<<WGM11)
	 TCCR1B|=(1<<WGM12)|(4<<CS10);                                      //prescaler
	 DDRD|=(1<<PWML_PIN)|(1<<PWMR_PIN);
	 //OCR1A=global_PWM_value;
	 //OCR1B=global_PWM_value;
 }
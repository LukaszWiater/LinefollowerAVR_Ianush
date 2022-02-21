/*
 * pwm.h
 *
 * Created: 2017-05-08 01:06:09
 *  Author: User
 */ 


#ifndef PWM_H_
#define PWM_H_

#include <avr/io.h>

#define PWML OCR1B	
#define PWMR OCR1A

#define PWML_PIN PD4
#define PWMR_PIN PD5

uint8_t global_PWM_value;

void pwmInit();

#endif /* PWM_H_ */
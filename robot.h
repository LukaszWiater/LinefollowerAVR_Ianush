/*
 * robot.h
 *
 * Created: 2018-03-05 23:11:47
 *  Author: wooca
 */ 


#ifndef ROBOT_H_
#define ROBOT_H_

#include <avr/io.h>
#include <util/delay.h>
#include "pwm.h"

#define AIN1 PD7
#define AIN2 PD6
#define BIN1 PC2
#define BIN2 PC3
#define FRAME_LENGTH 4
#define NUMBER_LENGTH 3

void robotBridgeInit();
void robotSwitchInit();
int robotCalculateError(uint8_t sensor_state[]);
//int robotRegPD(int error);
//void robotSetPWM(int left, int right);
void robotMessageHandler();
uint8_t robotLettersToNumber(char letters[], uint8_t bytes);
void robotSetVelocity(int16_t control_signal, int sensor_error);

void robotMainLoop();



#endif /* ROBOT_H_ */
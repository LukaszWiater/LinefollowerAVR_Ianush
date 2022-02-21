/*
 * ADC.h
 *
 * Created: 2018-03-01 01:17:52
 *  Author: wooca
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>
#include <avr/sfr_defs.h>

void adcInit();
void adcLedInit();
void adcReadSensors(uint8_t sensor_state[]);
void adcUpdateLedState(uint8_t sensor_state[]);


#endif /* ADC_H_ */
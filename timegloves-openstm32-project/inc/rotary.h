#ifndef ROTARY_H_
#define ROTARY_H_

#include "gpio.h"
#include "stm32l476xx.h"
#include "common.h"
#include "lcd.h"


int DARK;				// dark duration
#define LIGHT	200		// light duration, should be a constant


int DURATION;
int FREQUENCY;



void ROTARY_Init(void);
void pinChangeISR();
void ROTARY_EXIT_Setup();


#endif

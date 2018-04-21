#ifndef LCD_H_
#define LCD_H_

#include "gpio.h"
#include "stm32l476xx.h"
#include "common.h"


void LCD_Init(void);
void LCD_Write_Command(uint8_t RS, uint8_t RW, uint8_t DATA);
void LCD_Append_Char(char ch);
void LCD_Display_One_Line(uint8_t* line);
void LCD_Display_Two_Line(uint8_t* line1, uint8_t* line2);
void LCD_Clear();


#endif

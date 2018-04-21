#include "lcd.h"


/* LCD 1602A pins
 *
 * 1:	VSS		GND
 * 2:	VDD		5V
 * 3:	VS		GND
 * 4:	RS		PB3
 * 5:	R/W		PB4
 * 6:	E		PB5
 * 7:	DB0		PB10
 * 8:	DB1		PA8
 * 9:	DB2		PA9
 * 10:	DB3		PC7
 * 11:	DB4		PB6
 * 12:	DB5		PA7
 * 13:	DB6		PA6
 * 14:	DB7		PA5
 * 15:			5V
 * 16:			GND
 */

#define	RS_PORT		GPIOB
#define	RS_PIN		3
#define	RW_PORT		GPIOB
#define	RW_PIN		5
#define	E_PORT		GPIOB
#define	E_PIN		4
#define	DB0_PORT	GPIOB
#define DB0_PIN		10
#define	DB1_PORT	GPIOA
#define DB1_PIN		8
#define	DB2_PORT	GPIOA
#define DB2_PIN		9
#define	DB3_PORT	GPIOC
#define DB3_PIN		7
#define	DB4_PORT	GPIOB
#define DB4_PIN		6
#define	DB5_PORT	GPIOA
#define DB5_PIN		7
#define	DB6_PORT	GPIOA
#define DB6_PIN		6
#define	DB7_PORT	GPIOA
#define DB7_PIN		5



void LCD_Init(void) {
	// open GPIO clock
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);

	// set GPIO
	TM_GPIO_Init(RS_PORT, RS_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	TM_GPIO_Init(RW_PORT, RW_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	TM_GPIO_Init(E_PORT, E_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	TM_GPIO_Init(DB0_PORT, DB0_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	TM_GPIO_Init(DB1_PORT, DB1_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	TM_GPIO_Init(DB2_PORT, DB2_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	TM_GPIO_Init(DB3_PORT, DB3_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	TM_GPIO_Init(DB4_PORT, DB4_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	TM_GPIO_Init(DB5_PORT, DB5_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	TM_GPIO_Init(DB6_PORT, DB6_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);
	TM_GPIO_Init(DB7_PORT, DB7_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_OD, TM_GPIO_PuPd_UP, TM_GPIO_Speed_Medium);

	// set LCD
	LCD_Write_Command(0, 0, 0x38); // set LCD function
	LCD_Write_Command(0, 0, 0x06); // set LCD mode
	LCD_Write_Command(0, 0, 0x0C); // open LCD
}



void LCD_Write_Command(uint8_t RS, uint8_t RW, uint8_t DATA){
	TM_GPIO_SetPinLow(E_PORT, E_PIN);
	if(RS){
		TM_GPIO_SetPinHigh(RS_PORT, RS_PIN);
	} else {
		TM_GPIO_SetPinLow(RS_PORT, RS_PIN);
	}
	if(RW){
		TM_GPIO_SetPinHigh(RW_PORT, RW_PIN);
	} else {
		TM_GPIO_SetPinLow(RW_PORT, RW_PIN);
	}
	if((DATA & (1<<7)) != 0){
		TM_GPIO_SetPinHigh(DB7_PORT, DB7_PIN);
	} else {
		TM_GPIO_SetPinLow(DB7_PORT, DB7_PIN);
	}
	if((DATA & (1<<6)) != 0){
		TM_GPIO_SetPinHigh(DB6_PORT, DB6_PIN);
	} else {
		TM_GPIO_SetPinLow(DB6_PORT, DB6_PIN);
	}
	if((DATA & (1<<5)) != 0){
		TM_GPIO_SetPinHigh(DB5_PORT, DB5_PIN);
	} else {
		TM_GPIO_SetPinLow(DB5_PORT, DB5_PIN);
	}
	if((DATA & (1<<4)) != 0){
		TM_GPIO_SetPinHigh(DB4_PORT, DB4_PIN);
	} else {
		TM_GPIO_SetPinLow(DB4_PORT, DB4_PIN);
	}
	if((DATA & (1<<3)) != 0){
		TM_GPIO_SetPinHigh(DB3_PORT, DB3_PIN);
	} else {
		TM_GPIO_SetPinLow(DB3_PORT, DB3_PIN);
	}
	if((DATA & (1<<2)) != 0){
		TM_GPIO_SetPinHigh(DB2_PORT, DB2_PIN);
	} else {
		TM_GPIO_SetPinLow(DB2_PORT, DB2_PIN);
	}
	if((DATA & (1<<1)) != 0){
		TM_GPIO_SetPinHigh(DB1_PORT, DB1_PIN);
	} else {
		TM_GPIO_SetPinLow(DB1_PORT, DB1_PIN);
	}
	if((DATA & (1<<0)) != 0){
		TM_GPIO_SetPinHigh(DB0_PORT, DB0_PIN);
	} else {
		TM_GPIO_SetPinLow(DB0_PORT, DB0_PIN);
	}
	TM_GPIO_SetPinHigh(E_PORT, E_PIN);
	delay_us(10);
	TM_GPIO_SetPinLow(E_PORT, E_PIN);
	delay_us(10);
	if(RS == 0 && RW == 0 && DATA < 4){
		delay_us(1640);
	} else {
		delay_us(40);
	}
}


void LCD_Append_Char(char ch){
	LCD_Write_Command(1, 0, ch);
}


void LCD_Display_One_Line(uint8_t* line){
	LCD_Clear();
	for(int i = 0; line[i] != '\0' && i < 16; ++i){
		LCD_Append_Char(line[i]);
	}
}


void LCD_Display_Two_Line(uint8_t* line1, uint8_t* line2){
	LCD_Clear();
	for(int i = 0; line1[i] != '\0' && i < 16; ++i){
		LCD_Append_Char(line1[i]);
	}
	LCD_Write_Command(0, 0, 0xC0); // move LCD cursor to next line
	for(int i = 0; line2[i] != '\0' && i < 16; ++i){
		LCD_Append_Char(line2[i]);
	}
}


void LCD_Clear(){
	LCD_Write_Command(0, 0, 0x01); // clear screen
}



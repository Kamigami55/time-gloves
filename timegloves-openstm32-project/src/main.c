/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32l4xx.h"
#include "stm32l4xx_nucleo.h"
#include "gpio.h"
#include "common.h"
#include "lcd.h"
#include "rotary.h"


#define LIGHT_PORT	GPIOC
#define LIGHT_PIN	1

#define BUTTON_PORT	GPIOA
#define BUTTON_PIN	1

int newdark = 20000;
uint32_t oldAdcVal = 0;
uint32_t adcVal;



void SystemClock_Config(int us){
	// Enable SysTick Interrupt
	//SET_REG(SysTick->CTRL, 1, 0); 						// Disable SysTick
	SET_REG(SysTick->LOAD, 0x00FFFFFF, us/2) 			// Set reload value (4M freq / 8tic per count * us / 1M)
	//SET_REG(SysTick->VAL, 0x00FFFFFF, 0) 				// Clear counter value
	SET_REG(SysTick->CTRL, 11, 11);			 			// Enable SysTick

}

// note: I've comment out the original version of SysTick_Handler() in /inc/stm32l4xx_it.c
void SysTick_Handler(void) {
	TM_GPIO_SetPinHigh(LIGHT_PORT, LIGHT_PIN);
	delay_us(LIGHT);
	TM_GPIO_SetPinLow(LIGHT_PORT, LIGHT_PIN);
	//delay_us(newdark);
}



void LED_GPIO_Init(){
	// open GPIO clock
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);
	// set GPIO
	TM_GPIO_Init(LIGHT_PORT, LIGHT_PIN, TM_GPIO_Mode_OUT, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast);

//	// button
//	TM_GPIO_Init(BUTTON_PORT, BUTTON_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_OD, TM_GPIO_PuPd_DOWN, TM_GPIO_Speed_Fast);
}



void ConfigureADC() {
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);

	// ADC Pin
	TM_GPIO_Init(GPIOC, 0, TM_GPIO_Mode_AN, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Medium);
	MODIFY_REG(GPIOC->ASCR,GPIO_ASCR_ASC0,GPIO_ASCR_ASC0);//connect analog switch to adc input pa0 ADC1IN1

	__HAL_RCC_ADC_CLK_ENABLE();
	// Clear Deep Sleep
	CLEAR_BIT(ADC1->CR, ADC_CR_DEEPPWD);
	// Turn on Voltage Regulator
	SET_BIT(ADC1->CR, ADC_CR_ADVREGEN);
	delay_us(200);
	// Prescaler
	MODIFY_REG(ADC123_COMMON->CCR, ADC_CCR_PRESC|ADC_CCR_CKMODE, ADC_CCR_CKMODE_0);
	MODIFY_REG(ADC1->CFGR, ADC_CFGR_CONT, ADC_CFGR_CONT);
	CLEAR_BIT(ADC1->CFGR2, ADC_CFGR2_ROVSE);
	CLEAR_BIT(ADC1->SQR1, ADC_SQR1_L);
	MODIFY_REG(ADC1->SQR1, (0xF)|(0x1F<<24)|(0x1F<<18)|(0x1F<<12)|(0x1F<<6), (0x1<<6)); // Channel 1, Rank 1
	MODIFY_REG(ADC1->SMPR1, (0x3FFFFFFFFF), (0x6<<3)); // Channel 1, Sampling Time: 247.5 ADC cycles
}

void startADC() {
	while (!(ADC1->ISR & ADC_ISR_ADRDY)) ADC1->CR |= ADC_CR_ADEN; // TURN ON
	ADC1->ISR = ADC_ISR_EOC | ADC_ISR_EOS | ADC_ISR_OVR; // Clear flags
	SET_BIT(ADC1->CR, ADC_CR_ADSTART); // START CONV
}



void read_rotary() {
	startADC();
	while (!(ADC1->ISR & ADC_ISR_EOC));
	adcVal = ADC1->DR;
	if(adcVal > oldAdcVal + 10 || adcVal < oldAdcVal - 10){
		newdark = adcVal * 100000 / 4096 + 5000; // range: 10000 ~ 30000
		SystemClock_Config(newdark);
		DURATION = newdark + LIGHT;		// us
		FREQUENCY = 1000000/DURATION;	// Hz

		char buff1[16];
		char buff2[16];
		sprintf(buff1, "FREQ:%dHz", FREQUENCY);
		sprintf(buff2, "DURA:%dus", DURATION);
		LCD_Display_Two_Line(buff1, buff2);
		oldAdcVal = adcVal;
	}
}



int main(void)
{

	LCD_Init();
	LED_GPIO_Init();
	ConfigureADC();
	LCD_Display_Two_Line("  +=  Time  =+", "  += Gloves =+");
	delay_us(1000000);
	//ROTARY_Init();


	while(1){
		read_rotary();
	}
}

#include "rotary.h"

//#define ROTARY_CLK_PORT		GPIOA
//#define ROTARY_CLK_PIN		1
////#define ROTARY_CLK_PORT		GPIOB
////#define ROTARY_CLK_PIN		0
//#define ROTARY_DT_PORT		GPIOA
//#define ROTARY_DT_PIN		4
#define ROTARY_CLK_PORT		GPIOC
#define ROTARY_CLK_PIN		2
//#define ROTARY_CLK_PORT		GPIOB
//#define ROTARY_CLK_PIN		0
#define ROTARY_DT_PORT		GPIOC
#define ROTARY_DT_PIN		3

#define ROTARY_UPPER		50000
#define ROTARY_LOWER		0
#define ROTARY_STEP			10
#define ROTARY_DEFAULT		20000


int abOld;		// Initialize state
int old_count;	// old rotary count


void ROTARY_Init(void){
	// open GPIO clock
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOBEN | RCC_AHB2ENR_GPIOCEN);

	// set GPIO
	TM_GPIO_Init(ROTARY_CLK_PORT, ROTARY_CLK_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast);
	TM_GPIO_Init(ROTARY_DT_PORT, ROTARY_DT_PIN, TM_GPIO_Mode_IN, TM_GPIO_OType_PP, TM_GPIO_PuPd_NOPULL, TM_GPIO_Speed_Fast);

	// set INTERRUPT
	ROTARY_EXIT_Setup();

	abOld = DARK = old_count = ROTARY_DEFAULT;
}



void pinChangeISR() {
//	enum { upMask = 0x66, downMask = 0x99 };
//	int abNew = (TM_GPIO_GetInputPinValue(ROTARY_CLK_PORT, ROTARY_CLK_PIN) << 1) | TM_GPIO_GetInputPinValue(ROTARY_DT_PORT, ROTARY_DT_PIN);
//	int criterion = abNew^abOld;
//	if (criterion==1 || criterion==2) {
//		if (upMask & (1 << (2*abOld + abNew/2))){
//			if(DARK > ROTARY_LOWER + ROTARY_STEP)
//				DARK -= ROTARY_STEP;
//		}
//		else {
//			if(DARK < ROTARY_UPPER - ROTARY_STEP)
//				DARK += ROTARY_STEP;       // upMask = ~downMask
//		}
//	}
//	abOld = abNew;		// Save new state
//	DURATION = DARK + LIGHT;		// us
//	FREQUENCY = 1000000/DURATION;	// Hz
//
//	char buff1[16];
//	char buff2[16];
//	sprintf(buff1, "FREQ: %d", FREQUENCY);
//	sprintf(buff2, "DURA: %d", DURATION);
//	LCD_Display_Two_Line(buff1, buff2);
}



void ROTARY_EXIT_Setup(){
	//ENABLE PERIPHERAL CLOCK
	SET_REG(RCC->APB2ENR,RCC_APB2ENR_SYSCFGEN,1);

	//SYSCFG MODULE
	SET_REG(SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI2,SYSCFG_EXTICR1_EXTI2_PC);//PC0
	SET_REG(SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI3,SYSCFG_EXTICR1_EXTI3_PC);//PC1


	//EXTI module
	SET_REG(EXTI->RTSR1,EXTI_RTSR1_RT2,EXTI_RTSR1_RT2);
	SET_REG(EXTI->FTSR1,EXTI_FTSR1_FT2,EXTI_FTSR1_FT2);
	SET_REG(EXTI->IMR1,EXTI_IMR1_IM2,EXTI_IMR1_IM2);
	SET_REG(EXTI->PR1,EXTI_PR1_PIF2,EXTI_PR1_PIF2);

	SET_REG(EXTI->RTSR1,EXTI_RTSR1_RT3,EXTI_RTSR1_RT3);
	SET_REG(EXTI->FTSR1,EXTI_FTSR1_FT3,EXTI_FTSR1_FT3);
	SET_REG(EXTI->IMR1,EXTI_IMR1_IM3,EXTI_IMR1_IM3);
	SET_REG(EXTI->PR1,EXTI_PR1_PIF3,EXTI_PR1_PIF3);

	//NVIC
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_EnableIRQ(EXTI3_IRQn);

	//clear NVIC pending
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	NVIC_ClearPendingIRQ(EXTI3_IRQn);

	//set priority
	NVIC_SetPriority(EXTI2_IRQn, 3);
	NVIC_SetPriority(EXTI3_IRQn, 3);
}


void EXTI2_IRQHandler(void){
	pinChangeISR();
	NVIC_ClearPendingIRQ(EXTI2_IRQn);
	//clear pending interrupt flag of line 2
	SET_REG(EXTI->PR1,EXTI_PR1_PIF2,EXTI_PR1_PIF2);
}


void EXTI3_IRQHandler(void){
	pinChangeISR();
	NVIC_ClearPendingIRQ(EXTI3_IRQn);
	//clear pending interrupt flag of line 3
	SET_REG(EXTI->PR1,EXTI_PR1_PIF3,EXTI_PR1_PIF3);
}





//
//void ROTARY_EXIT_Setup(){
//	//ENABLE PERIPHERAL CLOCK
//	SET_REG(RCC->APB2ENR,RCC_APB2ENR_SYSCFGEN,1);
//
//	//SYSCFG MODULE
//	SET_REG(SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI1,SYSCFG_EXTICR1_EXTI1_PA);//PB0
//	SET_REG(SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI4,SYSCFG_EXTICR2_EXTI4_PA);//PA4
//
//
//	//EXTI module
//	SET_REG(EXTI->RTSR1,EXTI_RTSR1_RT1,EXTI_RTSR1_RT1);
//	SET_REG(EXTI->FTSR1,EXTI_FTSR1_FT1,EXTI_FTSR1_FT1);
//	SET_REG(EXTI->IMR1,EXTI_IMR1_IM1,EXTI_IMR1_IM1);
//	SET_REG(EXTI->PR1,EXTI_PR1_PIF1,EXTI_PR1_PIF1);
//
//	SET_REG(EXTI->RTSR1,EXTI_RTSR1_RT4,EXTI_RTSR1_RT4);
//	SET_REG(EXTI->FTSR1,EXTI_FTSR1_FT4,EXTI_FTSR1_FT4);
//	SET_REG(EXTI->IMR1,EXTI_IMR1_IM4,EXTI_IMR1_IM4);
//	SET_REG(EXTI->PR1,EXTI_PR1_PIF4,EXTI_PR1_PIF4);
//
//	//NVIC
//	NVIC_EnableIRQ(EXTI1_IRQn);
//	NVIC_EnableIRQ(EXTI4_IRQn);
//
//	//clear NVIC pending
//	NVIC_ClearPendingIRQ(EXTI1_IRQn);
//	NVIC_ClearPendingIRQ(EXTI4_IRQn);
//
//	//set priority
//	NVIC_SetPriority(EXTI1_IRQn, 3);
//	NVIC_SetPriority(EXTI4_IRQn, 3);
//}
//
//
//void EXTI1_IRQHandler(void){
//	pinChangeISR();
//	NVIC_ClearPendingIRQ(EXTI1_IRQn);
//	//clear pending interrupt flag of line 2
//	SET_REG(EXTI->PR1,EXTI_PR1_PIF1,EXTI_PR1_PIF1);
//}
//
//
//void EXTI4_IRQHandler(void){
//	pinChangeISR();
//	NVIC_ClearPendingIRQ(EXTI4_IRQn);
//	//clear pending interrupt flag of line 3
//	SET_REG(EXTI->PR1,EXTI_PR1_PIF4,EXTI_PR1_PIF4);
//}









//
//
//void ROTARY_EXIT_Setup(){
//	//ENABLE PERIPHERAL CLOCK
//	SET_REG(RCC->APB2ENR,RCC_APB2ENR_SYSCFGEN,1);
//
//	//SYSCFG MODULE
//	SET_REG(SYSCFG->EXTICR[0],SYSCFG_EXTICR1_EXTI0,SYSCFG_EXTICR1_EXTI0_PB);//PB0
//	SET_REG(SYSCFG->EXTICR[1],SYSCFG_EXTICR2_EXTI4,SYSCFG_EXTICR2_EXTI4_PA);//PA4
//
//
//	//EXTI module
//	SET_REG(EXTI->RTSR1,EXTI_RTSR1_RT0,EXTI_RTSR1_RT0);
//	SET_REG(EXTI->FTSR1,EXTI_FTSR1_FT0,EXTI_FTSR1_FT0);
//	SET_REG(EXTI->IMR1,EXTI_IMR1_IM0,EXTI_IMR1_IM0);
//	SET_REG(EXTI->PR1,EXTI_PR1_PIF0,EXTI_PR1_PIF0);
//
//	SET_REG(EXTI->RTSR1,EXTI_RTSR1_RT4,EXTI_RTSR1_RT4);
//	SET_REG(EXTI->FTSR1,EXTI_FTSR1_FT4,EXTI_FTSR1_FT4);
//	SET_REG(EXTI->IMR1,EXTI_IMR1_IM4,EXTI_IMR1_IM4);
//	SET_REG(EXTI->PR1,EXTI_PR1_PIF4,EXTI_PR1_PIF4);
//
//	//NVIC
//	NVIC_EnableIRQ(EXTI0_IRQn);
//	NVIC_EnableIRQ(EXTI4_IRQn);
//
//	//clear NVIC pending
//	NVIC_ClearPendingIRQ(EXTI0_IRQn);
//	NVIC_ClearPendingIRQ(EXTI4_IRQn);
//
//	//set priority
//	NVIC_SetPriority(EXTI0_IRQn, 3);
//	NVIC_SetPriority(EXTI4_IRQn, 3);
//}
//
//
//void EXTI0_IRQHandler(void){
//	pinChangeISR();
//	NVIC_ClearPendingIRQ(EXTI0_IRQn);
//	//clear pending interrupt flag of line 2
//	SET_REG(EXTI->PR1,EXTI_PR1_PIF0,EXTI_PR1_PIF0);
//}
//
//
//void EXTI4_IRQHandler(void){
//	pinChangeISR();
//	NVIC_ClearPendingIRQ(EXTI4_IRQn);
//	//clear pending interrupt flag of line 3
//	SET_REG(EXTI->PR1,EXTI_PR1_PIF4,EXTI_PR1_PIF4);
//}

#include "common.h"

void delay_us(int n)
{
	asm("push {r0}\r\n"
		"mov r0, r0\r\n"
		"LOOP_US:\r\n"
		"nop\r\n"
		"subs r0, #1\r\n"
		"BGT LOOP_US\r\n"
		"POP {r0}\r\n"
		:: "r" (n));
}

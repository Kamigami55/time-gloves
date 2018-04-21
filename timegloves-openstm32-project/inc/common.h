#ifndef COMMON_H_
#define COMMON_H_

#define SET_REG(REG,SELECT,VAL) {((REG)=((REG)&(~(SELECT))) | (VAL));};

void delay_us(int n);

#endif

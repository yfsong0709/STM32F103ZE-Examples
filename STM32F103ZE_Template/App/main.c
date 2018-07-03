#include "stm32f10x.h"

void delay(int t);

int main(void){
	while(1);
}

void delay(int t){
	int x,temp=10000;
	while(temp--){
		x=t;
		while(x--);
	}
}

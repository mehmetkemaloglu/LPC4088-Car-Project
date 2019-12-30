#include "External.h"
uint32_t turn = 0;

void External_Init() {
	//Change the functionality of the push button as EINT0
	uint32_t valIocon = IOCON_TURN;
	valIocon |= (1<<0);
	valIocon &=~(1<<2 | 1<<1);
	IOCON_TURN = valIocon;	
	
	//Change the External interrupt mode as Edge Sensitive
	EXT->EXTMODE |= (1<<0);
	
	//Change polarity of the External Interrupt as Low-active
	EXT->EXTPOLAR &= ~(1<<0);
	
	//Enable interrupt for EINT0_IRQn
	NVIC_EnableIRQ(EINT0_IRQn);
}

void EINT0_IRQHandler() {
	//Clear interrupt for EINT0
	EXT->EXTINT |= (1<<0);
	
	turn++;
	
}

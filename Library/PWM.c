#include "PWM.h"

void PWM0_Init() {
	uint32_t ioconRegisterValue = IOCON_FRONTLEFT_LED;
	ioconRegisterValue |= 0x3;
	ioconRegisterValue &= ~(1 << 2);
	IOCON_FRONTLEFT_LED = ioconRegisterValue;
	
	ioconRegisterValue = IOCON_FRONTRIGHT_LED;
	ioconRegisterValue |= 0x3;
	ioconRegisterValue &= ~(1 << 2);
	IOCON_FRONTRIGHT_LED = ioconRegisterValue;
	
	ioconRegisterValue = IOCON_BACKLEFT_LED;
	ioconRegisterValue |= 0x3;
	ioconRegisterValue &= ~(1 << 2);
	IOCON_BACKLEFT_LED = ioconRegisterValue;
	
	ioconRegisterValue = IOCON_BACKRIGHT_LED;
	ioconRegisterValue |= 0x3;
	ioconRegisterValue &= ~(1 << 2);
	IOCON_BACKRIGHT_LED = ioconRegisterValue;
	
	PCONP |= (1 << 5);
	
	//Enable PWM output for corresponding pin.
	PWM0->PCR |= (1<<9 | 1<<10 | 1<<11 | 1<<12);
	
	
	PWM0->TCR = 1 << 1;
	
	//Ilerde motorda degisebilir!!
	PWM0->PR = 0;
	
	PWM0->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * 20 * 1000;
	
	//Reset TC, when MR0 matches TC. Also generate an interrupt when MR0 matches the value in the TC.
	PWM0->MCR |= (1 << 1);
	
	PWM0->LER |= 1 << 0;
	
	PWM0->TCR = (1 << 0 | 1 << 3);
	
	//Clear pending for PWM0
	
	//Set Priority PWM0 IRQ as 5
	
	//Enable PWM0_IRQn (Interrupt Request)
	
	PWM0_Write(0, 1);
	PWM0_Write(0, 2);
	PWM0_Write(0, 3);
	PWM0_Write(0, 4);

}

void PWM0_Cycle_Rate(uint32_t period_In_Cycles) {
	PWM0->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * period_In_Cycles * 1000;
	
	PWM0->LER |= 1 << 0;
}

void PWM0_Write(uint32_t T_ON, uint32_t MR_NO) {	
	if(T_ON > 100) {
		T_ON = 100;
	}
	
	T_ON = (uint32_t)(((PWM0->MR0) * T_ON) / 100);
	
	if (T_ON == PWM0->MR0) {
		T_ON++;
	}
	
	switch(MR_NO) {
		
		case(1): {
			PWM0->MR1 = T_ON;
			PWM0->LER |= 1 << 1;
			break;
		}
		case(2): {
			PWM0->MR2 = T_ON;
			PWM0->LER |= 1 << 2;
			break;
		}
		case(3): {
			PWM0->MR3 = T_ON;
			PWM0->LER |= 1 << 3;
			break;
		}
		case(4): {
			PWM0->MR4 = T_ON;
			PWM0->LER |= 1 << 4;
			break;
		}
			
	}
	
}

void GO_FORWARD_LED(){
	PWM0_Cycle_Rate(20);
	PWM0_Write(100, 1);
	PWM0_Write(100, 2);
	PWM0_Write(0, 3);
	PWM0_Write(0, 4);

}

void GO_BACKWARD_LED(){
	PWM0_Cycle_Rate(20);
	PWM0_Write(0, 1);
	PWM0_Write(0, 2);
	PWM0_Write(100, 3);
	PWM0_Write(100, 4);

}
void TURN_LEFT_LED(){
	PWM0_Cycle_Rate(500);
	PWM0_Write(50, 1);
	PWM0_Write(0, 2);
	PWM0_Write(50, 3);
	PWM0_Write(0, 4);

}

void TURN_RIGHT_LED(){
	PWM0_Cycle_Rate(500);
	PWM0_Write(0, 1);
	PWM0_Write(50, 2);
	PWM0_Write(0, 3);
	PWM0_Write(50, 4);

}
void STOP_LED(){
	PWM0_Cycle_Rate(20);
	PWM0_Write(0, 1);
	PWM0_Write(0, 2);
	PWM0_Write(0, 3);
	PWM0_Write(0, 4);

}

void PWM1_Init() {
	uint32_t ioconRegisterValue = IOCON_MOTOR1_SPEED;
	ioconRegisterValue |= 0x2;
	ioconRegisterValue &= ~(1 << 2 | 1 << 0);
	IOCON_MOTOR1_SPEED = ioconRegisterValue;
	
	ioconRegisterValue = IOCON_MOTOR2_SPEED;
	ioconRegisterValue |= 0x2;
	ioconRegisterValue &= ~(1 << 2 | 1 << 0);
	IOCON_MOTOR2_SPEED = ioconRegisterValue;
	
	PCONP |= (1 << 5);
	
	//Enable PWM output for corresponding pin.
	PWM1->PCR |= (1<<13);
	
	
	PWM1->TCR = 1 << 1;
	
	//Ilerde motorda degisebilir!!
	PWM1->PR = 9;
	
	PWM1->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * 20 * 1000;
	
	//Reset TC, when MR0 matches TC. Also generate an interrupt when MR0 matches the value in the TC.
	PWM1->MCR |= (1 << 1);
	
	PWM1->LER |= 1 << 0;
	
	PWM1->TCR = (1 << 0 | 1 << 3);
	
	//Clear pending for PWM1
	
	//Set Priority PWM1 IRQ as 5
	
	//Enable PWM1_IRQn (Interrupt Request)
	
	PWM1_Write(0, 4);
	PWM1_Write(30, 5);

}

void PWM1_Cycle_Rate(uint32_t period_In_Cycles) {
	PWM1->MR0 = (PERIPHERAL_CLOCK_FREQUENCY / 1000000) * period_In_Cycles * 1000;
	
	PWM1->LER |= 1 << 0;
}

void PWM1_Write(uint32_t T_ON, uint32_t MR_NO) {	
	if(T_ON > 100) {
		T_ON = 100;
	}
	
	T_ON = (uint32_t)(((PWM1->MR0) * T_ON) / 100);
	
	if (T_ON == PWM1->MR0) {
		T_ON++;
	}
	
	if(MR_NO == 5) {
		PWM1->MR5 = T_ON;
		PWM1->LER |= 1 << 5;
	} else {
		PWM1->MR4 = T_ON;
		PWM1->LER |= 1 << 4;
	}
}



#include "GPIO.h"

void GPIO_DIR_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value) {
	if(value == 0) {
		PORT->DIR &= ~MASK;
	}
	else {
		PORT->DIR |= MASK;
	}
}

void GPIO_PIN_Write(GPIO_TypeDef* PORT,uint32_t MASK,uint8_t value) {
	if(value == 0) {
		PORT->PIN &= ~MASK;
	}
	else {
		PORT->PIN |= MASK;
	}
}

uint32_t GPIO_PIN_Read(GPIO_TypeDef* PORT,uint32_t MASK) {
	return PORT->PIN &= MASK;
}

void GPIO_Motor_Init() {
	
	uint32_t ioconRegisterValue = IOCON_MOTOR_IN1;
	ioconRegisterValue &= ~(1 << 0 | 1 << 1 | 1 << 2);
	IOCON_MOTOR_IN1 = ioconRegisterValue;
	
	ioconRegisterValue = IOCON_MOTOR_IN2;
	ioconRegisterValue &= ~(1 << 0 | 1 << 1 | 1 << 2);
	IOCON_MOTOR_IN2 = ioconRegisterValue;
	
	GPIO_DIR_Write(IN1_PORT, IN1_MASK, 1);
	GPIO_DIR_Write(IN2_PORT, IN2_MASK, 1);

}

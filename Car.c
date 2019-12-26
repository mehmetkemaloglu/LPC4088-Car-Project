#include "Car.h"

void GO_FORWARD() {
	GO_FORWARD_MOTOR();
	GO_FORWARD_LED();
}

void GO_BACKWARD() {
	GO_BACKWARD_MOTOR();
	GO_BACKWARD_LED();
}

void TURN_LEFT() {
	TURN_LEFT_MOTOR();
	TURN_LEFT_LED();
}

void TURN_RIGHT() {
	TURN_RIGHT_MOTOR();
	TURN_RIGHT_LED();
}

void STOP() {
	STOP_MOTOR();
	STOP_LED();
}

void GO_FORWARD_MOTOR() {
	PWM1_Write(30, 5);
	//	PWM1_Write(30, 4);
	
	GPIO_PIN_Write(IN1_PORT, IN1_MASK, 1);
	GPIO_PIN_Write(IN2_PORT, IN2_MASK, 0);

}

void GO_BACKWARD_MOTOR() {
	PWM1_Write(30, 5);
	//	PWM1_Write(30, 4);

	GPIO_PIN_Write(IN1_PORT, IN1_MASK, 0);
	GPIO_PIN_Write(IN2_PORT, IN2_MASK, 1);

}

void TURN_LEFT_MOTOR() {
	PWM1_Write(30, 5);
	//	PWM1_Write(30, 4);

	GPIO_PIN_Write(IN1_PORT, IN1_MASK, 0);
	GPIO_PIN_Write(IN2_PORT, IN2_MASK, 1);

}

void TURN_RIGHT_MOTOR() {
	PWM1_Write(30, 5);
	//	PWM1_Write(30, 4);

	GPIO_PIN_Write(IN1_PORT, IN1_MASK, 1);
	GPIO_PIN_Write(IN2_PORT, IN2_MASK, 0);

}

void STOP_MOTOR() {
	PWM1_Write(30, 5);
	//	PWM1_Write(30, 4);

	GPIO_PIN_Write(IN1_PORT, IN1_MASK, 0);
	GPIO_PIN_Write(IN2_PORT, IN2_MASK, 0);
}
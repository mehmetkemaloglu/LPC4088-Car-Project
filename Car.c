#include "Car.h"

uint32_t velocity;

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
	//PWM1_Write(30, 5);
	//	PWM1_Write(30, 4);
	
	GPIO_PIN_Write(IN1_PORT, IN1_MASK, 1);
	GPIO_PIN_Write(IN2_PORT, IN2_MASK, 0);
	GPIO_PIN_Write(IN3_PORT, IN3_MASK, 0);
	GPIO_PIN_Write(IN4_PORT, IN4_MASK, 1);

}

void GO_BACKWARD_MOTOR() {
	//PWM1_Write(30, 5);
	//	PWM1_Write(30, 4);

	GPIO_PIN_Write(IN1_PORT, IN1_MASK, 0);
	GPIO_PIN_Write(IN2_PORT, IN2_MASK, 1);
	GPIO_PIN_Write(IN3_PORT, IN3_MASK, 1);
	GPIO_PIN_Write(IN4_PORT, IN4_MASK, 0);

}

void TURN_LEFT_MOTOR() {
	//PWM1_Write(30, 5);
	//	PWM1_Write(30, 4);

	GPIO_PIN_Write(IN1_PORT, IN1_MASK, 0);
	GPIO_PIN_Write(IN2_PORT, IN2_MASK, 1);
	GPIO_PIN_Write(IN3_PORT, IN3_MASK, 0);
	GPIO_PIN_Write(IN4_PORT, IN4_MASK, 1);

}

void TURN_RIGHT_MOTOR() {
	//PWM1_Write(30, 5);
	//	PWM1_Write(30, 4);

	GPIO_PIN_Write(IN1_PORT, IN1_MASK, 1);
	GPIO_PIN_Write(IN2_PORT, IN2_MASK, 0);
	GPIO_PIN_Write(IN3_PORT, IN3_MASK, 1);
	GPIO_PIN_Write(IN4_PORT, IN4_MASK, 0);

}

void STOP_MOTOR() {
	//PWM1_Write(30, 5);
	//	PWM1_Write(30, 4);

	GPIO_PIN_Write(IN1_PORT, IN1_MASK, 0);
	GPIO_PIN_Write(IN2_PORT, IN2_MASK, 0);
	GPIO_PIN_Write(IN3_PORT, IN3_MASK, 0);
	GPIO_PIN_Write(IN4_PORT, IN4_MASK, 0);
}

uint32_t velocity_Calculator(uint32_t adc) {
	uint32_t vel = 0;
	if(adc < 0x34) {
		vel = 0;
	} else if(adc > 0xF00) {
		vel = 90;
	} else {
		vel = (adc -0x34) / ((0xF00 - 0x34)/90);
	}
	return vel;
}

void change_velocity(uint32_t new_velocity) {
	if(new_velocity <= velocity - 5 || new_velocity >= velocity + 5){
		velocity = new_velocity;
		PWM1_Write(velocity, 4);
		PWM1_Write(velocity, 5);
	}
	
}

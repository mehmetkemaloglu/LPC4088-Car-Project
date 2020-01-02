#ifndef CAR_H
#define CAR_H

#include "LPC407x_8x_177x_8x.h"
#include "Library/PWM.h"
#include "Library/GPIO.h"
typedef enum {
	STOP_ = 0,
	FORWARD_ = 1,
	LEFT_ = 2,
	RIGHT_ = 3,
	BACKWARD_ = 4
} Car_Direction;

typedef enum {
	MOVE_ = 0,
	FIX_ = 1
} Auto_Status;


extern uint32_t velocity;

void Car_Init();

void GO_FORWARD();
void GO_BACKWARD();
void TURN_LEFT();
void TURN_RIGHT();
void STOP();

void GO_FORWARD_MOTOR();
void GO_BACKWARD_MOTOR();
void TURN_LEFT_MOTOR();
void TURN_RIGHT_MOTOR();
void STOP_MOTOR();

void MOVE_AUTO(uint32_t leftMotor, uint32_t rightMotor);


uint32_t velocity_Calculator(uint32_t adc);
void change_velocity(uint32_t new_velocity);

#endif
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


#endif
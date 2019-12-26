#include "LPC407x_8x_177x_8x.h"
#include "Library/GPIO.h"
#include "Library/PWM.h"
#include "Library/Joystick.h"
#include "Library/External.h"
#include "Library/LED.h"
#include "Car.h"

void init() {
	//Joystick_Init();
	PWM0_Init();
	PWM1_Init();
	GPIO_Motor_Init();
	STOP();
	
	//External_Init();
	
	//PWM0_Write(0,1);
}

//void EINT0_IRQHandler() ;
	//Clear interrupt for EINT0
	




//void PWM0_IRQHandler() ;
	//Clear the interrupt.
	

void update() {
	
}
 
int main() {
    init();
 
    while(1) {
        update();
    }
}

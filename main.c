#include "LPC407x_8x_177x_8x.h"
#include "Library/GPIO.h"
#include "Library/PWM.h"
#include "Library/Joystick.h"
#include "Library/External.h"
#include "Library/LED.h"
#include "Car.h"
#include "Library/ADC.h"
#include "Library/Timer.h"
#include "Library/Ultrasonic.h"
uint32_t status;
uint32_t lastStatus;
uint32_t mode;

void init() {
	//Joystick_Init();
	PWM0_Init();
	PWM1_Init();
	GPIO_Motor_Init();
	GO_FORWARD();
	status = FORWARD_;
	
	ADC_Init();
	
	Timer_Init();
	
	ADC_Start();
	Ultrasonic_Init();
	Ultrasonic_Trigger_Timer_Init();
	Ultrasonic_Capture_Timer_Init();
	
	Ultrasonic_Start_Trigger_Timer();
	
	mode = 1;
	//External_Init();
	
	//PWM0_Write(0,1);
}

//void EINT0_IRQHandler() ;
	//Clear interrupt for EINT0
	




//void PWM0_IRQHandler() ;
	//Clear the interrupt.
	

void updateTest() {
	if(ADC_New_Data_Available == 1){
		if(ADC_GetLastValue() < 0x500){			
			if(status != STOP_){
				STOP();
				lastStatus = status;
				status = STOP_;
			}
		}
		else if(status == STOP_){
			if(lastStatus == FORWARD_){
				GO_FORWARD();
				status = FORWARD_;
			}
			else if(lastStatus == LEFT_){
				TURN_LEFT();
				status = LEFT_;
			}
			else if(lastStatus == RIGHT_){
				TURN_RIGHT();
				status = RIGHT_;
			}
			else if(lastStatus == BACKWARD_){
				GO_BACKWARD();
				status = BACKWARD_;
			}
		}
	}
	
}

void updateAuto() {
	if(ultrasonicSensorNewDataAvailable == 1){
		ultrasonicSensorDistance = (ultrasonicSensorRisingCaptureTime - ultrasonicSensorFallingCaptureTime) / 58;
		if(ultrasonicSensorDistance < 15){
			TURN_LEFT();
		}
		else if(ultrasonicSensorDistance > 35){
			TURN_RIGHT();
		}
		ultrasonicSensorNewDataAvailable = 0;
	}else{
		GO_FORWARD();
	}
	
}
 
int main() {
    init();
 
    while(1) {
			if(mode == 0){
				updateTest();
			}
			else{
				updateAuto();
			}
        
    }
}

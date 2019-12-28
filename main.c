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
#include "Library/Serial.h"
#include "Library/HM10.h"
uint32_t status;
uint32_t lastStatus;


void init() {
	
	PWM0_Init();
	PWM1_Init();
	GPIO_Motor_Init();
	GO_FORWARD();
	status = FORWARD_;
	
	ADC_Init();
	
	Timer_Init();
	
	ADC_Start();
	HM10_Init();
	/*Ultrasonic_Init();
	Ultrasonic_Trigger_Timer_Init();
	Ultrasonic_Capture_Timer_Init();
	
	Ultrasonic_Start_Trigger_Timer();
	
	Serial_Init();*/
	
	//
	serialSelectedCommand = SERIAL_TEST;
	
	selectedMode = MODE_TEST;
	
	//External_Init();
	
	//PWM0_Write(0,1);
}

//void EINT0_IRQHandler() ;
	//Clear interrupt for EINT0
	




//void PWM0_IRQHandler() ;
	//Clear the interrupt.
	

void updateTest() {
	if(serialSelectedCommand == SERIAL_FORWARD){
		if(status != FORWARD_){
			GO_FORWARD();
			status = FORWARD_;
		}
	}
	else if(serialSelectedCommand == SERIAL_BACK){
		if(status != BACKWARD_){
			GO_BACKWARD();
			status = BACKWARD_;
		}
	}
	else if(serialSelectedCommand == SERIAL_STOP){
		if(status != STOP_){
			STOP();
			status = STOP_;
		}
	}
	else if(serialSelectedCommand == SERIAL_LEFT){
		if(status != LEFT_){
			TURN_LEFT();
			status = LEFT_;
		}
	}
	else if(serialSelectedCommand == SERIAL_RIGHT){
		if(status != RIGHT_){
			TURN_RIGHT();
			status = RIGHT_;
		}
	}
	
	if(ADC_New_Data_Available == 1){
		if(ADC_GetLastValue() < 0x600){			
			if(status != STOP_){
				STOP();
				lastStatus = status;
				status = STOP_;
				serialSelectedCommand = SERIAL_ELSE;
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
			if(selectedMode == MODE_TEST){
				updateTest();
			}
			else if(selectedMode == MODE_AUTO){
				updateAuto();
			}
        
    }
}



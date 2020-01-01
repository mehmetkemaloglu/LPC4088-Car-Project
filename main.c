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
#include "Library/External.h"
#include "stdlib.h"

uint32_t lastStatus;
uint32_t auto_status;
uint32_t lastDistance;
uint32_t lastTurn;
uint8_t isAuto = 0;
uint32_t isFixStart = 0;
uint32_t isTurning = 0;
uint32_t isHardTurning = 0;
uint32_t LDRStop = 0;
uint32_t status = 0;


void init() {
	
	PWM0_Init();
	PWM1_Init();
	GPIO_Motor_Init();
	STOP();
	status = STOP_;
	

	ADC_Init();
	
	Timer_Init();
	
	ADC_Start();
	HM10_Init();
	External_Init();
	
	Ultrasonic_Init();
	Ultrasonic_Trigger_Timer_Init();
	Ultrasonic_Capture_Timer_Init();
	
	Ultrasonic_Start_Trigger_Timer();
	
	//Serial_Init();
	
	//
	serialSelectedCommand = SERIAL_TEST;
	
	selectedMode =MODE_TEST;
	
	auto_status = MOVE_;
	
	//External_Init();
	
	//PWM0_Write(0,1);
}

//void EINT0_IRQHandler() ;
	//Clear interrupt for EINT0
	




//void PWM0_IRQHandler() ;
	//Clear the interrupt.

void autoInit() {
	isAuto = 1;
	turn = 0;
	status = STOP_;
	serialSelectedCommand = SERIAL_STOP;
	STOP();
}
	

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
		STOP();
		status = STOP_;
		
	}
	else if(serialSelectedCommand == SERIAL_LEFT){
		if(status != LEFT_){
			TURN_LEFT();
			status = LEFT_;
			turn = 0;
		}
		if(turn >= 6){
			STOP();
			status = STOP_;
			serialSelectedCommand = SERIAL_STOP;
		}
	}
	else if(serialSelectedCommand == SERIAL_RIGHT){
		if(status != RIGHT_){
			TURN_RIGHT();
			status = RIGHT_;
			turn = 0;
		}
		if(turn >= 6){
			STOP();
			status = STOP_;
			serialSelectedCommand = SERIAL_STOP;
		}
	}
	
	if(ADC_New_Data_Available == 1){
		uint32_t* adc = ADC_GetLastValue();
		if(adc[2] < 0x600 || adc[3] < 0x600){			
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
		change_velocity(velocity_Calculator(adc[4]));
	}
	
	
}

void updateAuto() {
	if(!isAuto) {
		autoInit();
	}
	if(ultrasonicSensorNewDataAvailable == 1){
		ultrasonicSensorDistance = (ultrasonicSensorFallingCaptureTime - ultrasonicSensorRisingCaptureTime) / 58;
		ultrasonicSensorNewDataAvailable = 0;
	}
	
	 if(serialSelectedCommand == SERIAL_START){
		 if(ultrasonicSensorDistance > 400){
			 return;
		 }
		if( ultrasonicSensorDistance < 15 && !isTurning && LDRStop == 0 ) {
			if(turn >= 1 && status != RIGHT_){
						TURN_RIGHT();
						status = RIGHT_;
						turn = 0;
						isHardTurning = 1;
			}
			if(turn >= 2){
				turn = 0;
				auto_status = MOVE_;
				status= FORWARD_;
				GO_FORWARD();
				isHardTurning = 0;
			}
		}
		else if(ultrasonicSensorDistance > 35 && !isTurning && LDRStop == 0) {
			if(turn >= 1 && status != LEFT_){
						TURN_LEFT();
						status = LEFT_;
						turn = 0;
						isHardTurning = 1;
			}
			if(turn >= 2){
				turn = 0;
				auto_status = MOVE_;
				status= FORWARD_;
				GO_FORWARD();
				isHardTurning = 0;
			}
		}	
		else if (!isHardTurning && LDRStop == 0) { 
			if(auto_status == MOVE_){
				if(status != FORWARD_){
					lastDistance = ultrasonicSensorDistance;
					GO_FORWARD();
					status = FORWARD_;
				}
				if(turn >= 1) { 
					auto_status = FIX_;
					isFixStart = 1;
				}
			}
			else{
				int32_t diff;
				if(isFixStart) {
					diff = ultrasonicSensorDistance - lastDistance;
					isFixStart = 0;
				}
				
				if(abs(diff) < 3) {
					if(status != FORWARD_){
						GO_FORWARD();
						status = FORWARD_;
					}
				} else if(abs(diff) < 5) {
					if(diff > 0) {
						if(status != LEFT_){
							TURN_LEFT();
							status = LEFT_;
							turn = 0;
							isTurning = 1;
						}
						if(turn >= 3){
							turn = 0;
							auto_status = MOVE_;
							isTurning = 0;
						}
					} else {
						if(status != RIGHT_){
							TURN_RIGHT();
							status = RIGHT_;
							turn = 0;
							isTurning = 1;
						}
						if(turn >= 3){
							turn = 0;
							auto_status = MOVE_;
							isTurning = 0;
						}
					}
				} else if(abs(diff) > 15) {
					if(diff > 0) {
						if(status != LEFT_){
							TURN_LEFT();
							status = LEFT_;
							turn = 0;
							isTurning = 1;
						}
						if(turn >= 6){
							turn = 0;
							auto_status = MOVE_;
							isTurning = 0;
						}
					} else {
						if(status != RIGHT_){
							TURN_RIGHT();
							status = RIGHT_;
							turn = 0;
							isTurning = 1;
						}
						if(turn >= 6){
							turn = 0;
							auto_status = MOVE_;
							isTurning = 0;
						}
					}
				}
			}
		}
	
		if(ADC_New_Data_Available == 1){
			uint32_t* adc = ADC_GetLastValue();
			if(adc[2] < 0x600 || adc[3] < 0x600){			
				if(status != STOP_){
					STOP();
					lastStatus = status;
					status = STOP_;
					LDRStop = 1;
				}
			}
			else if(LDRStop == 1){
				LDRStop = 0;
			}
		//change_velocity(velocity_Calculator(adc[4]));
		}
		
	}
}

int main() {
    init();
 
    while(1) {
			HM10_ReadCommand();
			if(selectedMode == MODE_TEST){
				isAuto = 0;
				updateTest();
			}
			else if(selectedMode == MODE_AUTO){
				updateAuto();
			}
        
    }
}



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
uint8_t isTest = 0;
uint32_t isFixStart = 0;
uint32_t isTurning = 0;
uint32_t isHardTurning = 0;
uint32_t LDRStop = 0;
uint32_t status = 0;
uint32_t mehmetMode = 0;//2 35ten büyükler|1 15ten küçükler|3 arasi|0 hiçbiri
uint32_t turnNumber = 0;//20-30 arasi yapilcak özel adim için turn sayaci 
uint32_t mehmetFlag = 0;
char* message;
uint8_t autoFirstTime = 0;


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
	mehmetMode=3;
	PWM1_Write(50,4);
	PWM1_Write(50,5);
	
	autoFirstTime = 0;
	
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
		if(adc[2] < 2400 || adc[3] < 2400){			
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

void stopCar(){
	STOP();
	int i=0;
	for (int i=0;i<10000;i++);
	
}


void updateAuto() {
	if(isAuto == 0) {
		autoInit();
	}
	if(serialSelectedCommand == SERIAL_START){
		if (LDRStop == 0){
			if(autoFirstTime == 0 && ultrasonicSensorNewDataAvailable == 0){
				autoFirstTime = 1;
				if(mehmetFlag != 11) {
						MOVE_AUTO(40, 40);
						mehmetFlag = 11;
					}

			}
			else if(ultrasonicSensorNewDataAvailable == 1){
				ultrasonicSensorDistance = (ultrasonicSensorFallingCaptureTime - ultrasonicSensorRisingCaptureTime) / 58;
				ultrasonicSensorNewDataAvailable = 0;
				/*sprintf(message, "%s%d%s", "distance", ultrasonicSensorDistance, "\r\n");
				HM10_Write(message);
				message = "";*/
		
			
								
				if(ultrasonicSensorDistance < 18) {
					if(mehmetFlag != 1) {
						MOVE_AUTO(60, 30);
						mehmetFlag = 1;
					}
					
				} else if (ultrasonicSensorDistance < 21) {
					if(mehmetFlag != 2) {
						MOVE_AUTO(60, 40);
						mehmetFlag = 2;
					}
				} else if (ultrasonicSensorDistance < 26) {
					if(mehmetFlag != 3) {
						MOVE_AUTO(30, 65);
						mehmetFlag = 3;
					}
				} else if (ultrasonicSensorDistance < 28) {
					if(mehmetFlag != 9) {
						MOVE_AUTO(25, 65);
						mehmetFlag = 9;
					}
				} else if (ultrasonicSensorDistance < 30) {
					if(mehmetFlag != 7) {
						MOVE_AUTO(20, 65);
						mehmetFlag = 7;
					}
				} else if (ultrasonicSensorDistance < 32) {
					if(mehmetFlag != 4) {
						MOVE_AUTO(20, 60);
						mehmetFlag = 4;
					}
				} else if (ultrasonicSensorDistance < 38) {
					if(mehmetFlag != 8) {
						MOVE_AUTO(20, 55);
						mehmetFlag = 8;
					}
				} else if (ultrasonicSensorDistance < 45) {
					if(mehmetFlag != 5) {
						MOVE_AUTO(15, 55);
						mehmetFlag = 5;
					}
				} else if(ultrasonicSensorDistance <400){
					
					if(mehmetFlag != 6) {
						MOVE_AUTO(15, 60);
						mehmetFlag = 6;
					}
				}
				else return;
			}
	}
			
		
	
		if(ADC_New_Data_Available == 1){
				uint32_t* adc = ADC_GetLastValue();
				if(adc[2] < 2400 || adc[3] < 2400){			
					STOP();
					if(LDRStop == 0 && serialSelectedCommand == SERIAL_START){
						HM10_Write("FINISH\r\n");
					}
					LDRStop = 1;
					
				}
				else {
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
				if (isTest==0){//testInit
					isTest=1;
					status=STOP_;
					STOP();
				}
				isAuto = 0;
				updateTest();
			}
			else if(selectedMode == MODE_AUTO){
				isTest = 0;
				updateAuto();
			}
        
    }
}



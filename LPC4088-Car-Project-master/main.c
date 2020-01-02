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
	mehmetMode=3;
	PWM1_Write(50,4);
	PWM1_Write(50,5);
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
	if (LDRStop == 0){
		if(serialSelectedCommand == SERIAL_START){
			if(ultrasonicSensorDistance > 100){
				return;
			}
			if(( ultrasonicSensorDistance < 18 && !isHardTurning) || mehmetMode==1) {  //4-1-4 uzaklasir
				if(turn < 4  && status != RIGHT_){
					TURN_RIGHT();
					status = RIGHT_;
					turn = 0;
					isHardTurning = 1;
					mehmetMode=1;
				}
				if(turn == 4 && status != FORWARD_){
					status= FORWARD_;
					GO_FORWARD();			
				}
				if(turn == 5 && status != LEFT_){
					status=LEFT_;
					TURN_LEFT();
				}
				if(turn >= 9){
					isHardTurning = 0;
					turn = 0;
					mehmetMode=0;	
					status=BACKWARD_;
				}
			} 
			else if((ultrasonicSensorDistance < 75 && ultrasonicSensorDistance > 32 && !isHardTurning)|| mehmetMode==2) { //4-1-4 yaklasir
				if(turn < 4 && status != LEFT_){
						TURN_LEFT();
						status = LEFT_;
						turn = 0;
						isHardTurning = 1;
						mehmetMode=2;
				}
				if(turn == 4 && status != FORWARD_){
					status= FORWARD_;
					GO_FORWARD();			
				}
				if(turn == 5 && status != RIGHT_){
					status= RIGHT_;
					TURN_RIGHT();
				}
				if(turn >= 9){
					isHardTurning = 0;
					turn = 0;
					mehmetMode=0;
					status=BACKWARD_;
				}
			}
			else if((!isHardTurning && ultrasonicSensorDistance <= 32 && ultrasonicSensorDistance >= 18)|| mehmetMode==3  ) { //caseler gelistirilmeli
				
				if(turn<1 && status != FORWARD_){
					if (ultrasonicSensorDistance>100){
						return;
					}
					status = FORWARD_;
					
					lastDistance = ultrasonicSensorDistance;
					GO_FORWARD();
					status = FORWARD_;
					turn=0;
					isHardTurning=1;
					mehmetMode=3;
				}
				if(turn == 1 && status != LEFT_) { 
					if (ultrasonicSensorDistance>100){
						return;
					}
					status=LEFT_;	
					//here we go logic
					
					if (lastDistance>=ultrasonicSensorDistance ){
						uint32_t diff = lastDistance - ultrasonicSensorDistance ;
						if (diff >= 6){
							TURN_RIGHT();
							turnNumber=3;
						}
						else if (diff >= 5){
							TURN_RIGHT();
							turnNumber=2;
						}
						else if (diff >= 3){
							TURN_RIGHT();
							turnNumber=1;
						}
						else if (diff >= 1){
							GO_FORWARD();
							turnNumber=1;
						}
					}
					else{
						uint32_t diff = ultrasonicSensorDistance - lastDistance;
						if (diff >= 6){
							TURN_LEFT();
							turnNumber=3;
						}
						else if (diff >= 5){
							TURN_LEFT();
							turnNumber=2;
						}
						
						else if (diff >= 3){
							TURN_LEFT();
							turnNumber=1;
						}
						else if (diff >= 1){
							GO_FORWARD();
							turnNumber=1;
						}
					}
					
				}
				if (turn>=(2+turnNumber) && status == LEFT_){	
					isHardTurning = 0;
					turn = 0;
					mehmetMode=0;
					status=BACKWARD_;		
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
}

int main() {
    init();
 
    while(1) {
			HM10_ReadCommand();
			if(selectedMode == MODE_TEST){
				if (isTest==0){//testInit
					isTest=1;
					status=STOP_;
				}
				isAuto = 0;
				updateTest();
			}
			else if(selectedMode == MODE_AUTO){
				isTest = 1;
				updateAuto();
			}
        
    }
}



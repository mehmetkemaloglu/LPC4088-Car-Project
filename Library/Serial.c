#include "Serial.h"
#include "../Car.h"
#include <string.h>

char serialReceivedCharacter = 0;
uint8_t serialNewDataAvailable = 0;
char* serialTransmitData = 0;
uint8_t serialTransmitCompleted = 0;
uint32_t size = 0;
uint32_t serialSelectedCommand = 0;
uint32_t selectedMode = 0;
char characters[128];

void Serial_Init() {
	//Change the function of TX and RX pins for UART.
	uint32_t ioconRegisterValue = Serial_UART_TX_PIN;
	ioconRegisterValue &= ~(1<< 1 | 1<<2);
	ioconRegisterValue |= (1<<0);
	Serial_UART_TX_PIN = ioconRegisterValue;
	
	ioconRegisterValue = Serial_UART_RX_PIN;
	ioconRegisterValue &= ~(1<< 1 | 1<<2);
	ioconRegisterValue |= (1<<0);
	Serial_UART_RX_PIN = ioconRegisterValue;
	
	//Turn on UART0.
	PCONP |= (1<<3);
	
	//Enable FIFO for UART0.
	Serial_UART->FCR |= (1<<0);
	
	//In order to change the DLM, DLL and FDR values, Write correct code for enabling the access to Divisor Latches.
	Serial_UART->LCR |= (1<<7);
	
	//Write correct DLM, DLL and FDR values for 9600 baudrate
	Serial_UART->DLM = 0x01;
	Serial_UART->DLL = 0x25;
	Serial_UART->FDR = 0x01 << 0 | 0x03 << 4;
	
	//Write correct code for disabling the access to Divisor Latches.
	Serial_UART->LCR &= ~(1<<7);
	
	//Change LCR register value for 8-bit character transfer, 1 stop bits and Even Parity.
	uint32_t tmpLcr = Serial_UART->LCR;
	tmpLcr |= (1<<0 | 1<< 1 | 1<< 3 | 1<<4);
	tmpLcr &= ~(1<< 2 | 1<<5);
	Serial_UART->LCR = tmpLcr;
							
	//Enable the Receive Data Available and THRE Interrupt.
	Serial_UART->IER |= (1<< 0 | 1<< 1);
	
	//Enable UART0_IRQn Interrupt.
	NVIC_EnableIRQ(UART0_IRQn);
	
	//Set UART0_IRQn Priority to 5.
	NVIC_SetPriority(UART0_IRQn, 5);
	
	write_Characters("TESTING\r\n");

}

void UART0_IRQHandler() {	
	uint32_t currentInterrupt = ((Serial_UART->IIR/2)%8);
	
	//First if statement is for Receive Data Available interrupt.
	//When Receive Data Available, the code will enter to the first if condition.
	if(currentInterrupt == 0x2) {
		serialReceivedCharacter = Serial_ReadData();
		serialNewDataAvailable = 1;
	}
	//Second if statement is for THRE interrupt
	//When THRE interrupt is handled, the code will enter to the second if condition.
	else if(currentInterrupt == 0x1) {
		if(*serialTransmitData > 0) {
			Serial_WriteData(*serialTransmitData++);
		}
		else {
			serialTransmitCompleted = 1;
		}
	}
}

char Serial_ReadData() {
	return Serial_UART->RBR;
}

void Serial_WriteData(const char data) {
	serialTransmitCompleted = 0;
	Serial_UART->THR = data;
}

void Serial_ReadCommand(){
	while(1){
		if(serialNewDataAvailable == 1){
			serialNewDataAvailable = 0;
			if(serialReceivedCharacter == 13){
				Serial_DetectCommand(characters);
				characters[size++] = 10;
				characters[size++] = 13;
				serialTransmitData = &characters[0];
				// auto ve status kontrol
				if(serialSelectedCommand != SERIAL_ELSE){
					Serial_WriteData(*serialTransmitData++);
					while(!serialTransmitCompleted);
				}
				int i = 0;
				for( i = 0; i<128;i++){
					characters[i] = 0;
				}
				size = 0;
				
				if(serialSelectedCommand == SERIAL_AUTO){
					selectedMode = MODE_AUTO;
					write_Characters("AUTONOMOUS\r\n");
				}
				else if(serialSelectedCommand == SERIAL_TEST){
					selectedMode = MODE_TEST;
					write_Characters("TESTING\r\n");
				}
				else if(serialSelectedCommand == SERIAL_STATUS){
					
				}
				break;
			}
			else{
				characters[size++] = serialReceivedCharacter;
			}
		}
	}
}

void Serial_DetectCommand(char * chars){
	if(strcmp(chars, "START\r\n") == 0){
		serialSelectedCommand = SERIAL_START;
	}
	else if(strcmp(chars, "FINISH\r\n") == 0){
		serialSelectedCommand = SERIAL_FINISH;
	}
	else if(strcmp(chars, "FORWARD\r\n") == 0){
		serialSelectedCommand = SERIAL_FORWARD;
	}
	else if(strcmp(chars, "LEFT\r\n") == 0){
		serialSelectedCommand = SERIAL_LEFT;
	}
	else if(strcmp(chars, "RIGHT\r\n") == 0){
		serialSelectedCommand = SERIAL_RIGHT;
	}
	else if(strcmp(chars, "STOP\r\n") == 0){
		serialSelectedCommand = SERIAL_STOP;
	}
	else if(strcmp(chars, "STATUS\r\n") == 0){
		serialSelectedCommand = SERIAL_STATUS;
	}
	else if(strcmp(chars, "AUTO\r\n") == 0){
		serialSelectedCommand = SERIAL_AUTO;
		status = STOP_;
	}
	else if(strcmp(chars, "BACK\r\n") == 0){
		serialSelectedCommand = SERIAL_BACK;
	}
	else if(strcmp(chars, "TEST\r\n") == 0){
		serialSelectedCommand = SERIAL_TEST;
		status = STOP_;
		PWM1_Write(50, 4);
		PWM1_Write(50, 5);
	}
	else{
		serialSelectedCommand = SERIAL_ELSE;
	}
	
}

void write_Characters(char * character){
	serialTransmitData = character;
	Serial_WriteData(*serialTransmitData++);
	while(!serialTransmitCompleted);
}



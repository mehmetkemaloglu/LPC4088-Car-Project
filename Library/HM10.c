#include "HM10.h"
#include "ADC.h"
#include "Ultrasonic.h"

uint8_t HM10CurrentBufferIndex = 0;
uint8_t HM10NewDataAvailable = 0;
uint8_t HM10EndOfLine = 0;

char HM10Buffer[HM10BufferSize];

void HM10_Init() {	
	HM10_UART_TX_PIN |= 0x02;
	HM10_UART_RX_PIN |= 0x02;
	
	PCONP |= 1 <<  25;
	
	HM10_UART->FCR =	1 << 0
						 |	0 << 1
						 |	0 << 2
						 |	0 << 6;
	
	HM10_UART->LCR |= (1 << 7);
	
	//Write correct DLM, DLL and FDR values for 9600 baudrate
	HM10_UART->DLM = 0x01;
	HM10_UART->DLL = 0x25;
	HM10_UART->FDR = 0x01 << 0 | 0x03 << 4;


	HM10_UART->LCR &= ~(1 << 7);
	
	HM10_UART->LCR =	3 << 0
							| 0 << 2
							| 0 << 3
							| 0 << 4;
	
	//Enable the Receive Data Available Interrupt.
	HM10_UART->IER |= ( 1<< 0);
	
	//Enable UART3_IRQn Interrupt.
	NVIC_EnableIRQ(UART3_IRQn);
	
	//HM10_Write("AT+NAMEpitCrew\r\n");
	
	//HM10_Write("AT+RESET\r\n");
	
	HM10_Write("TESTING\r\n");
	
	NVIC_SetPriority(UART3_IRQn, 6);
 NVIC_ClearPendingIRQ(UART3_IRQn);
}

void HM10_SendCommand(char* command) {
	HM10_Write(command);
}

void HM10_ClearBuffer() {
	int i = 0;
	for(i = 0; i< HM10CurrentBufferIndex ;i ++){
		HM10Buffer[i] = 0;
	}
	HM10CurrentBufferIndex = 0;
}

char HM10_ReadData() {
	while (!(HM10_UART->LSR & 0x01));
	return HM10_UART->RBR;
}

void HM10_WriteData(char data) {
	while (!(HM10_UART->LSR & 0x20));
	HM10_UART->THR = data;
}

void HM10_Write(char* data) {
	while(*data > 0)  {
		HM10_WriteData(*data++);
	}
}

void UART3_IRQHandler() {
	char data;
	data = HM10_ReadData();
	HM10NewDataAvailable = 1;
	HM10Buffer[HM10CurrentBufferIndex] = data;
	HM10CurrentBufferIndex++;
	HM10Buffer[HM10CurrentBufferIndex] = '\0';
	//HM10_ReadCommand();
		
}


void HM10_ReadCommand(){
	if(HM10NewDataAvailable == 1){
		
		Serial_DetectCommand(HM10Buffer);
		
		//HM10EndOfLine = 0;
		
		// auto ve status kontrol
		if(serialSelectedCommand != SERIAL_ELSE){
			HM10_Write(HM10Buffer);
			HM10NewDataAvailable = 0;
			HM10_ClearBuffer();
		}
				
		if(serialSelectedCommand == SERIAL_AUTO){
			selectedMode = MODE_AUTO;
			HM10_Write("AUTONOMOUS\r\n");
		}
		else if(serialSelectedCommand == SERIAL_TEST){
			selectedMode = MODE_TEST;
			HM10_Write("TESTING\r\n");
		}
		else if(serialSelectedCommand == SERIAL_STATUS){
			char status[1024];
			char* op_mode;
			if(selectedMode == MODE_AUTO) {
				op_mode = "AUTO";
			} else {
				op_mode = "TEST";
			}
			uint32_t ultrasonicSensorDistance = (ultrasonicSensorFallingCaptureTime - ultrasonicSensorRisingCaptureTime) / 58;
			sprintf(status, "%s%d%s%d%s%d%s%s%s", "{\"distance\":", ultrasonicSensorDistance,",\"light_level_left\":", ADC_Last_Value[2]/4, ",\"light_level_right\":", ADC_Last_Value[3]/4, ",\"op_mode\":\"", op_mode, "\"}\r\n");
			HM10_Write(status);
		}
	}
}


#ifndef SERIAL_H
#define SERIAL_H

#include "LPC407x_8x_177x_8x.h"

#include "SystemStructures.h"

#pragma anon_unions

typedef struct
{
	union
	{
		volatile  uint8_t  RBR;
		volatile  uint8_t  THR;
		volatile	uint8_t  DLL;
							uint32_t RESERVED0;
	};
	union
	{
		volatile	uint8_t  DLM;
		volatile	uint32_t IER;
	};
	union
	{
		volatile  uint32_t IIR;
		volatile  uint8_t  FCR;
	};
	volatile	uint8_t  LCR;
						uint8_t  RESERVED1[7];
	volatile  uint8_t  LSR;
						uint8_t  RESERVED2[7];
	volatile	uint8_t  SSCR;
						uint8_t  RESERVED3[3];
	volatile	uint32_t ACR;
	volatile	uint8_t  ICR;
						uint8_t  RESERVED4[3];
	volatile	uint8_t  FDR;
						uint8_t  RESERVED5[7];
	volatile	uint8_t  TER;
						uint8_t  RESERVED8[27];
	volatile	uint8_t  RS485CTRL;
						uint8_t  RESERVED9[3];
	volatile	uint8_t  ADRMATCH;
						uint8_t  RESERVED10[3];
	volatile	uint8_t  RS485DLY;
						uint8_t  RESERVED11[3];
	volatile  uint8_t  FIFOLVL;
}UART_TypeDef;

typedef enum {
	SERIAL_TEST = 0,
	SERIAL_LEFT = 1,
	SERIAL_RIGHT = 2,
	SERIAL_FORWARD = 3,
	SERIAL_START = 4,
	SERIAL_FINISH = 5,
	SERIAL_BACK = 6,
	SERIAL_AUTO = 7,
	SERIAL_STATUS = 8,
	SERIAL_STOP = 9,
	SERIAL_ELSE = 10
} Serial_Command;

typedef enum {
	MODE_TEST = 0,
	MODE_AUTO = 1
} Serial_Mode;


//Write the base address of the UART0.
#define Serial_UART_BASE	0x4000C000
#define Serial_UART	((UART_TypeDef*) Serial_UART_BASE)

//Write the IOCON address of TX Pin
#define Serial_UART_TX_PIN_ADDRESS	0x4002C008
#define Serial_UART_TX_PIN	*((volatile uint32_t*)(Serial_UART_TX_PIN_ADDRESS))

//Write the IOCON address of PX Pin
#define Serial_UART_RX_PIN_ADDRESS	0x4002C00C
#define Serial_UART_RX_PIN	*((volatile uint32_t*)(Serial_UART_RX_PIN_ADDRESS))

extern char serialReceivedCharacter;
extern uint8_t serialNewDataAvailable;
extern char* serialTransmitData;
extern uint8_t serialTransmitCompleted;
extern uint32_t serialSelectedCommand;
extern uint32_t selectedMode;
extern uint32_t status;
extern uint32_t statusCame;

void Serial_Init(void);
char Serial_ReadData(void);
void Serial_WriteData(char data);
void Serial_ReadCommand();
void Serial_DetectCommand(char * chars);
void write_Characters(char * character);

#endif

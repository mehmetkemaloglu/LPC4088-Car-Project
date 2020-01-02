#ifndef ADC_H
#define ADC_H

#include "LPC407x_8x_177x_8x.h"

#include "SystemStructures.h"

typedef struct {
  volatile	uint32_t CR;
  volatile	uint32_t GDR;
						uint32_t RESERVED0;
  volatile	uint32_t INTEN;
  volatile	uint32_t DR[8];
  volatile	uint32_t STAT;
  volatile	uint32_t TRM;
} ADC_TypeDef;

typedef enum {
	DR0 = 0,
	DR1 = 1,
	DR2 = 2,
	DR3 = 3,
	DR4 = 4
} ADC_Status;

#define ADC_CLOCK_FREQUENCY 1000000

//Write the IOCON address of Analog Pi
#define	ANALOG_PIN3_IOCON_ADDRESS	 0x4002C068
#define	ANALOG_PIN2_IOCON_ADDRESS	 0x4002C064
#define	ANALOG_PIN4_IOCON_ADDRESS	 0x4002C0F8

#define ANALOG_PIN3_IOCON	*((volatile uint32_t*)(ANALOG_PIN3_IOCON_ADDRESS))
#define ANALOG_PIN2_IOCON	*((volatile uint32_t*)(ANALOG_PIN2_IOCON_ADDRESS))
#define ANALOG_PIN4_IOCON	*((volatile uint32_t*)(ANALOG_PIN4_IOCON_ADDRESS))


//Write the max value of ADC.
#define ADC_MAX_VALUE 4095

//Define a ADC_CLKDIV variable for given ADC_CLOCK_FREQUENCY.
#define ADC_CLKDIV 59

#define ADC_BASE	0x40034000
#define ADC	((ADC_TypeDef*) ADC_BASE)

extern uint32_t ADC_Last;
extern uint8_t selectedADC;
extern uint8_t ADC_New_Data_Available;
extern uint32_t ADC_Last_Value[5];

void ADC_Init(void);
void ADC_Start(void);
void ADC_Stop(void);
void enableADC(uint8_t selected);
uint32_t* ADC_GetLastValue(void);

#endif

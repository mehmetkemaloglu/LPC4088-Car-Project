#include "ADC.h"

uint32_t ADC_Last = 0;
uint8_t ADC_New_Data_Available = 0;
uint8_t selectedADC = DR2;
uint32_t ADC_Last_Value[5];

void ADC_Init() {
	//Change the function value of pin to ADC.
	uint32_t ioconRegisterValue = ANALOG_PIN3_IOCON;
	ioconRegisterValue &= ~(1 << 2 | 1 << 1);
	ioconRegisterValue |= (1<<0);
	ANALOG_PIN3_IOCON = ioconRegisterValue;
	
	ioconRegisterValue = ANALOG_PIN2_IOCON;
	ioconRegisterValue &= ~(1 << 2 | 1 << 1);
	ioconRegisterValue |= (1<<0);
	ANALOG_PIN2_IOCON = ioconRegisterValue;
	
	ioconRegisterValue = ANALOG_PIN4_IOCON;
	ioconRegisterValue &= ~(1 << 2);
	ioconRegisterValue |= (1<<0 | 1 << 1);
	ANALOG_PIN4_IOCON = ioconRegisterValue;
	
	//Change the mode value of pin to mode which should be selected if Analog mode is used.
	ioconRegisterValue = ANALOG_PIN3_IOCON;
	ioconRegisterValue &= ~(1 << 3 | 1 << 4);
	ANALOG_PIN3_IOCON = ioconRegisterValue;
	
	ioconRegisterValue = ANALOG_PIN2_IOCON;
	ioconRegisterValue &= ~(1 << 3 | 1 << 4);
	ANALOG_PIN2_IOCON = ioconRegisterValue;
	
	ioconRegisterValue = ANALOG_PIN4_IOCON;
	ioconRegisterValue &= ~(1 << 3 | 1 << 4);
	ANALOG_PIN4_IOCON = ioconRegisterValue;
	
	//Change Analog/Digital mode of pin to Analog.
	ioconRegisterValue = ANALOG_PIN3_IOCON;
	ioconRegisterValue &= ~(1 << 7);
	ANALOG_PIN3_IOCON = ioconRegisterValue;
	
	ioconRegisterValue = ANALOG_PIN2_IOCON;
	ioconRegisterValue &= ~(1 << 7);
	ANALOG_PIN2_IOCON = ioconRegisterValue;
	
	ioconRegisterValue = ANALOG_PIN4_IOCON;
	ioconRegisterValue &= ~(1 << 7);
	ANALOG_PIN4_IOCON = ioconRegisterValue;
	
	//Turn on ADC.
	PCONP |= (1<<12);
	
	//Set the CLKDIV 
	uint32_t cr_val = ADC->CR;
	cr_val |= (1<<8 | 1<<9 | 1<< 11 | 1<<12 | 1<< 13);
	cr_val &= ~(1<<10 | 1<<14 | 1<<15);

	//Make the A/D converter operational
	cr_val |= (1<<21);
	
	//Make sure conversions are software controlled and require 31 clocks (Do not use Burst mode)
	cr_val&= ~(1<<16);
	ADC->CR = cr_val;
	
	//Configure CR SEL bits for sampled and converting corresponding pin.
	cr_val = ADC->CR;
	cr_val &= ~(255);
	cr_val |= 1<<2;
	ADC->CR = cr_val;
	
	//Enable interrupt for corresponding pin.
	ADC->INTEN |= (1<<8);
	
	//Enable ADC_IRQn (Interrupt Request).
	NVIC_EnableIRQ(ADC_IRQn);
	
	NVIC_SetPriority(ADC_IRQn, 4);

}

void ADC_Start () {
	//Write a code for starting A/D conversion on a rising edge on the TIMER 0 MATCH 1.
	uint32_t cr_val = ADC->CR;
	cr_val |= 1<<26;
	cr_val &= ~(1<<24 | 1<<25);
	ADC->CR = cr_val;

}

uint32_t* ADC_GetLastValue() {
	ADC_New_Data_Available = 0;
	return ADC_Last_Value;
}

void ADC_IRQHandler() {
	ADC->GDR &= ~((uint32_t)1 << 31);
	
	//Write the converted data (only the converted data) to ADC_Last variable.	
	uint32_t val = ADC->DR[selectedADC];
	val = val >>4;
	val &= 0xFFF;
	ADC_Last = val;
	ADC_Last_Value[selectedADC] = ADC_Last;

	ADC_New_Data_Available = 1;
	if(selectedADC == DR4){
		selectedADC = DR2;
	}else if(selectedADC == DR2){
		selectedADC = DR3;
	} else if(selectedADC == DR3){
		selectedADC = DR4;
	}
	enableADC(selectedADC);
}

void enableADC(uint8_t selected){
	uint32_t cr_val = ADC->CR;
	cr_val &= ~(255);
	cr_val |= 1<<selected;
	ADC->CR = cr_val;
}

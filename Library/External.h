#ifndef EXTERNAL_H
#define EXTERNAL_H

#include "LPC407x_8x_177x_8x.h"

typedef struct
{
	volatile	uint32_t EXTINT;
						uint32_t RESERVED0[1];
	volatile	uint32_t EXTMODE;
	volatile	uint32_t EXTPOLAR;
} EXT_TypeDef;

//0x00000000 is a dummy value, write the correct address
#define IOCON_TURN_ADDRESS	0x4002C128
#define IOCON_TURN	*((volatile uint32_t*)(IOCON_TURN_ADDRESS))

//0x00000000 is a dummy value, write the correct address
#define EXT_ADDRESS	0x400FC140
#define EXT	((EXT_TypeDef*) EXT_ADDRESS)

extern uint32_t turn;
void External_Init(void);

#endif

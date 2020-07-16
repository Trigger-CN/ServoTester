#include "softdshot.h"

volatile uint32_t DSHOT_PIN = 0;
volatile uint32_t DSHOT_Port = 0;
volatile uint32_t DSHOT_Set = 0;
volatile uint32_t DSHOT_Reset = 0;
volatile uint32_t DSHOT_Mask = 0;
volatile uint32_t DSHOT_Data = 0;
__asm void DSHOT_Transmit_48MHz();

void DSHOT_Write(Pxx_TypeDef pin, uint16_t throttle, bool rx, bool rev)
{
	DSHOT_PIN = pin;
	DSHOT_Port = (uint32_t)&(PIN_MAP[DSHOT_PIN].GPIOx->ODR);
	if(rev)
	{
		DSHOT_Set = 0;
		DSHOT_Reset = (uint32_t)(PIN_MAP[DSHOT_PIN].GPIO_Pin_x);
	}
	else
	{
		DSHOT_Set = (uint32_t)(PIN_MAP[DSHOT_PIN].GPIO_Pin_x);
		DSHOT_Reset = 0;
	}
	DSHOT_Mask = ~(uint32_t)(PIN_MAP[DSHOT_PIN].GPIO_Pin_x);
	
	throttle &= 2047;
	uint8_t sec1 = (throttle >> 7) & 0x0f;
	uint8_t sec2 = (throttle >> 3) & 0x0f;
	uint8_t sec3 = ((throttle << 1) | rx) & 0x0f;
	uint8_t check = sec1 ^ sec2 ^ sec3;
	DSHOT_Data = (uint32_t)sec1 << 12;
	DSHOT_Data |= (uint32_t)sec2 << 8;
	DSHOT_Data |= (uint32_t)sec3 << 4;
	DSHOT_Data |= (uint32_t)check;
	DSHOT_Data <<= 1;
	
	DSHOT_Transmit_48MHz();
}

/*
 * 48MHz: 27 NOP 32 NOP / 57 NOP 2 NOP   - TOTAL 59 NOP
 */
__asm void DSHOT_Transmit_48MHz()
{
	extern DSHOT_Port
	extern DSHOT_Set
	extern DSHOT_Reset
	extern DSHOT_Data
	extern DSHOT_Mask
	
	PUSH {r0-r7, lr}
	LDR r0, =DSHOT_Data		; Load data
	LDR r0, [r0]
	LDR r1, =DSHOT_Port
	LDR r1, [r1]
	MOVS r2, #0
	MOVS r3, #0
	LDR r2, =DSHOT_Set
	LDR r2, [r2]
	LDR r3, =DSHOT_Reset
	LDR r3, [r3]
	LDR r7, =DSHOT_Mask
	LDR r7, [r7]
	MOVS r6, #0x10
	
dshot_loop
	MOVS r5, r0
	LSRS r5, r6
	MOVS r4, #1
	ANDS r5, r4;
	CMP r5, #0x01
	BEQ dshot_logic_1_pulse
	
dshot_logic_0_pulse		
	LDR r4, [r1]							; SET PIN
	ANDS r4, r7
	ORRS r4, r2
	STRH r4, [r1]
	NOP												; Logic 0 High Level Hold Delay
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP	
	LDR r4, [r1]
	ANDS r4, r7
	ORRS r4, r3
	STRH r4, [r1]		
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP												; Logic 0 Low Level Hold Delay
	NOP
	NOP
	NOP
	NOP
	B dshot_end_cycle
	
dshot_logic_1_pulse
	LDR r4, [r1]							; SET PIN
	ANDS r4, r7
	ORRS r4, r2
	STRH r4, [r1]
	NOP												; Logic 1 High Level Hold Delay
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	LDR r4, [r1]
	ANDS r4, r7
	ORRS r4, r3
	STRH r4, [r1]
	NOP												; Logic 1 Low Level Hold Delay
	NOP
	
dshot_end_cycle
	SUBS r6, #1
	CMP r6, #0x00
	BEQ dshot_end_transmit
	B dshot_loop
	
dshot_end_transmit
	POP {r0-r7, pc}
}







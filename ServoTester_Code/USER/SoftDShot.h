#ifndef __SOFT_DSHOT_H__
#define __SOFT_DSHOT_H__

#include "stdint.h"
#include "arduino.h"

void DSHOT_Write(Pxx_TypeDef pin, uint16_t throttle, bool rx = 0, bool rev = false);

#endif


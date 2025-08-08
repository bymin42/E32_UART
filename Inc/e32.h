#ifndef E32_H
#define E32_H

#include "main.h"
#include <stdint.h>

// E32 mode selection
typedef enum {
  E32_MODE_NORMAL = 0,   // M1=0, M0=0
  E32_MODE_WAKEUP,       // M1=0, M0=1
  E32_MODE_POWERSAVE,    // M1=1, M0=0
  E32_MODE_SLEEP         // M1=1, M0=1  (AT config)
} E32_Mode;

// Function prototypes
void E32_Init(void);
void E32_SetMode(E32_Mode mode);
uint8_t E32_AuxHigh(void);
void E32_WaitAux(uint32_t timeout_ms);
HAL_StatusTypeDef E32_SendBytes(uint8_t *data, uint16_t len, uint32_t timeout);
HAL_StatusTypeDef E32_ReceiveBytes(uint8_t *buf, uint16_t len, uint32_t timeout);

#endif // E32_H

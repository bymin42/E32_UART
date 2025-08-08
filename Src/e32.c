#include "e32.h"

extern UART_HandleTypeDef huart2;

#define M0_PORT GPIOB
#define M0_PIN  GPIO_PIN_0
#define M1_PORT GPIOB
#define M1_PIN  GPIO_PIN_1
#define AUX_PORT GPIOB
#define AUX_PIN  GPIO_PIN_2

// Helper to set M1/M0 pin states
static void setPins(uint8_t m1, uint8_t m0)
{
  HAL_GPIO_WritePin(M1_PORT, M1_PIN, m1 ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(M0_PORT, M0_PIN, m0 ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void E32_Init(void)
{
  E32_SetMode(E32_MODE_NORMAL);
}

void E32_SetMode(E32_Mode mode)
{
  switch (mode) {
    case E32_MODE_NORMAL:    setPins(0,0); break;
    case E32_MODE_WAKEUP:    setPins(0,1); break;
    case E32_MODE_POWERSAVE: setPins(1,0); break;
    case E32_MODE_SLEEP:     setPins(1,1); break;
  }
  E32_WaitAux(1000);
  HAL_Delay(2); // Guard time
}

uint8_t E32_AuxHigh(void)
{
  return (HAL_GPIO_ReadPin(AUX_PORT, AUX_PIN) == GPIO_PIN_SET);
}

void E32_WaitAux(uint32_t timeout_ms)
{
  uint32_t start = HAL_GetTick();
  while (!E32_AuxHigh()) {
    if ((HAL_GetTick() - start) > timeout_ms) break;
  }
}

HAL_StatusTypeDef E32_SendBytes(uint8_t *data, uint16_t len, uint32_t timeout)
{
  E32_WaitAux(timeout);
  HAL_StatusTypeDef st = HAL_UART_Transmit(&huart2, data, len, timeout);
  E32_WaitAux(timeout);
  return st;
}

HAL_StatusTypeDef E32_ReceiveBytes(uint8_t *buf, uint16_t len, uint32_t timeout)
{
  return HAL_UART_Receive(&huart2, buf, len, timeout);
}

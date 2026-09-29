#include "stm32f4xx_crc.h"

void CRC_ResetDR(void)
{

  CRC->CR = CRC_CR_RESET;
}

uint32_t CRC_CalcCRC(uint32_t Data)
{
  CRC->DR = Data;

  return (CRC->DR);
}

uint32_t CRC_CalcBlockCRC(uint32_t pBuffer[], uint32_t BufferLength)
{
  uint32_t index = 0;

  for(index = 0; index < BufferLength; index++)
  {
    CRC->DR = pBuffer[index];
  }
  return (CRC->DR);
}

uint32_t CRC_GetCRC(void)
{
  return (CRC->DR);
}

void CRC_SetIDRegister(uint8_t IDValue)
{
  CRC->IDR = IDValue;
}

uint8_t CRC_GetIDRegister(void)
{
  return (CRC->IDR);
}

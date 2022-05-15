#include "enc28j60.h"
//--------------------------------------------------
extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;
//--------------------------------------------------
static void Error(void)
{
	LD_ON;
}
//--------------------------------------------------
static uint8_t SPIx_WriteRead(uint8_t Byte)
{
  uint8_t receivedbyte = 0;
  if(HAL_SPI_TransmitReceive(&hspi1, (uint8_t*) &Byte, (uint8_t*) &receivedbyte, 1, 0x1000) != HAL_OK)
  {
    Error();
  }
  return receivedbyte;
}
//--------------------------------------------------
void SPI_SendByte(uint8_t bt)
{
 SPIx_WriteRead(bt);
}
//--------------------------------------------------
uint8_t SPI_ReceiveByte(void)
{
 uint8_t bt = SPIx_WriteRead(0xFF);
 return bt; // повернення регістра даних
}
//--------------------------------------------------
void enc28j60_writeOp(uint8_t op,uint8_t addres, uint8_t data)
{
 SS_SELECT();
 SPI_SendByte(op|(addres&ADDR_MASK));
 SPI_SendByte(data);
 SS_DESELECT();
}
//--------------------------------------------------
static uint8_t enc28j60_readOp(uint8_t op,uint8_t addres)
{
 uint8_t result;
 SS_SELECT();
 SPI_SendByte(op|(addres&ADDR_MASK));
 SPI_SendByte(0x00);
	//пропускаєм фейк байт
  if(addres & 0x80) SPI_ReceiveByte();
 result=SPI_ReceiveByte();
  SS_DESELECT();
  return result;
}
//--------------------------------------------------
void enc28j60_ini(void)
{
	LD_OFF;
}
//--------------------------------------------------

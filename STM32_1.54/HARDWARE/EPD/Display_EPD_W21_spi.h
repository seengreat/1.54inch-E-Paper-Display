#ifndef _DISPLAY_EPD_W21_SPI_
#define _DISPLAY_EPD_W21_SPI_
#include "stm32f10x.h"


#define EPD_W21_MOSI_0	GPIO_ResetBits(GPIOB, GPIO_Pin_15)
#define EPD_W21_MOSI_1	GPIO_SetBits(GPIOB, GPIO_Pin_15)

#define EPD_W21_CLK_0	GPIO_ResetBits(GPIOB, GPIO_Pin_13)
#define EPD_W21_CLK_1	GPIO_SetBits(GPIOB, GPIO_Pin_13)

#define EPD_W21_CS_0	GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define EPD_W21_CS_1	GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define EPD_W21_DC_0	GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define EPD_W21_DC_1	GPIO_SetBits(GPIOA, GPIO_Pin_8)

#define EPD_W21_RST_0	GPIO_ResetBits(GPIOA, GPIO_Pin_11)
#define EPD_W21_RST_1	GPIO_SetBits(GPIOA, GPIO_Pin_11)

#define isEPD_W21_BUSY GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) // for solomen solutions


void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char data);
void EPD_W21_WriteCMD(unsigned char command);
void EPD_GPIO_Init(void);

#endif  //#ifndef _MCU_SPI_H_

/***********************************************************
						end file
***********************************************************/

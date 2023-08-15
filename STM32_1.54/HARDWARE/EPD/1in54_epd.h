#ifndef _1INCH54_EPD_H_
#define _1INCH54_EPD_H_

#include "sys.h"

#define EPD_WIDTH   200
#define EPD_HEIGHT  200

//#define EPD_W21_MOSI_0	GPIO_ResetBits(GPIOB, GPIO_Pin_15)
//#define EPD_W21_MOSI_1	GPIO_SetBits(GPIOB, GPIO_Pin_15)

//#define EPD_W21_CLK_0	GPIO_ResetBits(GPIOB, GPIO_Pin_13)
//#define EPD_W21_CLK_1	GPIO_SetBits(GPIOB, GPIO_Pin_13)

//#define EPD_W21_CS_0	GPIO_ResetBits(GPIOB, GPIO_Pin_12)
//#define EPD_W21_CS_1	GPIO_SetBits(GPIOB, GPIO_Pin_12)

//#define EPD_W21_DC_0	GPIO_ResetBits(GPIOA, GPIO_Pin_8)
//#define EPD_W21_DC_1	GPIO_SetBits(GPIOA, GPIO_Pin_8)

//#define EPD_W21_RST_0	GPIO_ResetBits(GPIOA, GPIO_Pin_11)
//#define EPD_W21_RST_1	GPIO_SetBits(GPIOA, GPIO_Pin_11)

//#define isEPD_W21_BUSY GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12) // for solomen solutions

#define	EPD_W21_CS      PBout(12)
#define	EPD_W21_DC      PAout(8)
#define	EPD_W21_RST     PAout(11)
#define	isEPD_W21_BUSY  PAin(12)

#define EPD_W21_MOSI	  PBout(15)

#define EPD_W21_CLK	    PBout(13)

#define	TEST_PIN  PBout(8) 


void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char data);
void EPD_W21_WriteCMD(unsigned char command);
void EPD_GPIO_Init(void);

//EPD
void Epaper_READBUSY(void);
void Epaper_Spi_WriteByte(unsigned char TxData);
void Epaper_Write_Command(unsigned char cmd);
void Epaper_Write_Data(unsigned char data);


void Epaper_HW_SW_RESET(void);
void EPD_HW_Init(void); //Electronic paper initialization
void EPD_Update(void);

void EPD_Part_Init(void);//Local refresh initialization
void EPD_Part_Update(void); 

void EPD_WhiteScreen_White(void);
void EPD_DeepSleep(void);
//Display 
void EPD_WhiteScreen_ALL(const unsigned char *datas);
void EPD_SetRAMValue_BaseMap(const unsigned char * datas);
void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE);
void EPD_Dis_Part_myself(unsigned int x_startA,unsigned int y_startA,const unsigned char * datasA,
	                       unsigned int x_startB,unsigned int y_startB,const unsigned char * datasB,
												 unsigned int x_startC,unsigned int y_startC,const unsigned char * datasC,
												 unsigned int x_startD,unsigned int y_startD,const unsigned char * datasD,
											   unsigned int x_startE,unsigned int y_startE,const unsigned char * datasE,
												 unsigned int PART_COLUMN,unsigned int PART_LINE
	                      );
//Display canvas function
void EPD_HW_Init_GUI(void); //EPD init GUI
void EPD_Display(unsigned char *Image); 
void EPD_HW_Init_P(void);
void EPD_Standby(void);

void EPD_HW_Init_Fast(void);
void EPD_WhiteScreen_ALL_Fast(const unsigned char *datas);
#endif
/***********************************************************
						end file
***********************************************************/



#ifndef __EPD_1INCH54_H
#define __EPD_1INCH54_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

 /* Pin wiring definion
 * VCC---->5V
 * GND---->GND
 * RST---->D9
 * BUSY--->D10
 * D/C---->D8
 * MOSI--->D51
 * CLK---->D52
 * CS----->D53
 * */
//IO settings
#define TEST_Pin  7 
#define BUSY_Pin  10 
#define RES_Pin   9 
#define DC_Pin    8 

#define CS_Pin    53 
#define SCK_Pin   52 
#define SDI_Pin   51


#define TEST_0  digitalWrite(TEST_Pin,LOW)
#define TEST_1  digitalWrite(TEST_Pin,HIGH) 

#define EPD_W21_MOSI_0  digitalWrite(SDI_Pin,LOW)
#define EPD_W21_MOSI_1  digitalWrite(SDI_Pin,HIGH) 

#define EPD_W21_CLK_0  digitalWrite(SCK_Pin,LOW)
#define EPD_W21_CLK_1  digitalWrite(SCK_Pin,HIGH)

#define EPD_W21_CS_0   digitalWrite(CS_Pin,LOW)
#define EPD_W21_CS_1   digitalWrite(CS_Pin,HIGH)

#define EPD_W21_DC_0   digitalWrite(DC_Pin,LOW)
#define EPD_W21_DC_1   digitalWrite(DC_Pin,HIGH)
#define EPD_W21_RST_0  digitalWrite(RES_Pin,LOW)
#define EPD_W21_RST_1  digitalWrite(RES_Pin,HIGH)
#define isEPD_W21_BUSY digitalRead(BUSY_Pin)

#define MONOMSB_MODE 1
#define MONOLSB_MODE 2 
#define RED_MODE     3

#define MAX_LINE_BYTES  25// =200/8
#define MAX_COLUMN_BYTES  200

#define ALLSCREEN_GRAGHBYTES 5000

#define EPD_WIDTH   200
#define EPD_HEIGHT  200

void printf_begin(void);

////////FUNCTION//////
void driver_delay_us(unsigned int xus);
void driver_delay_xms(unsigned long xms);
void DELAY_S(unsigned int delaytime);     
void SPI_Delay(unsigned char xrate);
void SPI_Write(unsigned char value);
void Epaper_Write_Command(unsigned char command);
void Epaper_Write_Data(unsigned char data);
//EPD
void Epaper_READBUSY(void);

void EPD_HW_Init(void); //Electronic paper initialization
void EPD_Part_Init(void); //Local refresh initialization
void EPD_WhiteScreen_ALL_Fast(const unsigned char *datas);
void EPD_HW_Init_GUI(void);
void EPD_Part_Update(void); 
void EPD_Update(void);
void EPD_Update_Fast(void);

void EPD_WhiteScreen_Black(void);
void EPD_WhiteScreen_White(void);
void EPD_DeepSleep(void);
//Display 
void EPD_WhiteScreen_ALL(const unsigned char * datas);
void EPD_SetRAMValue_BaseMap(const unsigned char * datas);
void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE); 
void EPD_Display(unsigned char *Image);
void EPD_Dis_Part_myself(unsigned int x_startA,unsigned int y_startA,const unsigned char * datasA,
                         unsigned int x_startB,unsigned int y_startB,const unsigned char * datasB,
                         unsigned int x_startC,unsigned int y_startC,const unsigned char * datasC,
                         unsigned int x_startD,unsigned int y_startD,const unsigned char * datasD,
                         unsigned int x_startE,unsigned int y_startE,const unsigned char * datasE,
                         unsigned int PART_COLUMN,unsigned int PART_LINE);
#endif

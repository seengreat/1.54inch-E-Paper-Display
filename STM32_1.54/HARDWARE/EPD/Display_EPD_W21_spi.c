#include "Display_EPD_W21_spi.h"

/****************************
* PB12-----SPI2_NSS
* PB13-----SPI2_SCK
* PB15-----SPI2_MOSI
* PA8------DC
* PA11-----RST
* PA12-----BUSY
***************************/

void EPD_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	  				     	
	
	 //CS-->PB12   SCK-->PB13  SDO--->PB15 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;		//Port configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 		
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  	
		
	 // D/C--->PA8	   RES-->PA11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_11;		//Port configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	  				     		
	
	// BUSY--->PA12
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	//Pull up input
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);				//Initialize GPIO
	
	 //LED 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		//Port configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 		
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}



void SPI_Write(unsigned char value)                                    
{                                                           
    unsigned char i;

    for(i=0; i<8; i++)   
    {
        EPD_W21_CLK_0;
        if(value & 0x80)
        	EPD_W21_MOSI_1;
        else
        	EPD_W21_MOSI_0;		
        value = (value << 1); 
        EPD_W21_CLK_1; 
    }
}

void EPD_W21_WriteCMD(unsigned char command)
{
  EPD_W21_CS_0;                   
	EPD_W21_DC_0;		// command write
	SPI_Write(command);
	EPD_W21_CS_1;
}
void EPD_W21_WriteDATA(unsigned char data)
{
  EPD_W21_CS_0;                   
	EPD_W21_DC_1;		// data write
	SPI_Write(data);
	EPD_W21_CS_1;
}



/***********************************************************
						end file
***********************************************************/

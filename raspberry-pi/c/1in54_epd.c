#include <string.h>
#include <stdlib.h>	
#include <stdio.h>
#include <stdio.h>	
#include <wiringPiSPI.h>
#include <wiringPi.h>
#include "1in54_epd.h"

//define pin conection in wiringpi number
/****************************
* PIN_CS      10 
* PIN_RST     0 
* PIN_DC      6
* PIN_BBUSY   5
***************************/

void EPD_GPIO_Init(void)
{
	printf("Init gpio for wiringpi\r\n");
    if(wiringPiSetup() < 0)//use wiringpi Pin number
    { 
        printf("wiringPi lib init failed! \r\n");
    } 
    pinMode(PIN_CS, OUTPUT);
    pinMode(PIN_RST, OUTPUT);
    pinMode(PIN_DC, OUTPUT);
    pinMode(PIN_BUSY, INPUT);
    pullUpDnControl(PIN_BUSY, PUD_DOWN);
    wiringPiSPISetup(0,8000000);  
}



//void SPI_Write(unsigned char value)                                    
//{                                                           
    //unsigned char i;

    //for(i=0; i<8; i++)   
    //{
        //EPD_W21_CLK = 0;
        //if(value & 0x80)
        	//EPD_W21_MOSI = 1;
        //else
        	//EPD_W21_MOSI = 0;		
        //value = (value << 1); 
        //EPD_W21_CLK = 1; 
    //}
//}

//void EPD_W21_WriteCMD(unsigned char command)
//{
    //digitalWrite(PIN_CS, 0);                    
	//digitalWrite(PIN_DC, 0); 		// command write
	////SPI_Write(command);
	//wiringPiSPIDataRW(0,&command,1); 
	//digitalWrite(PIN_CS, 1); 
//}
//void EPD_W21_WriteDATA(unsigned char data)
//{
    //digitalWrite(PIN_CS, 0);                    
	//digitalWrite(PIN_DC, 1); 		// data write
	////SPI_Write(data);
	//wiringPiSPIDataRW(0,&data,1); 
	//digitalWrite(PIN_CS, 1); 
//}

void delay_xms(unsigned int xms)
{
	unsigned int i;
	while(xms--)
	{
		i=12000;
		while(i--);
	}
}


void EPD_Display(unsigned char *Image)
{
    unsigned int width, Height,i,j;
    width = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    Height = EPD_HEIGHT;

    Epaper_Write_Command(0x24);
    for ( j = 0; j < Height; j++) 
	{
        for ( i = 0; i < width; i++) 
		{
           Epaper_Write_Data(Image[i + j * width]);
        }
    }
    EPD_Update();		 
}

//void Epaper_Spi_WriteByte(unsigned char TxData)
//{				   			 
		//unsigned char TempData;
		//unsigned char scnt;
		//TempData=TxData;

		//EPD_W21_CLK = 0;  
		//for(scnt=0;scnt<8;scnt++)
		//{ 
				//if(TempData&0x80)
					//EPD_W21_MOSI = 1 ;
				//else
					//EPD_W21_MOSI = 0 ;
				//EPD_W21_CLK = 1;  
				//EPD_W21_CLK = 0;  
				//TempData=TempData<<1;
		//}
//}

void Epaper_READBUSY(void)
{ 
	while(1)
	{	 //=1 BUSY
		 if(digitalRead(PIN_BUSY)==LOW) break;
	}  
}

void Epaper_Write_Command(unsigned char cmd)
{
	digitalWrite(PIN_CS, 1); 
	digitalWrite(PIN_CS, 0); 
	digitalWrite(PIN_DC, 0);   // D/C#   0:command  1:data
	wiringPiSPIDataRW(0,&cmd,1); 
//	Epaper_Spi_WriteByte(cmd);
	digitalWrite(PIN_CS, 1); 
}

void Epaper_Write_Data(unsigned char data)
{
	digitalWrite(PIN_CS, 1); 
	digitalWrite(PIN_CS, 0); 
	digitalWrite(PIN_DC, 1);   // D/C#   0:command  1:data
	wiringPiSPIDataRW(0,&data,1); 
//	Epaper_Spi_WriteByte(data);
	digitalWrite(PIN_CS, 1); 
}


	
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////

//SSD1681
void EPD_HW_Init(void)
{
	digitalWrite(PIN_RST, 0);  // Module reset   
	delay_xms(10);//At least 10ms delay 
	digitalWrite(PIN_RST, 1);
	delay_xms(10); //At least 10ms delay 
		
	Epaper_READBUSY();   
	Epaper_Write_Command(0x12);  //SWRESET
	Epaper_READBUSY();   
			
	Epaper_Write_Command(0x01); //Driver output control     
	delay_xms(10); //At least 10ms delay 	
	Epaper_Write_Data(0xC7);
	delay_xms(10); //At least 10ms delay 
	Epaper_Write_Data(0x00);
	delay_xms(10); //At least 10ms delay 
	Epaper_Write_Data(0x00);

	Epaper_Write_Command(0x11); //data entry mode       
	Epaper_Write_Data(0x01);

	Epaper_Write_Command(0x44); //set Ram-X address start/end position   
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x18);    //0x0C-->(18+1)*8=200

	Epaper_Write_Command(0x45); //set Ram-Y address start/end position          
	Epaper_Write_Data(0xC7);    //0xC7-->(199+1)=200
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00); 

	Epaper_Write_Command(0x3C); //BorderWavefrom
	Epaper_Write_Data(0x05);	
				
	Epaper_Write_Command(0x18); //Read built-in temperature sensor
	Epaper_Write_Data(0x80);	

	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(0x00);
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X199;    
	Epaper_Write_Data(0xC7);
	Epaper_Write_Data(0x00);
	Epaper_READBUSY();
	
}
void EPD_HW_Init_Fast(void)
{
	digitalWrite(PIN_RST, 0);  // Module reset   
	delay_xms(10);//At least 10ms delay 
	digitalWrite(PIN_RST, 1);
	delay_xms(10); //At least 10ms delay 
		
	Epaper_Write_Command(0x12);  //SWRESET
	Epaper_READBUSY();   
		
	Epaper_Write_Command(0x18); //Read built-in temperature sensor
	Epaper_Write_Data(0x80);	
				
	Epaper_Write_Command(0x22); // Load temperature value
	Epaper_Write_Data(0xB1);		
	Epaper_Write_Command(0x20);	
	Epaper_READBUSY();   

	Epaper_Write_Command(0x1A); // Write to temperature register
	Epaper_Write_Data(0x64);		
	Epaper_Write_Data(0x00);	
							
	Epaper_Write_Command(0x22); // Load temperature value
	Epaper_Write_Data(0x91);		
	Epaper_Write_Command(0x20);	
	Epaper_READBUSY();   
}

void EPD_HW_Init_GUI(void)
{
	digitalWrite(PIN_RST, 0);  // Module reset   
	delay_xms(10);//At least 10ms delay 
	digitalWrite(PIN_RST, 1);
	delay_xms(10); //At least 10ms delay 
	
	Epaper_READBUSY();   
	Epaper_Write_Command(0x12);  //SWRESET
	Epaper_READBUSY();   
		
	Epaper_Write_Command(0x01); //Driver output control 
	delay_xms(10); //At least 10ms delay 	
	Epaper_Write_Data(0xC7);
	delay_xms(10); //At least 10ms delay 
	Epaper_Write_Data(0x00);
	delay_xms(10); //At least 10ms delay 
	Epaper_Write_Data(0x01);//0x00:Show normal 0x01:Show mirror

	Epaper_Write_Command(0x11); //data entry mode       
	Epaper_Write_Data(0x01);

	Epaper_Write_Command(0x44); //set Ram-X address start/end position   
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x18);    //0x0C-->(18+1)*8=200

	Epaper_Write_Command(0x45); //set Ram-Y address start/end position          
	Epaper_Write_Data(0xC7);    //0xC7-->(199+1)=200
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00);
	Epaper_Write_Data(0x00); 

	Epaper_Write_Command(0x3C); //BorderWavefrom
	Epaper_Write_Data(0x05);	
	  	
    Epaper_Write_Command(0x18); //Read built-in temperature sensor
	Epaper_Write_Data(0x80);	

	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(0x00);
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X199;    
	Epaper_Write_Data(0xC7);
	Epaper_Write_Data(0x00);
    Epaper_READBUSY();
	
}
/////////////////////////////////////////////////////////////////////////////////////////
/*When the electronic paper screen is updated, do not unplug the electronic paper to avoid damage to the screen*/

void EPD_Update(void)
{   
  Epaper_Write_Command(0x22); //Display Update Control
  Epaper_Write_Data(0xF7);   
  Epaper_Write_Command(0x20); //Activate Display Update Sequence
  Epaper_READBUSY();   

}
void EPD_Update_Fast(void)
{   
  Epaper_Write_Command(0x22); //Display Update Control
  Epaper_Write_Data(0xC7);   
  Epaper_Write_Command(0x20); //Activate Display Update Sequence
  Epaper_READBUSY();   

}
/*When the electronic paper screen is updated, do not unplug the electronic paper to avoid damage to the screen*/
void EPD_Part_Update(void)
{
	Epaper_Write_Command(0x22); //Display Update Control
	Epaper_Write_Data(0xFF);   
	Epaper_Write_Command(0x20); //Activate Display Update Sequence
	Epaper_READBUSY(); 			
}
//////////////////////////////All screen update////////////////////////////////////////////
void EPD_WhiteScreen_ALL(const unsigned char *datas)
{
   unsigned int i;	
   Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
   for(i=0;i<5000;i++)
   {               
       Epaper_Write_Data(*datas);
	   datas++;
   }
   EPD_Update();	 
}
void EPD_WhiteScreen_ALL_Fast(const unsigned char *datas)
{
   unsigned int i;	
   Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
   for(i=0;i<5000;i++)
   {               
       Epaper_Write_Data(*datas);
	   datas++;
   } 
	 
   EPD_Update_Fast();	 
}
///////////////////////////Part update//////////////////////////////////////////////
//The x axis is reduced by one byte, and the y axis is reduced by one pixel.
void EPD_SetRAMValue_BaseMap( const unsigned char * datas)
{
	unsigned int i;   
	const unsigned char  *datas_flag;   
	datas_flag=datas;

	
    Epaper_Write_Command(0x24);   //Write Black and White image to RAM
	for(i=0;i<5000;i++)
	{               
		Epaper_Write_Data(*datas);
   		datas++;
	}
	datas=datas_flag;
    Epaper_Write_Command(0x26);   //Write Black and White image to RAM
    for(i=0;i<5000;i++)
    {               
        Epaper_Write_Data(*datas);
		datas++;
    }
    EPD_Update();		 
	 
}
void EPD_Dis_Part(unsigned int x_start,unsigned int y_start,const unsigned char * datas,unsigned int PART_COLUMN,unsigned int PART_LINE)
{
	unsigned int i;  
	unsigned int x_end,y_start1,y_start2,y_end1,y_end2;
	x_start=x_start/8;
	x_end=x_start+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_start;
	if(y_start>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_start+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		

		// Add hardware reset to prevent background color change
	digitalWrite(PIN_RST, 0);  // Module reset   
	delay_xms(10);//At least 10ms delay 
	digitalWrite(PIN_RST, 1);
	delay_xms(10); //At least 10ms delay 	
	  //Lock the border to prevent flashing
	Epaper_Write_Command(0x3C); //BorderWavefrom,
	Epaper_Write_Data(0x80);
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_start);    // RAM x address start at 00h;
	Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);    // ????=0	


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_start); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	
	Epaper_Write_Command(0x24);   //Write Black and White image to RAM
    for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
    {                         
        Epaper_Write_Data(*datas);
		datas++;
    } 
	EPD_Part_Update();

}

void EPD_DeepSleep(void)
{  	
    Epaper_Write_Command(0x10); //enter deep sleep
    Epaper_Write_Data(0x01); 
    delay_xms(100);
}


/////////////////////////////////Single display////////////////////////////////////////////////

void EPD_WhiteScreen_White(void)

{
    unsigned int i,k;
	Epaper_Write_Command(0x24);   //write RAM for black(0)/white (1)
	for(k=0;k<200;k++)
    {
	    for(i=0;i<25;i++)
		{
		    Epaper_Write_Data(0xff);
		}
    }
	EPD_Update();
}

/////////////////////////////////////TIME///////////////////////////////////////////////////
void EPD_Dis_Part_myself(unsigned int x_startA,unsigned int y_startA,const unsigned char * datasA,
	                       unsigned int x_startB,unsigned int y_startB,const unsigned char * datasB,
												 unsigned int x_startC,unsigned int y_startC,const unsigned char * datasC,
												 unsigned int x_startD,unsigned int y_startD,const unsigned char * datasD,
											   unsigned int x_startE,unsigned int y_startE,const unsigned char * datasE,
												 unsigned int PART_COLUMN,unsigned int PART_LINE
	                      )
{
		unsigned int i;  
		unsigned int x_end,y_start1,y_start2,y_end1,y_end2;
		
		//Data A////////////////////////////
		x_startA=x_startA/8;//Convert to byte
		x_end=x_startA+PART_LINE/8-1; 
		
		y_start1=0;
		y_start2=y_startA-1;
		if(y_startA>=256)
		{
				y_start1=y_start2/256;
				y_start2=y_start2%256;
		}
		y_end1=0;
		y_end2=y_startA+PART_COLUMN-1;
		if(y_end2>=256)
		{
				y_end1=y_end2/256;
				y_end2=y_end2%256;		
		}		
		
		// Add hardware reset to prevent background color change
		digitalWrite(PIN_RST, 0);  // Module reset   
		delay_xms(10);//At least 10ms delay 
		digitalWrite(PIN_RST, 1);
		delay_xms(10); //At least 10ms delay 	
			//Lock the border to prevent flashing
		Epaper_Write_Command(0x3C); //BorderWavefrom,
		Epaper_Write_Data(0x80);	
		
		Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
		Epaper_Write_Data(x_startA);    // RAM x address start at 00h;
		Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
		Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
		Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
		Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
		Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
		Epaper_Write_Data(y_end1);    


		Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
		Epaper_Write_Data(x_startA); 
		Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
		Epaper_Write_Data(y_start2);
		Epaper_Write_Data(y_start1);
	
	
	 Epaper_Write_Command(0x24);   //Write Black and White image to RAM
    for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
    {                         
		Epaper_Write_Data(*datasA);
		datasA++;
    } 
	//Data B/////////////////////////////////////
	x_startB=x_startB/8;//Convert to byte
	x_end=x_startB+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_startB-1;
	if(y_startB>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_startB+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_startB);    // RAM x address start at 00h;
	Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);   


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_startB); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	
	Epaper_Write_Command(0x24);   //Write Black and White image to RAM
    for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
    {                         
        Epaper_Write_Data(*datasB);
		datasB++;
    } 	 
	 
	//Data C//////////////////////////////////////
	x_startC=x_startC/8;//Convert to byte
	x_end=x_startC+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_startC-1;
	if(y_startC>=256)
	{
			y_start1=y_start2/256;
			y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_startC+PART_COLUMN-1;
	if(y_end2>=256)
	{
			y_end1=y_end2/256;
			y_end2=y_end2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_startC);    // RAM x address start at 00h;
	Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);   


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_startC); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	
	Epaper_Write_Command(0x24);   //Write Black and White image to RAM
    for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
    {                         
        Epaper_Write_Data(*datasC);
		datasC++;
    } 	 	 
 	 
	//Data D//////////////////////////////////////
	x_startD=x_startD/8;//Convert to byte
	x_end=x_startD+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_startD-1;
	if(y_startD>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_startD+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_startD);    // RAM x address start at 00h;
	Epaper_Write_Data(x_end);        // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);    


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_startD); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	
	Epaper_Write_Command(0x24);   //Write Black and White image to RAM
    for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
    {                         
        Epaper_Write_Data(*datasD);
		datasD++;
    } 
	//Data E//////////////////////////////////////
	x_startE=x_startE/8;//Convert to byte
	x_end=x_startE+PART_LINE/8-1; 
	
	y_start1=0;
	y_start2=y_startE-1;
	if(y_startE>=256)
	{
		y_start1=y_start2/256;
		y_start2=y_start2%256;
	}
	y_end1=0;
	y_end2=y_startE+PART_COLUMN-1;
	if(y_end2>=256)
	{
		y_end1=y_end2/256;
		y_end2=y_end2%256;		
	}		
	
	Epaper_Write_Command(0x44);       // set RAM x address start/end, in page 35
	Epaper_Write_Data(x_startE);    // RAM x address start at 00h;
	Epaper_Write_Data(x_end);    // RAM x address end at 0fh(15+1)*8->128 
	Epaper_Write_Command(0x45);       // set RAM y address start/end, in page 35
	Epaper_Write_Data(y_start2);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_start1);    // RAM y address start at 0127h;
	Epaper_Write_Data(y_end2);    // RAM y address end at 00h;
	Epaper_Write_Data(y_end1);    


	Epaper_Write_Command(0x4E);   // set RAM x address count to 0;
	Epaper_Write_Data(x_startE); 
	Epaper_Write_Command(0x4F);   // set RAM y address count to 0X127;    
	Epaper_Write_Data(y_start2);
	Epaper_Write_Data(y_start1);
	
	
	Epaper_Write_Command(0x24);   //Write Black and White image to RAM
    for(i=0;i<PART_COLUMN*PART_LINE/8;i++)
    {                         
        Epaper_Write_Data(*datasE);
		datasE++;
    } 	  
	EPD_Part_Update();

}

/***********************************************************
						end file
***********************************************************/


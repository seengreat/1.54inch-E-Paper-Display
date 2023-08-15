#include "epd_demo.h"
#include "epd_1inch54.h"
#include "epd_gui.h"
#include "fonts.h"
#include <SPI.h>
/***************************************************************************************
 * Project  :
 * Experimental Platform :Arduino Mega 2560 + 1.54inch E-Paper Display
 * Hardware Connection :Arduino Mega 2560-> 1.54inch E-Paper
 *      D53 --> CS
 *      D52 --> CLK
 *      D51 --> MOSI
 *      D8  --> DC
 *      D9  --> RST
 *      D10  --> BUSY
 * Author      : Andy Li
 * Web Site    :www.seengreat.com
***************************************************************************************/
/******************************
 * image :
 * 0------------> x (1~200) 
 * |
 * |
 * |
 * |
 * y (1~200)
 * 
 * Pin wiring definion
 * VCC---->5V
 * GND---->GND
 * RST---->D9
 * BUSY--->D10
 * D/C---->D8
 * MOSI--->D51
 * CLK---->D52
 * CS----->D53
 * */
#define SPI_USE_HARDWARE 0 //1:using hardware SPI; 0: Using GPIO to emulate SPI
void setup() {
    Serial.begin(115200);
    printf_begin();
   printf("Init gpio for arduino\r\n");
   pinMode(BUSY_Pin, INPUT); 
   pinMode(RES_Pin, OUTPUT);  
   pinMode(DC_Pin, OUTPUT);    
   pinMode(CS_Pin, OUTPUT);    
   pinMode(SCK_Pin, OUTPUT);    
   pinMode(SDI_Pin, OUTPUT);
   pinMode(TEST_Pin, OUTPUT);    
   
}

unsigned char image[5000];//Define canvas space  

//Tips//
/*
1.When the e-paper is refreshed in full screen, the picture flicker is a normal phenomenon, and the main function is to clear the display afterimage in the previous picture.
2.When the partial refresh is performed, the screen does not flash.
3.After the e-paper is refreshed, you need to put it into sleep mode, please do not delete the sleep command.
4.Please do not take out the electronic paper when power is on.
5.Wake up from sleep, need to re-initialize the e-paper.
6.When you need to transplant the driver, you only need to change the corresponding IO. The BUSY pin is the input mode and the others are the output mode.
*/
void loop() {
    unsigned char i,Miao_L,Miao_H,Fen_L,Fen_H; 

    EPD_HW_Init(); //Electronic paper initialization
    EPD_WhiteScreen_ALL(gImage_0); //Refresh the picture in full screen need 2.72s
    delay(2000);
    //Clear
    EPD_HW_Init(); //EPD init
    EPD_WhiteScreen_White();//EPD Clear
    EPD_WhiteScreen_ALL_Fast(gImage_1);//EPD_picture1 need 2.72s   
    EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
    delay(2000);     
//    //////////////////////Partial refresh time demo/////////////////////////////////////
    EPD_HW_Init(); //Electronic paper initialization  
    EPD_SetRAMValue_BaseMap(gImage_basemap); //Partial refresh background color 2.1s  
    Miao_H = 0;
    Fen_L = 0;
    Fen_H = 0;        
    for(Miao_L=0;Miao_L<10;Miao_L++)
    {
         //EPD_Dis_Part_myself function need 650~670ms                      
         EPD_Dis_Part_myself(64,40,Num[Miao_L],  //x-A,y-A,DATA-A
                64,72,Num[Miao_H],         //x-B,y-B,DATA-B
                64,112,gImage_numdot,      //x-C,y-C,DATA-C
                64,154,Num[Fen_L],         //x-D,y-D,DATA-D
                64,186,Num[Fen_H],32,64);  //x-E,y-E,DATA-E,Resolution 32*64 
         delay(100);    
    }
////////////////////////////////////////////////////////////////////////  
  //Clear screen
    EPD_HW_Init(); //Electronic paper initialization
    EPD_WhiteScreen_White(); //Show all white need 2.08s
    EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
    delay(2000);    
    ///////////////////////////GUI///////////////////////////////////////////////////////////////////////////////////
   //Data initialization settings
    Image_Init(image, EPD_WIDTH, EPD_HEIGHT, 270, WHITE); //Set screen size and display orientation

    /**************Drawing**********************/
    EPD_HW_Init_GUI(); //EPD init GUI
    Gui_Clear(WHITE);
    //Point   
    Gui_Draw_Point(5, 10, BLACK, PIXEL_1X1, DOT_STYLE_DFT);
    Gui_Draw_Point(5, 25, BLACK, PIXEL_2X2, DOT_STYLE_DFT);
    Gui_Draw_Point(5, 40, BLACK, PIXEL_3X3, DOT_STYLE_DFT);
    Gui_Draw_Point(5, 55, BLACK, PIXEL_4X4, DOT_STYLE_DFT);
    //Line
    Gui_Draw_Line(15, 10, 55, 50, BLACK, PIXEL_1X1, SOLID);
    Gui_Draw_Line(55, 10, 15, 50, BLACK, PIXEL_1X1, SOLID);
    //Rectangle
    Gui_Draw_Rectangle(15, 10, 55, 50, BLACK, EMPTY, PIXEL_1X1);
    Gui_Draw_Rectangle(70, 10, 110, 50, BLACK, FULL, PIXEL_1X1);
    //Circle
    Gui_Draw_Circle(135, 30, 18, BLACK, EMPTY, PIXEL_1X1);
    Gui_Draw_Circle(176, 30, 18, BLACK, FULL, PIXEL_1X1);
    Gui_Draw_Str(10, 75, "abcdefg", &Font12, WHITE, BLACK); //7*12
    Gui_Draw_Str(10, 100, "ABCabc012345", &Font16, WHITE, BLACK); //11*16
    Gui_Draw_Str(10, 130, "1.54\" E-Paper", &Font20, WHITE, BLACK); //14*20
    Gui_Draw_Str(10, 160, "SEENGREAT", &Font24, WHITE, BLACK); //17*24
    TEST_1;
    EPD_Display(image);//display image need 2.74s
    TEST_0;  
    EPD_DeepSleep();//EPD_DeepSleep,Sleep instruction is necessary, please do not delete!!!
    delay(2000);         
  
    //Clear screen
    EPD_HW_Init(); //Electronic paper initialization
    EPD_WhiteScreen_White(); //Show all white
    EPD_DeepSleep();  //Enter deep sleep,Sleep instruction is necessary, please do not delete!!!
    delay(2000);  

    while(1); 
}

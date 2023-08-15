#ifndef __EPD_GUI_H
#define __EPD_GUI_H

#include "../Fonts/fonts.h"
#include <stdint.h>
/**
 * Image attributes
**/
typedef struct {
    uint8_t *img;
    uint16_t w;
    uint16_t h;
    uint16_t mem_w;
    uint16_t mem_h;
    uint16_t color;
    uint16_t rotate;
    uint16_t mirror;
    uint16_t byte_w;
    uint16_t byte_h;
} IMAGE;
extern IMAGE Image;

/**
 * Display rotate
**/
#define ROTATE_0            0
#define ROTATE_90           90
#define ROTATE_180          180
#define ROTATE_270          270

/**
 * Display Flip
**/
typedef enum {
    MIRROR_NONE  = 0x00,
    MIRROR_HORIZONTAL = 0x01,
    MIRROR_VERTICAL = 0x02,
    MIRROR_ORIGIN = 0x03,
} MIRROR_IMAGE;
#define MIRROR_IMAGE_DFT MIRROR_NONE

/**
 * image color
**/
#define WHITE          0xFF
#define BLACK          0x00
#define RED            BLACK

#define IMAGE_BACKGROUND    WHITE
#define FONT_FOREGROUND     BLACK
#define FONT_BACKGROUND     WHITE

/**
 * The size of the point
**/
typedef enum {
    PIXEL_1X1  = 1,		// 1 x 1
    PIXEL_2X2  , 		// 2 X 2
    PIXEL_3X3  ,		// 3 X 3
    PIXEL_4X4  ,		// 4 X 4
    PIXEL_5X5  , 		// 5 X 5
    PIXEL_6X6  , 		// 6 X 6
    PIXEL_7X7  , 		// 7 X 7
    PIXEL_8X8  , 		// 8 X 8
} DOT_PIXEL;
#define DOT_PIXEL_DFT  PIXEL_1X1  //Default dot pilex

/**
 * Point size fill style
**/
typedef enum {
    AROUND  = 1,		// dot pixel 1 x 1
    RIGHTUP  , 		// dot pixel 2 X 2
} DOT_STYLE;
#define DOT_STYLE_DFT  AROUND  //Default dot pilex

/**
 * Line style, solid or dashed
**/
typedef enum {
    SOLID = 0,
    DOTTED,
} LINE_STYLE;

/**
 * Whether the graphic is filled
**/
typedef enum {
    EMPTY = 0,
    FULL,
} DRAW_FILL;

/**
 * Custom structure of a time attribute
**/
typedef struct {
    uint16_t year;  //0000
    uint8_t  month; //1 - 12
    uint8_t  day;   //1 - 30
    uint8_t  hour;  //0 - 23
    uint8_t  min;   //0 - 59
    uint8_t  sec;   //0 - 59
} DRAW_TIME;
extern DRAW_TIME Draw_Time;

//init and clear
void Image_Init(uint8_t *image, uint16_t width, uint16_t height, uint16_t rotate, uint16_t color);
void Gui_SelectImage(uint8_t *image);
void Gui_SetRotate(uint16_t rotate);
void Gui_SetMirror(uint8_t mirror);
void Gui_SetPixel(uint16_t x, uint16_t y, uint16_t color);

void Gui_Clear(uint16_t color);
void Gui_ClearWindows(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

//Drawing
void Gui_Draw_Point(uint16_t x, uint16_t y, uint16_t color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style);
void Gui_Draw_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, DOT_PIXEL Dot_Pixel, LINE_STYLE style);
void Gui_Draw_Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color, DRAW_FILL Filled , DOT_PIXEL Dot_Pixel);
void Gui_Draw_Circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color, DRAW_FILL Draw_Fill , DOT_PIXEL Dot_Pixel);

//Display string
void Gui_Draw_Char(uint16_t x, uint16_t y, const char str_char, FONT* Font, uint16_t Color_Background, uint16_t Color_Foreground);
void Gui_Draw_Str(uint16_t x, uint16_t y, const char *str_char, FONT* Font, uint16_t Color_Background, uint16_t Color_Foreground);
void Gui_Draw_Num(uint16_t x, uint16_t y, int32_t nummber, FONT* Font, uint16_t Color_Background, uint16_t Color_Foreground);
void Gui_draw_time(uint16_t x1, uint16_t y1, DRAW_TIME *pTime, FONT* Font, uint16_t Color_Background, uint16_t Color_Foreground);

//pic
void Gui_DrawBitMap(const unsigned char* image_buffer);


#endif






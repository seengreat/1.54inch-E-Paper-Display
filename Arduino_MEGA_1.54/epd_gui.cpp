#include <string.h>
#include "epd_1inch54.h"
#include <spi.h>
#include "epd_gui.h"
#include "fonts.h"

IMAGE Image;
/******************************************************************************
function:  Create Image
parameter:
    image   :   Pointer to the image cache
    width   :   The width of the picture
    height  :   The height of the picture
    Color   :   Whether the picture is inverted
******************************************************************************/

void Image_Init(uint8_t *image, uint16_t width, uint16_t height, uint16_t Rotate, uint16_t Color)
{
    Image.img = NULL;
    Image.img = image;

    Image.mem_w = width;
    Image.mem_h = height;
    Image.color = Color;    
    Image.byte_w = (width % 8 == 0)? (width / 8 ): (width / 8 + 1);
    Image.byte_h = height;    
    //printf("widthByte = %d, heightByte = %d\r\n", Image.byte_w, Image.byte_h);
    //printf(" EPD_width / 8 = %d\r\n",  122 / 8);
   
    Image.rotate = Rotate;
    Image.mirror = MIRROR_NONE;
    
    if(Rotate == ROTATE_0 || Rotate == ROTATE_180) 
    {
        Image.w = width;
        Image.h = height;
    } 
    else 
    {
        Image.w = height;
        Image.h = width;
    }
}

/******************************************************************************
function: Select Image
parameter:
    image   :   Pointer to the image cache
******************************************************************************/
void Gui_SelectImage(uint8_t *image)
{
    Image.img = image;
}

/******************************************************************************
function: Select Image Rotate
parameter:
    Rotate   :   0,90,180,270
******************************************************************************/
void Gui_SetRotate(uint16_t rotate)
{
    if(rotate == ROTATE_0 || rotate == ROTATE_90 || rotate == ROTATE_180 || rotate == ROTATE_270) 
    {
        //Debug("Set image Rotate %d\r\n", Rotate);
        Image.rotate = rotate;
    } 
    else 
    {
        //Debug("rotate = 0, 90, 180, 270\r\n");
    }
}

/******************************************************************************
function: Select Image mirror
parameter:
mirror   :       Not mirror,Horizontal mirror,Vertical mirror,Origin mirror
******************************************************************************/
void Gui_SetMirror(uint8_t mirror)
{
    if(mirror == MIRROR_NONE || mirror == MIRROR_HORIZONTAL || 
        mirror == MIRROR_VERTICAL || mirror == MIRROR_ORIGIN) 
    {
        //Debug("mirror image x:%s, y:%s\r\n",(mirror & 0x01)? "mirror":"none", ((mirror >> 1) & 0x01)? "mirror":"none");
        Image.mirror = mirror;
    } 
    else 
    {
        //Debug("mirror should be MIRROR_NONE, MIRROR_HORIZONTAL, \
        MIRROR_VERTICAL or MIRROR_ORIGIN\r\n");
    }    
}

/******************************************************************************
function: Draw Pixels
parameter:
       x  :   At point X
       x  :   At point Y
    color :   Guied colors
******************************************************************************/
void Gui_SetPixel(uint16_t x, uint16_t y, uint16_t color)
{
  uint16_t xx, yy;
  uint32_t Addr;
  uint8_t Rdata;
    if(x > Image.w || y > Image.h)
    {
        //Debug("Exceeding display boundaries\r\n");
        return;
    }      
    
    switch(Image.rotate) 
    {
        case 0:
            xx = x;
            yy = y;  
            break;
        case 90:
            xx = Image.mem_w - y - 1;
            yy = x;
            break;
        case 180:
            xx = Image.mem_w - x - 1;
            yy = Image.mem_h - y - 1;
            break;
        case 270:
            xx = y;
            yy = Image.mem_h - x - 1;
            break;
        
        default:
            return;
    }
    
    switch(Image.mirror) 
    {
        case MIRROR_NONE:
            break;
        case MIRROR_HORIZONTAL:
            xx = Image.mem_w - xx - 1;
            break;
        case MIRROR_VERTICAL:
            yy = Image.mem_h - yy - 1;
            break;
        case MIRROR_ORIGIN:
            xx = Image.mem_w - xx - 1;
            yy = Image.mem_h - yy - 1;
            break;
        default:
            return;
    }

    if(xx > Image.mem_w || yy > Image.mem_h)
    {
        //Debug("Exceeding display boundaries\r\n");
        return;
    }
    
    Addr = xx / 8 + yy * Image.byte_w;
    Rdata = Image.img[Addr];
    if(color == BLACK)
        Image.img[Addr] = Rdata & ~(0x80 >> (xx % 8));
    else
        Image.img[Addr] = Rdata | (0x80 >> (xx % 8));
}

/******************************************************************************
function: Clear the color of the picture
parameter:
Color   :   Guied colors
******************************************************************************/
void Gui_Clear(uint16_t color)
{
    uint16_t x,y;
    uint32_t addr;
    for (y = 0; y < Image.byte_h; y++) 
    {
        for (x = 0; x < Image.byte_w; x++) 
        {//8 pixel =  1 byte
            addr = x + y*Image.byte_w;
            Image.img[addr] = color;
        }
    }
}

/******************************************************************************
function: Clear the color of a window
parameter:
    x1 :   x starting point
    y1 :   Y starting point
    x2   :   x end point
    y2   :   y end point
******************************************************************************/
void Gui_ClearWindows(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t x, y;
    for (y = y1; y < y2; y++) 
    {
        for (x = x1; x < x2; x++) 
        {//8 pixel =  1 byte
            Gui_SetPixel(x, y, color);
        }
    }
}

/******************************************************************************
function: Draw Point(x, y) Fill the color
parameter:
    x   :   The xpoint coordinate of the point
    y   :   The ypoint coordinate of the point
    color   :   Set color
    Dot_Pixel : point size
******************************************************************************/
void Gui_Draw_Point(uint16_t x, uint16_t y, uint16_t color, DOT_PIXEL Dot_Pixel, DOT_STYLE Dot_Style)
{
    int16_t xd , yd;
    if (x > Image.w || y > Image.h) 
    {
        //Debug("Gui_Draw_Point Input exceeds the normal display range\r\n");
        return;
    }
    if (Dot_Style == AROUND) 
    {
        for (xd = 0; xd < 2 * Dot_Pixel - 1; xd++) 
        {
            for (yd = 0; yd < 2 * Dot_Pixel - 1; yd++) 
            {
                if(x + xd - Dot_Pixel < 0 || y + yd - Dot_Pixel < 0)
                    break;
                Gui_SetPixel(x + xd - Dot_Pixel, y + yd - Dot_Pixel, color);
            }
        }
    } 
    else 
    {
        for (xd = 0; xd <  Dot_Pixel; xd++) 
        {
            for (yd = 0; yd <  Dot_Pixel; yd++) 
            {
                Gui_SetPixel(x + xd - 1, y + yd - 1, color);
            }
        }
    }
}

/******************************************************************************
function: Draw a line of arbitrary slope use Bresenham algorithm 
parameter:
    x1 ：Starting Xpoint point coordinates
    y1 ：Starting Ypoint point coordinates
    x2   ：End point Xpoint coordinate
    y2   ：End point Ypoint coordinate
    color  ：The color of the line segment
******************************************************************************/
void Gui_Draw_Line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                    uint16_t color, DOT_PIXEL Dot_Pixel, LINE_STYLE style )
{ 
    uint16_t x = x1;
    uint16_t y = y1;
    int dx = x2>=x1?x2-x1:x1-x2;
    int dy = y2>=y1?y2-y1:y1-y2;
    int e2;
    int p = dx-dy;
    int xdir = x1>x2?-1:1;
    int ydir = y1>y2?-1:1;
    int dot_cnt = 0;
    for(;;)
    {
        dot_cnt++;
        if(style==DOTTED && dot_cnt%3==0)
        {
            Gui_Draw_Point(x, y, color, Dot_Pixel, AROUND);
        }
        else if(style==SOLID)
        {
            Gui_Draw_Point(x, y, color, Dot_Pixel, AROUND);
        }
        if(x==x2 && y==y2)
        {
            break;
        }
        e2 = 2*p;
        if(e2>= -dy)
        {
            p -= dy;
            x += xdir;
        }
        if(e2<=dx)
        {
            p += dx;
            y += ydir;
        }
    }
}

/******************************************************************************
function: Draw a rectangle
parameter:
    x1 ：Rectangular  Starting Xpoint point coordinates
    y1 ：Rectangular  Starting Ypoint point coordinates
    x2   ：Rectangular  End point Xpoint coordinate
    y2   ：Rectangular  End point Ypoint coordinate
    color  ：The color of the Rectangular segment
    filled : Whether it is filled--- 1 solid 0：empty
******************************************************************************/
void Gui_Draw_Rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                         uint16_t color, DRAW_FILL filled, DOT_PIXEL Dot_Pixel)
{
  uint16_t i;
    if (x1 > Image.w || y1 > Image.h || x2 > Image.w || y2 > Image.h) 
    {
        //Debug("Input exceeds the normal display range\r\n");
        return;
    }

    if (filled == FULL ) 
    {
        for(i = y1; i < y2; i++) 
        {
            Gui_Draw_Line(x1, i, x2, i, color ,Dot_Pixel, SOLID);
        }
    } 
    else 
    {
        Gui_Draw_Line(x1, y1, x2, y1, color, Dot_Pixel, SOLID);
        Gui_Draw_Line(x1, y1, x1, y2, color, Dot_Pixel, SOLID);
        Gui_Draw_Line(x2, y1, x2, y2, color, Dot_Pixel, SOLID);
        Gui_Draw_Line(x1, y2, x2, y2, color, Dot_Pixel, SOLID);
    }
}

/******************************************************************************
function: Use Bresenham  algorithm to draw a circle of the
            specified size at the specified position->
parameter:
    x  ：Center X coordinate
    y  ：Center Y coordinate
    radius    ：circle radius
    color     ：The color of the ：circle segment
    filled    : Whether it is filled: 1 filling 0：Do not
******************************************************************************/
void Gui_Draw_Circle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color, DRAW_FILL fill , DOT_PIXEL Dot_Pixel)
{
    uint16_t dx=0,dy=radius;
    int d = 1-radius;
    uint16_t i;
    if(x>Image.w || y > Image.h )
    {
        printf("ERR:circle center point out of screen area!\r\n");
        return ;
    } 
    while(dy>dx)
    {
        if(fill==EMPTY)
        {
            Gui_Draw_Point(x+dx, y+dy, color, Dot_Pixel, AROUND);
            Gui_Draw_Point(x+dy, y+dx, color, Dot_Pixel, AROUND);
            Gui_Draw_Point(x-dx, y+dy, color, Dot_Pixel, AROUND);
            Gui_Draw_Point(x-dy, y+dx, color, Dot_Pixel, AROUND);
            Gui_Draw_Point(x-dx, y-dy, color, Dot_Pixel, AROUND);
            Gui_Draw_Point(x-dy, y-dx, color, Dot_Pixel, AROUND);
            Gui_Draw_Point(x+dx, y-dy, color, Dot_Pixel, AROUND);
            Gui_Draw_Point(x+dy, y-dx, color, Dot_Pixel, AROUND);
        }
        else if(fill==FULL)
        {
            for(i=dx;i<=dy;i++)
            {
                Gui_Draw_Point(x+dx, y+i, color, Dot_Pixel, AROUND);
                Gui_Draw_Point(x+i, y+dx, color, Dot_Pixel, AROUND);
                Gui_Draw_Point(x-dx, y+i, color, Dot_Pixel, AROUND);
                Gui_Draw_Point(x-i, y+dx, color, Dot_Pixel, AROUND);
                Gui_Draw_Point(x-dx, y-i, color, Dot_Pixel, AROUND);
                Gui_Draw_Point(x-i, y-dx, color, Dot_Pixel, AROUND);
                Gui_Draw_Point(x+dx, y-i, color, Dot_Pixel, AROUND);
                Gui_Draw_Point(x+i, y-dx, color, Dot_Pixel, AROUND);                
            }
        }
        if(d<0)
        {
            d += 2*dx+3;    
        }
        else
        {
            d += 2*(dx-dy)+5;
            dy--;
        }
        dx++;
    }
}

/******************************************************************************
function: Show English characters
parameter:
    x           ：X coordinate
    y           ：Y coordinate
    Acsii_Char       ：To display the English characters
    Font             ：A structure pointer that displays a character size
    Color_Background : Select the background color of the English character
    Color_Foreground : Select the foreground color of the English character
******************************************************************************/
void Gui_Draw_Char(uint16_t x, uint16_t y, const char str_char,
                    FONT* Font, uint16_t Color_Background, uint16_t Color_Foreground)
{
    uint16_t row, col_bit;
    uint32_t  offset;
    uint16_t char_nbytes;
    const unsigned char *ptr;

    if (x > Image.w || y > Image.h) {
        //Debug("Gui_draw_char Input exceeds the normal display range\r\n");
        return;
    }

    //the number of bytes occupied by a character
    char_nbytes = Font->h * (Font->w / 8 + (Font->w % 8 ? 1 : 0));
    //the offset of the character
    offset = (str_char - ' ') * char_nbytes;
    ptr = &Font->table[offset];
    for (row = 0; row < Font->h; row ++) 
    {
        for (col_bit = 0; col_bit < Font->w; col_bit++ )
        {
            if (FONT_BACKGROUND == Color_Background) 
            {
                if (pgm_read_byte(ptr) & (0x80 >> (col_bit % 8)))
                    Gui_SetPixel(x+col_bit, y+row, Color_Foreground); 
            }
            else
            {
                if (pgm_read_byte(ptr) & (0x80 >> (col_bit % 8)))
                    Gui_SetPixel(x+col_bit, y+row, Color_Foreground);
                else
                    Gui_SetPixel(x+col_bit, y+row, Color_Background); 
                                
            }
            if (col_bit % 8 == 7) //next byte
                ptr++;
        }
        if(Font->w % 8 != 0)// next line
            ptr++;
    }
}

void Gui_Draw_Str(uint16_t x, uint16_t y, const char *str_char, FONT* Font, uint16_t Color_Background, uint16_t Color_Foreground )
{
    uint16_t dx = x,dy = y;
    while(*str_char != '\0')
    {
        if((dx+Font->w)>Image.w)//wrap the line if the x direction has reached the boundary
        {
            dx = x;
            dy += Font->h;
        }
        if((dy+Font->h)>Image.h)//wrap the line if the y direction has reached the boundary
        {
            dy = y;
            dx += Font->w;
        }
        Gui_Draw_Char(dx, dy, *str_char++, Font, Color_Background, Color_Foreground);
        dx += Font->w;
    }

}

/******************************************************************************
function: Display nummber
parameter:
    Xstart           ：X coordinate
    Ystart           : Y coordinate
    Nummber          : The number displayed
    Font             ：A structure pointer that displays a character size
    Color_Background : Select the background color of the English character
    Color_Foreground : Select the foreground color of the English character
******************************************************************************/
#define  ARRAY_LEN 255
void Gui_Draw_Num(uint16_t x, uint16_t y, int32_t nummber,
                   FONT* Font, uint16_t Color_Background, uint16_t Color_Foreground )
{

    int16_t Num_Bit = 0, Str_Bit = 0;
    uint8_t Str_Array[ARRAY_LEN] = {0}, Num_Array[ARRAY_LEN] = {0};
    uint8_t *pStr = Str_Array;

    if (x > Image.w || y > Image.h) 
    {
        //Debug("Gui_DisNum Input exceeds the normal display range\r\n");
        return;
    }

    //Converts a number to a string
    while (nummber) 
    {
        Num_Array[Num_Bit] = nummber % 10 + '0';
        Num_Bit++;
        nummber /= 10;
    }

    //The string is inverted
    while (Num_Bit > 0) 
    {
        Str_Array[Str_Bit] = Num_Array[Num_Bit - 1];
        Str_Bit ++;
        Num_Bit --;
    }

    //show
    Gui_Draw_Str(x, y, (const char*)pStr, Font, Color_Background, Color_Foreground);
}

/******************************************************************************
function: Display time
parameter:
    x1           ：X coordinate
    y1           : Y coordinate
    pTime            : Time-related structures
    Font             ：A structure pointer that displays a character size
    Color            : Select the background color of the English character
******************************************************************************/
void Gui_draw_time(uint16_t x1, uint16_t y1, DRAW_TIME *pTime, FONT* Font,
                    uint16_t Color_Background, uint16_t Color_Foreground)
{
    uint8_t value[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    uint16_t dx = Font->w;
    
    //Write data into the cache
    Gui_Draw_Char(x1                           , y1, value[pTime->Hour / 10], Font, Color_Background, Color_Foreground);
    Gui_Draw_Char(x1 + dx                      , y1, value[pTime->Hour % 10], Font, Color_Background, Color_Foreground);
    Gui_Draw_Char(x1 + dx  + dx / 4 + dx / 2   , y1, ':'                    , Font, Color_Background, Color_Foreground);
    Gui_Draw_Char(x1 + dx * 2 + dx / 2         , y1, value[pTime->Min / 10] , Font, Color_Background, Color_Foreground);
    Gui_Draw_Char(x1 + dx * 3 + dx / 2         , y1, value[pTime->Min % 10] , Font, Color_Background, Color_Foreground);
    Gui_Draw_Char(x1 + dx * 4 + dx / 2 - dx / 4, y1, ':'                    , Font, Color_Background, Color_Foreground);
    Gui_Draw_Char(x1 + dx * 5                  , y1, value[pTime->Sec / 10] , Font, Color_Background, Color_Foreground);
    Gui_Draw_Char(x1 + dx * 6                  , y1, value[pTime->Sec % 10] , Font, Color_Background, Color_Foreground);
}

/******************************************************************************
function: Display monochrome bitmap
parameter:
    image_buffer ：A picture data converted to a bitmap
info:
    Use a computer to convert the image into a corresponding array,
    and then embed the array directly into Imagedata.cpp as a .c file.
******************************************************************************/
void Gui_DrawBitMap(const unsigned char* image_buffer)
{
    uint16_t x, y;
    uint32_t Addr = 0;

    for (y = 0; y < Image.byte_h; y++) 
    {
        for (x = 0; x < Image.byte_w; x++) 
        {//8 pixel =  1 byte
            Addr = x + y * Image.byte_w;
            Image.img[Addr] = (unsigned char)image_buffer[Addr];
        }
    }
}

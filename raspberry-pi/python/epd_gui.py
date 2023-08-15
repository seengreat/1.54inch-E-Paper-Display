import os
import sys
import numpy as np
from epd_1inch54 import *
from PIL import Image, ImageFont

FILL_EMPTY = 0
FILL_FULL = 1

LINE_SOLID = 0
LINE_DOTTED = 1
FONT_SIZE_16 = 16
FONT_SIZE_20 = 20
FONT_SIZE_24 = 24
FONT_SIZE_28 = 28

PIXEL_1X1 = 1  # 1x1
PIXEL_2X2 = 2  # 2X2
PIXEL_3X3 = 3  # 3X3
PIXEL_4X4 = 4  # 4X4
PIXEL_5X5 = 5  # 5X5
PIXEL_6X6 = 6  # 6X6
PIXEL_7X7 = 7  # 7X7
PIXEL_8X8 = 8  # 8X8
DOT_PIXEL_DFT = PIXEL_1X1

WHITE = 0xFF
BLACK = 0x00
RED = BLACK
IMAGE_BACKGROUND = WHITE
FONT_FOREGROUND = BLACK
FONT_BACKGROUND = WHITE

MIRROR_NONE = 0x00
MIRROR_HORIZONTAL = 0x01
MIRROR_VERTICAL = 0x02
MIRROR_ORIGIN = 0x03

ROTATE_0 = 0
ROTATE_90 = 90
ROTATE_180 = 180
ROTATE_270 = 270

AROUND = 1  # dot pixel 1x1
RIGHTUP = 2  # dot pixel 2X2
DOT_STYLE_DFT = AROUND


class EPD_GUI():
    def __init__(self):
        self.epd = EPD_1Inch54()
        self.epd.reset()
        self.epd.hw_init()
        self.img = [0xff for i in range(5000)]
        self.mem_w = EPD_WIDTH
        self.mem_h = EPD_HEIGHT
        self.color = WHITE
        self.rotate = 270
        self.mirror = MIRROR_NONE
        if EPD_WIDTH % 8 == 0:
            self.byte_w = EPD_WIDTH//8
        else:
            self.byte_w = (EPD_WIDTH // 8) + 1
        self.byte_h = EPD_HEIGHT
        if self.rotate == ROTATE_0 or self.rotate == ROTATE_180:
            self.w = EPD_WIDTH
            self.h = EPD_HEIGHT
        else:
            self.w = EPD_HEIGHT
            self.h = EPD_WIDTH

    def set_pixel(self, x, y, color):
        xx = 0
        yy = 0
        if x > self.w or y > self.h:
            print("Exceeding display boundaries")
            return
        if self.rotate == 0:
            xx = x
            yy = y
        elif self.rotate == 90:
            xx = self.mem_w - y - 1
            yy = x
        elif self.rotate == 180:
            xx = self.mem_w - x - 1
            yy = self.mem_h - y - 1
        elif self.rotate == 270:
            xx = y
            yy = self.mem_h - x - 1
        else:
            xx = x
            yy = y
        if self.mirror == MIRROR_HORIZONTAL:
            xx = self.mem_w - xx - 1
        elif self.mirror == MIRROR_VERTICAL:
            yy = self.mem_h - yy - 1
        elif self.mirror == MIRROR_ORIGIN:
            xx = self.mem_w - xx - 1
            yy = self.mem_h - yy - 1

        if xx > self.mem_w or yy > self.mem_h:
            print("Exceeding display boundaries")
            return
        addr = xx // 8 + yy * self.byte_w
        rdata = self.img[addr]
        if color == BLACK:
            self.img[addr] = rdata & ~(0x80 >> (xx % 8))
        else:
            self.img[addr] = rdata | (0x80 >> (xx % 8))
    
    def clear(self, color):
        for y in range(self.byte_h):
            for x in range(self.byte_w):
                # 8 pixel =  1 byte
                addr = x + y * self.byte_w
                self.img[addr] = color
            
    def draw_point(self, x, y, color, dot_pixel, dot_style):
        if x > self.w or y > self.h:
            print("Gui_Draw_Point Input exceeds the normal display range")
            return
        if dot_style == AROUND:
            for xd in range(2 * dot_pixel - 1):
                for yd in range(2 * dot_pixel - 1):
                    if (x + xd - dot_pixel < 0) or (y + yd - dot_pixel < 0):
                        break
                    self.set_pixel(x + xd - dot_pixel, y + yd - dot_pixel, color)
        else:
            for xd in range(dot_pixel):
                for yd in range(dot_pixel):
                    self.set_pixel(x + xd - 1, y + yd - 1, color)

    '''Bresenham algorithm draw line'''
    def draw_line(self, x1, y1, x2, y2, color, dot_pixel, line_type):
        x = x1
        y = y1
        dx = x2-x1 if x2-x1 >= 0 else x1-x2
        dy = y2-y1 if y2-y1 >= 0 else y1-y2
        p = dx-dy
        
        xdir = -1 if x1 > x2 else 1
        ydir = -1 if y1 > y2 else 1
        dot_cnt = 0
        while True:
            dot_cnt += 1
            if line_type == LINE_DOTTED and dot_cnt%3 == 0:
                self.draw_point(x, y, color, dot_pixel, AROUND)
            elif line_type == LINE_SOLID:
                self.draw_point(x, y, color, dot_pixel, AROUND)
            if x == x2 and y == y2:
                break
            e2 = 2*p
            if e2 >= -dy:
                p -= dy
                x += xdir
            if e2 <= dx:
                p += dx
                y += ydir
        
    def draw_rectangle(self, x1, y1, x2, y2, color, fill, dot_pixel):
        if x1 > self.w or y1 > self.h or x2 > self.w or y2 > self.h:
            print("ERR:line point out of screen area!")
            return 0
        if fill == FILL_FULL:
            for i in range(y1, y2):
                self.draw_line(x1, i, x2, i, color, dot_pixel, LINE_SOLID)
        elif fill == FILL_EMPTY:
            self.draw_line(x1, y1, x2, y1, color, dot_pixel, LINE_SOLID)
            self.draw_line(x1, y1, x1, y2, color, dot_pixel, LINE_SOLID)
            self.draw_line(x1, y2, x2, y2, color, dot_pixel, LINE_SOLID)
            self.draw_line(x2, y1, x2, y2, color, dot_pixel, LINE_SOLID)
            
    '''Bresenham  algorithm draw circle'''
    def draw_circle(self, x, y, r, color, fill, dot_pixel):
        dx = 0
        dy = r
        d = 1-r
        if x > self.w or y > self.h:
            print("ERR:circle center point out of screen area!")
            return 0
        while dy > dx:
            if fill == FILL_EMPTY:
                self.draw_point(x+dx, y+dy, color, dot_pixel, AROUND)
                self.draw_point(x+dy, y+dx, color, dot_pixel, AROUND)
                self.draw_point(x-dx, y+dy, color, dot_pixel, AROUND)
                self.draw_point(x-dy, y+dx, color, dot_pixel, AROUND)
                self.draw_point(x-dx, y-dy, color, dot_pixel, AROUND)
                self.draw_point(x-dy, y-dx, color, dot_pixel, AROUND)
                self.draw_point(x+dx, y-dy, color, dot_pixel, AROUND)
                self.draw_point(x+dy, y-dx, color, dot_pixel, AROUND)
            elif fill == FILL_FULL:
                for i in range(dx, dy):
                    self.draw_point(x+dx, y+i, color, dot_pixel, AROUND)
                    self.draw_point(x+i, y+dx, color, dot_pixel, AROUND)
                    self.draw_point(x-dx, y+i, color, dot_pixel, AROUND)
                    self.draw_point(x-i, y+dx, color, dot_pixel, AROUND)
                    self.draw_point(x-dx, y-i, color, dot_pixel, AROUND)
                    self.draw_point(x-i, y-dx, color, dot_pixel, AROUND)
                    self.draw_point(x+dx, y-i, color, dot_pixel, AROUND)
                    self.draw_point(x+i, y-dx, color, dot_pixel, AROUND)
            if d < 0:
                d += 2*dx+3
            else:
                d += 2*(dx-dy)+5
                dy -= 1
            dx += 1

    def draw_str(self, x, y, text_str, color, font_size, font):
        str_list = list(font.getmask(text=text_str, mode="1"))
        str_size = font.getsize(text=text_str)
        font_act_h = int(len(str_list)/(str_size[0]))
        font_h = font_size
        top_add_row = int((font_h-font_act_h)/2)
        bot_add_row = int((font_h-font_act_h)) - top_add_row 
        for i in range(0, top_add_row*str_size[0]):
            str_list.insert(0, 0)
        for i in range(0, bot_add_row*str_size[0]):
            str_list.append(0)
        for i in range(len(str_list)):
            if str_list[i] == 0:
                str_list[i] = 1
            else:
                str_list[i] = 0
        n = 0
        for i in range(y-1, y+font_h-1):
            for j in range(x-1, x+str_size[0]-1):
                if str_list[n] == 0:
                    self.set_pixel(j, i, color)
                n += 1


from PIL import ImageDraw, Image, ImageFont
import time
from epd_1inch54 import *
from epd_gui import *
from image import *
'''define pin conection in wiringpi number
1.54inch_EPD    Raspberry Pi
      VCC   --->   3.3V
      GND   --->   GND
      RST   --->   P0
      BUSY  --->   P5
      D/C   --->   P6
      MOSI  --->   MOSI
      CLK   --->   CLK
      CS    --->   (CE0)P10 
'''

if __name__ == '__main__':
    gui = EPD_GUI()
    gui.epd.hw_init_fast()
    gui.epd.whitescreen_all_fast(gImage_0)  # Refresh the picture in full screen
    gui.epd.sleep()  # EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    time.sleep(2)  # delay 2s

    gui.epd.hw_init()  # Electronic paper initialization
    gui.epd.whitescreen_all(gImage_1)  # Refresh the picture in full screen
    gui.epd.sleep()  # EPD_sleep,Sleep instruction is necessary, please do not delete!!!
    time.sleep(2)  # delay 2s


    class BreakLoop(Exception):
        pass
    gui.epd.hw_init()  # EPD init
    gui.epd.setramvalue_basemap(gImage_basemap)  # EPD Clear
    try:
        for fen_h in range(6):
            for fen_l in range(10):
                for miao_h in range(6):
                    for miao_l in range(10):
                        gui.epd.dis_part_myself(64, 40, Num[miao_l], 64, 72, Num[miao_h], 64, 112, gImage_numdot,
                                                64, 154, Num[fen_l], 64, 186, Num[fen_h], 32, 64)
                        if fen_l == 0 and miao_h == 1 and miao_l == 0:
                            raise BreakLoop()
    except BreakLoop:
        print("out of time show loop")
    gui.epd.hw_init()  # Electronic paper initialization
    gui.epd.whitescreen_white()
    # Drawing
    gui.epd.hw_init_gui()  # EPD init
    gui.clear(WHITE)
    gui.epd.display(gui.img)
    font_16 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_16)  # read chinese font file
    font_20 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_20)  # read chinese font file
    font_24 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_24)  # read chinese font file
    font_28 = ImageFont.truetype("MiSans-Light.ttf", FONT_SIZE_28)  # read chinese font file
    # Point
    gui.draw_point(5, 10, BLACK, PIXEL_1X1, DOT_STYLE_DFT)
    gui.draw_point(5, 25, BLACK, PIXEL_2X2, DOT_STYLE_DFT)
    gui.draw_point(5, 40, BLACK, PIXEL_3X3, DOT_STYLE_DFT)
    gui.draw_point(5, 55, BLACK, PIXEL_4X4, DOT_STYLE_DFT)
    # Line
    gui.draw_line(15, 10, 55, 50, BLACK, PIXEL_1X1, LINE_SOLID)
    gui.draw_line(15, 50, 55, 10, BLACK, PIXEL_1X1, LINE_SOLID)
    # Rectangle
    gui.draw_rectangle(15, 10, 55, 50, BLACK, FILL_EMPTY, PIXEL_1X1)
    gui.draw_rectangle(70, 10, 110, 50, BLACK, FILL_FULL, PIXEL_1X1)
    # Circle
    gui.draw_circle(135, 30, 18, BLACK, FILL_EMPTY, PIXEL_1X1)
    gui.draw_circle(176, 30, 18, BLACK, FILL_FULL, PIXEL_1X1)
    gui.draw_str(10, 75, "abcdefg", BLACK, FONT_SIZE_16, font_16)
    gui.draw_str(10, 100, "ABCabc012345", BLACK, FONT_SIZE_20, font_20)
    gui.draw_str(10, 130, "1.54\" E-Paper", BLACK, FONT_SIZE_24, font_24)
    gui.draw_str(10, 160, "SEENGREAT", BLACK, FONT_SIZE_28, font_28)
    # TEST_PIN = 1
    gui.epd.display(gui.img)  # display image need 2.08s

    gui.epd.sleep()  # EPD_DeepSleep, Sleep instruction is necessary, please do not delete!!!
    time.sleep(3)  # 3s

    # Clear screen
    gui.epd.hw_init()  # EPD init  initialization
    gui.epd.whitescreen_white()  # Show all white
    gui.epd.sleep()  # Enter deep sleep, Sleep instruction is necessary, please do not delete!!!
    time.sleep(2)

    while True:
        pass

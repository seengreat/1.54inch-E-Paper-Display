1.54inch E-Paper Display from seengreat:www.seengreat.com
 =======================================
# Instructions
## Product Overview
This product is a 1.54-inch e-ink display expansion module suitable for Raspberry Pi series boards, Arduino, STM32, and more. We provide example programs for both the C and Python versions on Raspberry Pi, as well as example programs for Arduino and STM32. These example programs enable drawing of dots, lines, rectangles, and circles, and they also allow displaying English letters, numbers, and images.<br>
## Product parameters
|Dimensions	|48mm (Length) * 33mm (Width)|
|----------------------|-----------------------------------|
|Pixels	|200*200|
|Display Color	|Monochrome|
|Voltage Translator	|TXS0108EPWR|
|Signal Interface	|SPI|
|Power Supply Voltage	|5V/3.3V|
|LCD Display Area	|27mm (H) * 27mm (W)|
|Grayscale |4|
|Full Refresh Time	|2S|
|Fast Refresh Time	|1.5S|
|Partial Refresh Time	|0.26S|
|Power Consumption	|4.5nW|
|Standby Power Consumption	|0.003mW |
## Product dimensions
65mm(Length)*30mm(width)<br>
## Pin Definitions
|VCC	|3.3V&5V|
|-----------|----------|
|GND	|Power Ground|
|RST	|External Reset Pin (Low level for reset)|
|BUSY	|Busy Status Output Pin (Low level indicates busy)|
|D/C	|Data/Command Control Pin (High level for data, low level for command)|
|MOSI	|SPI Communication MOSI Pin|
|CS	|SPI Chip Select Pin (Low level active)|
|CLK	|SPI Communication SCK Pin|
# Usage
## Raspberry Pi demo codes usage
### Hardware interface configuration description
The example program for the Raspberry Pi uses the pin definitions numbered in wiringPi. The pin connections on the Raspberry Pi board are defined as follows in the table below：<br>
|E-Ink Display Interface	|WiringPi number	|BCM number|
|----------------------------------|----------------------|---------------|
|VCC	|3.3V	|3.3V|
|GND	|GND	|GND|
|RST	|P0	|17|
|BUSY	|P5	|24|
|D/C 	|P6	|25|
|MOSI	|MOSI/P12	|10|
|CLK	|SCLK/P14	|11|
|CS	|CE0/P10	|8|
### Demo Codes Usage	
#### Wiringpi library installation
sudo apt-get install wiringpi<br>
   wget https://project-downloads.drogon.net/wiringpi-latest.deb  # Version 4B upgrade of Raspberry Pi<br>
   sudo dpkg -i wiringpi-latest.deb<br>
   gpio -v # If version 2.52 appears, the installation is successful<br>
#For the Bullseye branch system, use the following command:<br>
git clone https://github.com/WiringPi/WiringPi<br>
cd WiringPi<br>
./build<br>
gpio -v<br>
#Running gpio - v will result in version 2.70. If it does not appear, it indicates an installation error<br>
If the error prompt "ImportError: No module named 'wiringpi'" appears when running the python version of the sample program, run the following command<br>
#For Python 2. x version<br>
pip install wiringpi<br>
 
#For Python version 3. X<br>
pip3 install wiringpi<br>
Note: If the installation fails, you can try the following compilation and installation:<br>
git clone --recursive https://github.com/WiringPi/WiringPi-Python.git<br>
Note: The -- recursive option can automatically pull the submodule, otherwise you need to download it manually.<br>
Enter the WiringPi Python folder you just downloaded, enter the following command, compile and install:<br>
#For Python 2. x version<br>
sudo python setup.py install <br>
#For Python version 3. X<br>
sudo python3 setup.py install<br>
If the following error occurs:<br>
"Error:Building this module requires either that swig is installed<br>
        (e.g.,'sudo apt install swig') or that wiringpi_wrap.c from the<br>
        source distribution (on pypi) is available."<br>
At this time, enter the command sudo apt install swig to install swig. After that, compile and install sudo python3 setup.py install. If a message similar to the following appears, the installation is successful.<br>
"ges<br>
Adding wiringpi 2.60.0 to easy-install.pth file<br>
Installed /usr/local/lib/python3.7/dist-packages/wiringpi-2.60.0-py3.7-linux-armv7<br>
Processing dependencies for wiringpi==2.60.0<br>
Finished processing dependencies for wiringpi==2.60.0"<br>
#### Open SPI interface
sudo raspi-config<br>
Enable SPI interface:<br>
Interfacing Options > SPI > Yes<br>
To view enabled SPI devices:<br>
ls /dev/spi * # The following will be printed: "/dev/spidev0.0" and "/dev/spidev0.1"<br>
#### Installation of python library
The demo codes uses the python 3 environment. To run the python demo codes, you need to install the pil, numpy, and spiderv libraries. Enter the following commands in order to install:<br>
sudo apt-get install python3-pil<br>
sudo apt-get install python3-numpy<br>
sudo apt-get install python3-pip<br>
sudo pip3 install spidev<br>
#### C version demo codes
Navigate to the C directory in the example program project folder:<br>
sudo make clean<br>
sudo make<br>
sudo ./main<br>
After entering the above command, you can observe the E-Ink display.<br>
#### python Version demo codes
Navigate to the Python directory in the example program project folder:<br>
python3 gui_demo.py<br>
After entering the above command, you can observe the E-Ink display.<br>
## Arduino MEGA Example Program Usage
### Hardware interface configuration description
|E-Ink display	|Arduino|
|----------------------|--------|
|VCC	|5V|
|GND	|GND|
|RST	|D9|
|BUSY	|D10|
|D/C 	|D8|
|MOSI	|D51|
|CLK	|D52|
|CS	|D53|
### Demo Codes Usage
Open the example program file using the Arduino IDE software. Click on "Verify" to confirm that there are no errors. Once verified, upload the program to the module and observe the E-Ink display.<br>
## Arduino UNO Example Program Usage
### Hardware Interface Configuration Instructions
|E-Ink Display Interface	|Arduino Interface|
|----------------------------------|-------------------|
|VCC	|5V|
|GND	|GND|
|RST	|D9|
|BUSY	|D8|
|D/C 	|D10|
|MOSI	|D13|
|CLK	|D12|
|CS	|D11|
### Example Program Usage
Open the example program file using the Arduino IDE software. Click on "Verify" to ensure there are no errors, and once verified, upload the program to the module. Then, observe the E-Ink display.<br>
## STM32 Demo Codes Usage
### Hardware interface configuration description
|E-Ink display|	STM32|
|----------------------|-------|
|VCC	|3.3V|
|GND	|GND|
|RST	|PA11|
|BUSY	|PA12|
|D/C 	|PA8|
|MOSI	|PB15|
|CLK	|PB13|
|CS	|PB12|
### Demo Codes Usage
Open the routine in directory 2.13inch E-Paper Display\demo codes\STM32 with Keil uVision5 software, compile it correctly, download it to the module, and observe the E-Ink display.<br>
## Image Creation and Bitmap Explanation
Image Creation
Create the images you want to display as pure black and white images with a resolution of 200x200 (grayscale is not supported). Save them as BMP or JPG files (BMP format is recommended).<br>
### Bitmap Creation
Bitmap creation can be done using the "image2lcd" software, which is provided in the package. Taking the example of achieving the effect shown in Figure 2-2, the parameter settings interface for bitmap creation is shown in Figure 2-3:<br>
1、Open the image you want to create a bitmap for.<br>
2、Output data type: Select "C Language Array (*.c)".<br>
3、Scanning method: Choose "Vertical Scanning".<br>
4、Output grayscale: Select "Monochrome".<br>
5、Maximum width and height: Choose "200" for both width and height, and click the arrow behind to confirm.<br>
Uncheck all five options shown in the image below:<br>
<img src="https://github.com/seengreat/1.54inch-E-Paper-Display/blob/4c56316d8c96e1a3515cdc14cee421cd0390f86f/figure-1.png"><br>
7、Color Inversion: Check this box to display the original image; uncheck it for color inversion.<br>
8、Click "Save" to store the converted array in a file with the extension ".c".<br>
9、Finally, replace the corresponding array in the program code with the array from the ".c" file.<br>
<img src="https://github.com/seengreat/1.54inch-E-Paper-Display/blob/4c56316d8c96e1a3515cdc14cee421cd0390f86f/figure-2.png"><br>
                                 Figure 2-2<br>
<img src="https://github.com/seengreat/1.54inch-E-Paper-Display/blob/4c56316d8c96e1a3515cdc14cee421cd0390f86f/figure-3.png"><br>
Figure 2-3<br>

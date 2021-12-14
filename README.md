# PES-Final Project

This repo contains code to display shapes, images and animation on a 128x64 OLED display. 

Link to demo video and screenshots:
https://drive.google.com/drive/folders/1E_zTZ0C1VG2NjpBQO8Ci6BrM9ni8bUDk?usp=sharing

The OLED display used for this project is a 128x64 OLED display. It has SSD1315 as its OLED driver/controller. It uses I2C for communication. 
Initially I had planned using a 64x48 OLED display from sparkfun that used SPI. It stopped working and I had to switch to this display. I hace added links to both the displays. 

128x64 OLED:
https://www.amazon.com/gp/product/B08J2M5H8N/ref=ox_sc_act_title_1?smid=A2LI0UT8DGJN2T&psc=1\

64X48 OLED:
https://www.sparkfun.com/products/13003

I also used a software called Krita to reduce the resolution of the images and then convert them into arrays that I could directly use in the code. 
krita:
https://krita.org/en/

The program contains 3 main parts: Command processor  & UART, I2C, OLED display. 

Command Processor & UART:
	This code was taken from assignment 6 and modidifed. I added more user friendly commands and also used arguments for almost all the commands. 
	The supported commands can be accessed using the help command. These commands allow the user to display a shape, image or animation on the OLED display. 
	The UART works at the same configuration as for assignment 6. I let the processor work at 24MHz also. 
	
I2C: 
	This code gave me a little problem because the I2C peripheral couldn't be configured in the same was as we configure it in other boards. I'm currently using I2C 		at approx 400KHz since that is the max limit of the OLED display. I have used I2C1 for this project and its pinout is: SCK - PE0, SDA - PE1, Vcc -3.3v GND- 		 GND.

OLED display: 
	This was the trickiest part of the project. The documentation for the OLED driver wasn't thorough enough and there were various aspects of its working that 			weren't explained properly. The OLED display used display RAM that maps directly to the screen. Every bit on the display RAM maps to a pixel on the screen. 			The display RAM is divided into 128 vertical columns called 'cols' and 8 horizontal rows called 'pages'. Each page has 8 rows of pixel assigned to it. SO by 			writing a byte, to the display RAM, you can control the 8 pixel in a page and a col. 
	
The display uses 2048 bytes to control the entire screen. 
	
The program provides the following functionalities: 
All the commands are case insensitive.

1. Author <None> : 
 Arguments: None
 Action: It prints the name of the user

2. Help <None> : 
 Arguments: None
 Action: It prints information regarding all the supported commands.

3. Display <Shape> : 
 Arguments: String with the name of the object. Supported objects: square, rectangle, oval & fish.
 Action: It prints the object on the OLED display.

4. Print <Shape> : 
 Arguments: String with the name of the object. Supported objects: square, rectangle, oval & fish.
 Action: It prints the object on the OLED display.

5. Draw <Shape> : 
 Arguments: String with the name of the object. Supported objects: square & rectangle.
 Action: It draws the object on the OLED display pixel by pixel.

6. Show <Shape> : 
 Arguments: String with the name of the object. Supported objects: check_gif.
 Action: It shows the appropriate animation.

7. Clear <None> : 
 Arguments: None
 Action: Clears the OLED display

8. Fill <None> : 
 Arguments: None
 Action: Switches on all the pixels on the OLED display

9. Toggle_pixel <X co-ordinate> <y co-ordinate> <pixel state> : 
 Arguments: It takes the x co-ordinate, y co-ordinate and pixel state(ON/OFF).
 Action: It switches ON/OFF the relevent pixel on the OLED. (0,0) is at the bottom left corner. So all the pixels are in the 1st quadrant. Y goes from 0-31 and X   goes from 0-127.

To run this project, build and run the code. The screen needs to be connected to the KL25Z. If that doesn't happen, the code will be stuck in init_oled(), waiting for a ACK for the byte sent on the I2C bus. The serial terminal needs to be configured for 38400 baud rate, 2 stop bits and no parity bits. 

To use the project, simply enter the command on the serial terminal.
	
	

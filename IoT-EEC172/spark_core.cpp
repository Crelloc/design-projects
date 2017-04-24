/**
 ******************************************************************************
 * @file    application.cpp
 * @authors  Thomas Turner, Andrew Trinh
 * @version V1.0.0
***********************************************
  Copyright (c) 2013 Spark Labs, Inc.  All rights reserved.

  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation, either
  version 3 of the License, or (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/  
#include "application.h"

#define total_bits 25

int led_toggle = 1;
int GENERATE = 0;
int buffer[25];
int button_sum =0;
volatile int pc5=-1;
int assert_count=0;
int y = 0, z=0, hold=0, next=0, array_count = 0;
volatile int done = 0;
volatile int count = 0;
char letter[100];
volatile int letter_count=0;

const int CHup = 24823891;								//{3,3,1,1,3,1,3,1,1,1,1,3,1,1,3,3,1,3,1,3,3,3,3,1,3}
const int VOLup = 8050771;	//{3,3,1,1,3,1,3,1,1,1,1,3,3,1,3,3,1,3,1,3,3,3,3,1,1}
const int zero = 11719891;    	//{3,3,1,1,3,1,3,3,1,1,3,1,3,1,3,3,1,3,1,1,3,3,1,3,1};
const int one  = 16961491;  						  //{3,3,1,1,3,1,3,3,3,3,3,3,1,1,3,3,1,3,1,1,1,1,1,1,3};
const int two  = 15913171;    				    //{3,3,1,1,3,1,3,3,1,1,1,1,3,1,3,3,1,3,1,1,3,3,3,3,1};
const int three= 15389011;   					    //{3,3,1,1,3,1,3,1,3,1,1,1,3,1,3,3,1,3,1,3,1,3,3,3,1};
const int four = 14864851;    				    //{3,3,1,1,3,1,3,3,3,1,1,1,3,1,3,3,1,3,1,1,1,3,3,3,1};
const int five = 14340691;    					  //{3,3,1,1,3,1,3,1,1,3,1,1,3,1,3,3,1,3,1,3,3,1,3,3,1};
const int six  = 13816531;								//{3,3,1,1,3,1,3,3,1,3,1,1,3,1,3,3,1,3,1,1,3,1,3,3,1};
const int seven= 13292371;								//{3,3,1,1,3,1,3,1,3,3,1,1,3,1,3,3,1,3,1,3,1,1,3,3,1};
const int eight= 12768211;								//{3,3,1,1,3,1,3,3,3,3,1,1,3,1,3,3,1,3,1,1,1,1,3,3,1};
const int nine = 12244051;								//{3,3,1,1,3,1,3,1,1,1,3,1,3,1,3,3,1,3,1,3,3,3,1,3,1};
const int del  = 19058131;								//{3,3,1,1,3,1,3,3,3,1,3,3,1,1,3,3,1,3,1,1,1,3,1,1,3};

volatile int start = 0;
volatile int success = 0;
volatile int first = 0;
int val =LOW;
int test;
int del_count=0, space_count=0;
int okaytoprint;
/*
7-17-2011
Spark Fun Electronics 2011
Nathan Seidle
This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
This code writes a series of images and text to the Nokia 5110 84x48 graphic LCD:
http://www.sparkfun.com/products/10168
Do not drive the backlight with 5V. It will smoke. However, the backlight on the LCD seems to be
happy with direct drive from the 3.3V regulator.

Although the PCD8544 controller datasheet recommends 3.3V, the graphic Nokia 5110 LCD can run at 3.3V or 5V.
No resistors needed on the signal lines.
You will need 5 signal lines to connect to the LCD, 3.3 or 5V for power, 3.3V for LED backlight, and 1 for ground.
*/

#define PIN_SCE 7 //Pin 3 on LCD
#define PIN_RESET 6 //Pin 4 on LCD
#define PIN_DC 5 //Pin 5 on LCD
#define PIN_SDIN 4 //Pin 6 on LCD
#define PIN_SCLK 3 //Pin 7 on LCD

//The DC pin tells the LCD if we are sending a command or data
#define LCD_COMMAND 0
#define LCD_DATA 1

//You may find a different size screen, but this one is 84 by 48 pixels
#define LCD_X 84
#define LCD_Y 48

//This table contains the hex values that represent pixels
//for a font that is 5 pixels wide and 8 pixels high
static const byte ASCII[][5] = {
  {0x00, 0x00, 0x00, 0x00, 0x00} // 20
  ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 21 !
  ,{0x00, 0x07, 0x00, 0x07, 0x00} // 22 "
  ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 23 #
  ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 24 $
  ,{0x23, 0x13, 0x08, 0x64, 0x62} // 25 %
  ,{0x36, 0x49, 0x55, 0x22, 0x50} // 26 &
  ,{0x00, 0x05, 0x03, 0x00, 0x00} // 27 '
  ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 28 (
  ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 29 )
  ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 2a *
  ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 2b +
  ,{0x00, 0x50, 0x30, 0x00, 0x00} // 2c ,
  ,{0x08, 0x08, 0x08, 0x08, 0x08} // 2d -
  ,{0x00, 0x60, 0x60, 0x00, 0x00} // 2e .
  ,{0x20, 0x10, 0x08, 0x04, 0x02} // 2f /
  ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 30 0
  ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 31 1
  ,{0x42, 0x61, 0x51, 0x49, 0x46} // 32 2
  ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 33 3
  ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 34 4
  ,{0x27, 0x45, 0x45, 0x45, 0x39} // 35 5
  ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 36 6
  ,{0x01, 0x71, 0x09, 0x05, 0x03} // 37 7
  ,{0x36, 0x49, 0x49, 0x49, 0x36} // 38 8
  ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 39 9
  ,{0x00, 0x36, 0x36, 0x00, 0x00} // 3a :
  ,{0x00, 0x56, 0x36, 0x00, 0x00} // 3b ;
  ,{0x08, 0x14, 0x22, 0x41, 0x00} // 3c <
  ,{0x14, 0x14, 0x14, 0x14, 0x14} // 3d =
  ,{0x00, 0x41, 0x22, 0x14, 0x08} // 3e >
  ,{0x02, 0x01, 0x51, 0x09, 0x06} // 3f ?
  ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 40 @
  ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 41 A
  ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 42 B
  ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 43 C
  ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 44 D
  ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 45 E
  ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 46 F
  ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 47 G
  ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 48 H
  ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 49 I
  ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 4a J
  ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 4b K
  ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 4c L
  ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 4d M
  ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 4e N
  ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 4f O
  ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 50 P
  ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 51 Q
  ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 52 R
  ,{0x46, 0x49, 0x49, 0x49, 0x31} // 53 S
  ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 54 T
  ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 55 U
  ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 56 V
  ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 57 W
  ,{0x63, 0x14, 0x08, 0x14, 0x63} // 58 X
  ,{0x07, 0x08, 0x70, 0x08, 0x07} // 59 Y
  ,{0x61, 0x51, 0x49, 0x45, 0x43} // 5a Z
  ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 5b [
  ,{0x02, 0x04, 0x08, 0x10, 0x20} /* 5c \ */
  ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 5d ]
  ,{0x04, 0x02, 0x01, 0x02, 0x04} // 5e ^
  ,{0x40, 0x40, 0x40, 0x40, 0x40} // 5f _
  ,{0x00, 0x01, 0x02, 0x04, 0x00} // 60 `
  ,{0x20, 0x54, 0x54, 0x54, 0x78} // 61 a
  ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 62 b
  ,{0x38, 0x44, 0x44, 0x44, 0x20} // 63 c
  ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 64 d
  ,{0x38, 0x54, 0x54, 0x54, 0x18} // 65 e
  ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 66 f
  ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 67 g
  ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 68 h
  ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 69 i
  ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 6a j
  ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 6b k
  ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 6c l
  ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 6d m
  ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 6e n
  ,{0x38, 0x44, 0x44, 0x44, 0x38} // 6f o
  ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 70 p
  ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 71 q
  ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 72 r
  ,{0x48, 0x54, 0x54, 0x54, 0x20} // 73 s
  ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 74 t
  ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 75 u
  ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 76 v
  ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 77 w
  ,{0x44, 0x28, 0x10, 0x28, 0x44} // 78 x
  ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 79 y
  ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 7a z
  ,{0x00, 0x08, 0x36, 0x41, 0x00} // 7b {
  ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 7c |
  ,{0x00, 0x41, 0x36, 0x08, 0x00} // 7d }
  ,{0x10, 0x08, 0x08, 0x10, 0x08} // 7e ~
  ,{0x78, 0x46, 0x41, 0x46, 0x78} // 7f DEL
};

void draw_num(int number);

//There are two memory banks in the LCD, data/RAM and commands. This
//function sets the DC pin high or low depending, and then sends
//the data byte
void LCDWrite(byte data_or_command, byte data) {
  digitalWrite(PIN_DC, data_or_command); //Tell the LCD that we are writing either to data or a command

  //Send the data
  digitalWrite(PIN_SCE, LOW);
  shiftOut(PIN_SDIN, PIN_SCLK, MSBFIRST, data);
  digitalWrite(PIN_SCE, HIGH);
}

void gotoXY(int x, int y) {
  LCDWrite(0, 0x80 | x); // Column.
  LCDWrite(0, 0x40 | y); // Row. ?
}

//This takes a large array of bits and sends them to the LCD
void LCDBitmap(char my_array[]){
  for (int index = 0 ; index < (LCD_X * LCD_Y / 8) ; index++)
    LCDWrite(LCD_DATA, my_array[index]);
}
// 
// //This function takes in a character, looks it up in the font table/array
// //And writes it to the screen
// //Each character is 8 bits tall and 5 bits wide. We pad one blank column of
// //pixels on each side of the character for readability.
void LCDCharacter(char character) {
  LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding

  for (int index = 0 ; index < 5 ; index++)
    LCDWrite(LCD_DATA, ASCII[character - 0x20][index]);
    //0x20 is the ASCII character for Space (' '). The font table starts with this character

  LCDWrite(LCD_DATA, 0x00); //Blank vertical line padding
}

//Given a string of characters, one by one is passed to the LCD
void LCDString(char *characters) {
  while (*characters)
    LCDCharacter(*characters++);
}

//Clears the LCD by writing zeros to the entire screen
void LCDClear(void) {
  for (int index = 0 ; index < (LCD_X * LCD_Y / 8) ; index++)
    LCDWrite(LCD_DATA, 0x00);
    
  gotoXY(0, 0); //After we clear the display, return to the home position
}

//This sends the magical commands to the PCD8544
void LCDInit(void) {

  //Configure control pins
  pinMode(PIN_SCE, OUTPUT);
  pinMode(PIN_RESET, OUTPUT);
  pinMode(PIN_DC, OUTPUT);
  pinMode(PIN_SDIN, OUTPUT);
  pinMode(PIN_SCLK, OUTPUT);

  //Reset the LCD to a known state
  digitalWrite(PIN_RESET, LOW);
  digitalWrite(PIN_RESET, HIGH);

  LCDWrite(LCD_COMMAND, 0x21); //Tell LCD that extended commands follow
  LCDWrite(LCD_COMMAND, 0xB8); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
  LCDWrite(LCD_COMMAND, 0x04); //Set Temp coefficent
  LCDWrite(LCD_COMMAND, 0x14); //LCD bias mode 1:48: Try 0x13 or 0x14

  LCDWrite(LCD_COMMAND, 0x20); //We must send 0x20 before modifying the display control mode
  LCDWrite(LCD_COMMAND, 0x0C); //Set display control, normal mode. 0x0D for inverse
}



void InitializeTimer(){
    
   
    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseInitTypeDef timerInitStructure;
    timerInitStructure.TIM_Prescaler = 4000; 
    timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    timerInitStructure.TIM_Period = 7; //
    timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    timerInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &timerInitStructure);
    TIM_Cmd(TIM2, ENABLE);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
}

void EnableTimerInterrupt()
{
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}


int led2 = D7; // This one is the built-in tiny one to the right of the USB jack

int receive = D2 ; //GPIO port to receive IR input


void setup() {
  
    //GPIO ports
    pinMode(led2, OUTPUT);  
    pinMode(D2, INPUT);
    
    //Serial ports
    pinMode(RX, INPUT);
    pinMode(TX, OUTPUT);
    
    //Serial settings
    Serial1.begin(9600);
    
    LCDInit(); //Init the LCD
    LCDClear();
    
    //Timer interrupt settings
    InitializeTimer();
    EnableTimerInterrupt();
    
    //initialize led2 to off state
    digitalWrite(led2, LOW);
    
    button_sum = 0;
    array_count = 0;
    done = 0;
    hold=0;
    next =0;
    okaytoprint=0;
    

}

int printc;

unsigned char checksum;
unsigned char endlength;
int incomingByte=0;
int len=0;

void loop() {
    
     //int timerValue = TIM_GetCounter(TIM2);
   	//does nothing, interrupt does it for us
	
	
// 	while(!done){
// 	 
// 	 	  //poll unit done with calculation
// 	}
	
	if(done){
	    LCDClear();
	    if(okaytoprint)
	      letter_count++;
	    
	    else{
	      switch(button_sum){
		  case del:
			//LCDString("deleting!");
			if(letter_count>0)
			  letter_count-=1;
			else
			  letter_count=0;
			
			letter[letter_count] = ' ';
			break;
		  case VOLup:
		    
			LCDClear();
			LCDString("Sending text: ");
			//delay(100);
			//LCDClear();
			for(printc=0; printc<letter_count; printc++){
			  LCDCharacter(letter[printc]);
			}
			
			Serial1.write(0x7E); //delimiter
			Serial1.write(0x00); //length start
			
			endlength = (letter_count+5) & 0xFF; //num_char contains total number of character to send inside message.  you add 5 because we are taking into account total number of bytes-1 (API identifier, API Frame ID, MSB des, LSB des, Option
			//endlength is masked with 0xFF to get byte value
			
			Serial1.write(endlength); //length end
			Serial1.write(0x01); //API identifier
			Serial1.write(0x00); //API Frame ID
			Serial1.write(0x00); //Destination Address to xbee connected to sparkcore
			Serial1.write(0x00);
			Serial1.write(0x01); //Disable ACK, Option
			
			checksum = 0x02;  //initialize checksum (add API indentifier, API Frame ID, MSByte of Des. address, LSByte of Des. address, Option)

			for(printc=0; printc<letter_count; printc++){
				
				
				Serial1.write(letter[printc]); //transmit character of message to UART1
				
				checksum = checksum + letter[printc];  //this will add hex values. the end result will be truncated  to a 8 bit (byte) value automatically
				
			}
			
			checksum = 0xFF - checksum; //compute checksum
			Serial1.write(checksum); //send checksum through UART1 to XBee
		      
			
// 			delay(100);
// 			LCDString("Done sending");
// 			delay(100);
			letter_count=0; //when done sending packet
			break;
			
		  case CHup:
		    
		    LCDClear();
		    letter_count=0;
		    break;
		    
	      }
	    }
	    
	    
	    for(printc=0; printc<letter_count; printc++){
	      LCDCharacter(letter[printc]);
	    }
	
	    button_sum = 0;
	    array_count = 0;
	    done = 0;
	    hold=0;
	    next =0;
	    okaytoprint=0;
	}
	
	incomingByte = Serial1.peek();
	if(incomingByte != -1){
	  LCDClear();
	  LCDString("Incoming    Message:     ");
	  delay(1000);
	  //LCDClear();
	  len = Serial1.available();
	  for(printc=0; printc<8; printc++){
	    incomingByte = Serial1.read();
	    
	  }
	  for(printc=0; printc<(len-9); printc++){
	    incomingByte = Serial1.read();
	    LCDCharacter(incomingByte);
	  }
	  incomingByte=Serial1.read();
	  
// 	  delay(2000);
// 	  if(letter_count != 0){
// 	    LCDClear();
// 	    LCDString("Last edited message:    ");
// 	    //delay(1000);
// 	    //LCDClear();
// 	    for(printc=0; printc<letter_count; printc++){
// 	      LCDCharacter(letter[printc]);
// 	    }
// 	    
// 	  }
	}

}



extern "C" void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

	pc5 = digitalRead(D2);

	
	  if (GENERATE && pc5){
		  assert_count++;
		  if (assert_count > 100){ //
			  GENERATE=0;
			  done=1;
		  }
	  }			
	  
	  if( (GENERATE) && (array_count < total_bits) && (assert_count>0) && (!pc5) ){
		  if(assert_count < 4){
			  buffer[array_count]=0;
			  array_count++;
		  }
		  
		  else if(assert_count >= 4){
			  button_sum = button_sum + (1 << array_count);
			  buffer[array_count] = 1;
			  array_count++;
		  }
			  
	  }
		  
	  if (array_count >= total_bits){
		  hold++;
		  
		  if ( !(hold%1500) ){ //1500
			 okaytoprint = 1;
			 LCDClear();
			 for(printc=0; printc<letter_count; printc++){
			    LCDCharacter(letter[printc]);
			 }
			 draw_num(button_sum);
			 
			 
			  
			  
			  next++;
			 // LCDCharacter(next + '0');
			  
		  }
	  }
	  if (!pc5){
		  GENERATE=1;
		  assert_count=0;
	  }

	
// 	/*if(val && start)
// 		count++;
// 	*/
// 	count++;
// 	/*else if(!val && !first && start){
// 		first=1;
// 		if(count > 5)
// 			digitalWrite(led2, HIGH);	
// 	}
// 	*/
// 	
// 	if(count==2571){
// 
// 		if(start){
// 			digitalWrite(led2, HIGH);
// 			start = 0;
// 		}
// 
// 		else{
// 			digitalWrite(led2, LOW);
// 			start = 1;
// 		}
// 		count = 0;
// 	}    
    }
}

void draw_num(int number){
	switch(number){
		case zero: 
		      LCDString("SPACE");
		      letter[letter_count] = ' ';
		      
		      break;
			    
		case one:
		      LCDCharacter('1');
		      letter[letter_count] = '1';
		      
		      break;
			      
	    
		case two: 
			if ((next%4) ==0){
				 LCDCharacter('A');
			}
			else if ((next%4) == 1)
				 LCDCharacter('B');
			else if ((next%4) == 2)
				 LCDCharacter('C');
			else if ((next%4) == 3)
				 LCDCharacter('2');
			if ((next%4) ==0){
				 letter[letter_count] ='A';
			}
			else if ((next%4) == 1)
				 letter[letter_count] ='B';
			else if ((next%4) == 2)
				 letter[letter_count] ='C';
			else if ((next%4) == 3)
				 letter[letter_count] ='2';
			
			break;
		case three: 
			if ((next%4) ==0)
				 LCDCharacter('D');
			else if ((next%4) == 1)
				 LCDCharacter('E');
			else if ((next%4) == 2)
				 LCDCharacter('F');
			else if ((next%4) == 3)
				 LCDCharacter('3');
			if ((next%4) ==0)
				 letter[letter_count] ='D';
			else if ((next%4) == 1)
				 letter[letter_count] ='E';
			else if ((next%4) == 2)
				 letter[letter_count] ='F';
			else if ((next%4) == 3)
				 letter[letter_count] ='3';
			break;
		case four: 
			if ((next%4) ==0)
				 LCDCharacter('G');
			else if ((next%4) == 1)
				 LCDCharacter('H');
			else if ((next%4) == 2)
				 LCDCharacter('I');
			else if ((next%4) == 3)
				 LCDCharacter('4');
			if ((next%4) ==0)
				 letter[letter_count] ='G';
			else if ((next%4) == 1)
				 letter[letter_count] ='H';
			else if ((next%4) == 2)
				 letter[letter_count] ='I';
			else if ((next%4) == 3)
				 letter[letter_count] ='4';
			break;
		case five: 
			if ((next%4) ==0)
				 LCDCharacter('J');
			else if ((next%4) == 1)
				 LCDCharacter('K');
			else if ((next%4) == 2)
				 LCDCharacter('L');
			else if ((next%4) == 3)
				 LCDCharacter('5');
			if ((next%4) ==0)
				 letter[letter_count] ='J';
			else if ((next%4) == 1)
				 letter[letter_count] ='K';
			else if ((next%4) == 2)
				 letter[letter_count] ='L';
			else if ((next%4) == 3)
				 letter[letter_count] ='5';
			break;
		case six: 
			if ((next%4) ==0)
				 LCDCharacter('M');
			else if ((next%4) == 1)
				 LCDCharacter('N');
			else if ((next%4) == 2)
				 LCDCharacter('O');
			else if ((next%4) == 3)
				 LCDCharacter('6');
			if ((next%4) ==0)
				 letter[letter_count] ='M';
			else if ((next%4) == 1)
				 letter[letter_count] ='N';
			else if ((next%4) == 2)
				 letter[letter_count] ='O';
			else if ((next%4) == 3)
				 letter[letter_count] ='6';
			break;
		case seven: 
			if ((next%5) ==0)
				 LCDCharacter('P');
			else if ((next%5) == 1)
				 LCDCharacter('Q');
			else if ((next%5) == 2)
				 LCDCharacter('R');
			else if ((next%5) == 3)
				 LCDCharacter('S');
			else if ((next%5) == 4)
				 LCDCharacter('7');
			if ((next%5) ==0)
				 letter[letter_count] ='P';
			else if ((next%5) == 1)
				 letter[letter_count] ='Q';
			else if ((next%5) == 2)
				 letter[letter_count] ='R';
			else if ((next%5) == 3)
				 letter[letter_count] ='S';
			else if ((next%5) == 4)
				 letter[letter_count] ='7';
			break;
		case eight: 
			if ((next%4) ==0)
				 LCDCharacter('T');
			else if ((next%4) == 1)
				 LCDCharacter('U');
			else if ((next%4) == 2)
				 LCDCharacter('V');
			else if ((next%4) == 3)
				 LCDCharacter('8');
			if ((next%4) ==0)
				 letter[letter_count] ='T';
			else if ((next%4) == 1)
				 letter[letter_count] ='U';
			else if ((next%4) == 2)
				 letter[letter_count] ='V';
			else if ((next%4) == 3)
				 letter[letter_count] ='8';
			break;
		case nine: 
			if ((next%5) ==0)
				 LCDCharacter('W');
			else if ((next%5) == 1)
				 LCDCharacter('X');
			else if ((next%5) == 2)
				 LCDCharacter('Y');
			else if ((next%5) == 3)
				 LCDCharacter('Z');
			else if ((next%5) == 4)
				 LCDCharacter('9');
			if ((next%5) ==0)
				 letter[letter_count] ='W';
			else if ((next%5) == 1)
				 letter[letter_count] ='X';
			else if ((next%5) == 2)
				 letter[letter_count] ='Y';
			else if ((next%5) == 3)
				 letter[letter_count] ='Z';
			else if ((next%5) == 4)
				 letter[letter_count] ='9';
			break;
//		default:
			
//			 LCDCharacter('ERROR');
		
	}

}

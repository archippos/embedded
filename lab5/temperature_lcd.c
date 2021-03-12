//team "keith help"
//eli lawrence, erin parker, carol pollard, kane simmons, drew stork
//temperature_lcd.c - flashes a heartbeat LED 1s with a 50% duty cycle
//              displays potentiometer value in 16b hex when SW1_PRESSED
//              SW2_PRESSED samples potentiometer voltage every 1s until SW2 or SW1 pressed
//              LCD screen displays a graphical representation of the potentio and temp value
// https://www.8051projects.net/lcd-interfacing/lcd-custom-character.php   some help with custom chars

//includes
#include "revF14.h"
#include "esos.h"
#include "esos_pic24.h"
#include "esos_f14ui.h"
#include "esos_sensor.h"
#include "esos_pic24_sensor.h"
#include "esos_lcd44780.h"
#include "esos_pic24_lcd44780.h"

//defines
#define REFRESH_RATE 200
#define FIRSTLINE 0b00010000

//static variable definitions
static uint8_t u8_state;
static uint8_t u8_proccessConst = ESOS_SENSOR_ONE_SHOT;
static uint16_t pu16_hexOut;
static char tempStrUpper[16];
static char tempStrLower[16];
static char PotStr[8];
static BOOL b_dispState;
static uint16_t u16_timeout;
static char potStr[3];
static uint8_t au8_slider;
static uint8_t au8_bar;
static uint8_t au8_bar2;
static uint8_t pu8_hexOut;


static uint8_t slider0[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000} ; //don't use row 8 because that is where the cursor lives
static uint8_t slider1[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b00000} ;
static uint8_t slider2[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b00000} ;
static uint8_t slider3[8] = { 0b00000, 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b00000} ;
static uint8_t slider4[8] = { 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000} ;
static uint8_t slider5[8] = { 0b00000, 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000} ;
static uint8_t slider6[8] = { 0b00000, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000} ;
static uint8_t slider7[8] = { 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b00000} ;
enum straight_bars{
	Blank,
	OneEight,
	TwoEight,
	ThreeEight,
	FourEight,
	FiveEight,
	SixEight,
	SevenEight,
};


static uint8_t sliderOnTheLeft[8] = { 0b00000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b00000, 0b00000} ;
static uint8_t sliderSecLeft[8] = { 0b00000, 0b01000, 0b01000, 0b01000, 0b01000, 0b01000, 0b00000, 0b00000} ;
static uint8_t sliderMiddle[8] = { 0b00000, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00000, 0b00000} ;
static uint8_t sliderSecRight[8] = { 0b00000, 0x00010, 0x00010, 0x00010, 0x00010, 0x00010, 0b00000, 0b00000} ;
static uint8_t sliderOnTheRight[8] = {0b00000, 0b00001, 0b00001, 0b00001, 0b00001, 0b00001, 0b00000, 0b00000} ;
enum slide_bby_slide{
	Left,
	SecLeft,
	Middle,
	SecRight,
	Right,
};

//USER TASK SET DISPLAY STATE (LCD)
ESOS_USER_TASK(setDispState)
{
	ESOS_TASK_BEGIN();
	b_dispState = 0;
	while (TRUE) {
		ESOS_TASK_WAIT_UNTIL(SW3_PRESSED);
		b_dispState = !b_dispState;
		esos_lcd44780_clearScreen();
		u16_timeout = REFRESH_RATE;
		ESOS_TASK_WAIT_UNTIL(SW3_RELEASED);

		ESOS_TASK_YIELD();
		}
	ESOS_TASK_END();
}

// heartbeat on LED 3
ESOS_USER_TASK(heartbeat)
{
	ESOS_TASK_BEGIN();
	while(TRUE){
		esos_uiF14_toggleLED3();
		ESOS_TASK_WAIT_TICKS( 500 );	//500ms period
	}
	ESOS_TASK_END();
}

//LCD FUNCTIONALITY + thermo and pot reading
ESOS_USER_TASK(info)
{
  static ESOS_TASK_HANDLE getADC; //this is the handle for the child we're gonna birth
	static int i;
  ESOS_TASK_BEGIN();
  while(TRUE)
  {
	    if (u16_timeout == REFRESH_RATE) {
			u16_timeout = 0;
	    if (!b_dispState) {		//if no, we do potentiometer stuff

	      //make all the custom chars
	      esos_lcd44780_setCustomChar(Left, sliderOnTheLeft);
	      esos_lcd44780_setCustomChar(SecLeft, sliderSecLeft);
	      esos_lcd44780_setCustomChar(Middle, sliderMiddle);
	      esos_lcd44780_setCustomChar(SecRight, sliderSecRight);
	      esos_lcd44780_setCustomChar(Right, sliderOnTheRight);
		
		

	      //get the pot value
	      ESOS_ALLOCATE_CHILD_TASK(getADC);
	      ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_ON_AVAILABLE_SENSOR, POT_CHANNEL, ESOS_SENSOR_VREF_3V3);
	      //ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_QUICK_READ, &pu16_hexOut);
	      ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_QUICK_READ, &pu16_hexOut);
	      ESOS_SENSOR_CLOSE();                      //read once, close the sensor channel

				//Still communicate with BullyCPP
				ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
				ESOS_TASK_WAIT_ON_SEND_STRING("\nPotentiometer: ");
				ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pu16_hexOut);	//send out pot output to console
				ESOS_TASK_WAIT_ON_SEND_UINT8('\n');			 //make purty
				ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();			//doneso


				pu16_hexOut = pu16_hexOut >> 4; //ignore the LSB since we are getting 12 bit output from the pot for some reason
				pu8_hexOut = pu16_hexOut & 0x00FF; //change to a uint8 because the max is two slots for the LCD for the output
				convert_pot8_to_str(pu8_hexOut, PotStr);	//need to be chars so that we can tell the LCD to send them
                                
				/* This block was used during testing
                                ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
                                ESOS_TASK_WAIT_ON_SEND_UINT8(PotStr[1]);	//send out pot output to console
				ESOS_TASK_WAIT_ON_SEND_UINT8(PotStr[0]);	//send out pot output to console
				ESOS_TASK_WAIT_ON_SEND_UINT8('\n');
				ESOS_TASK_WAIT_ON_SEND_STRING("\nHEX: ");
				ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pu8_hexOut);
				ESOS_TASK_WAIT_ON_SEND_UINT8('\n');	 	 //make purty
				ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();	
				*/

				
				// GIVE ME A P! GIVE ME AN O! GIVE ME A T! WHAT DOES THAT SPELL?!
				esos_lcd44780_writeChar(0,0,'p');
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
                                esos_lcd44780_writeChar(0,1,'o');
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
			        esos_lcd44780_writeChar(0,2,'t');
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
                                esos_lcd44780_writeChar(0,3,' ');
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				esos_lcd44780_writeChar(0,4,'0');
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				esos_lcd44780_writeChar(0,5,'x');
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				

				esos_lcd44780_writeChar(0,6,PotStr[1]);		//send the chars of the pot value created earlier in the convert pot8 to string function
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
                                esos_lcd44780_writeChar(0,7,PotStr[0]);
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();


				if((pu8_hexOut >= 0x00) && (pu8_hexOut < 0x0C)){		//lots of conditions on where the slider goes
					au8_slider = Left;
				        esos_lcd44780_writeChar(1,0,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0x0C) && (pu8_hexOut < 0x18)){
					au8_slider = Middle;
				        esos_lcd44780_writeChar(1,0,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0x18) && (pu8_hexOut < 0x24)){
					au8_slider = Right;
				        esos_lcd44780_writeChar(1,0,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0x24) && (pu8_hexOut < 0x30)){
					au8_slider = SecLeft;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0x30) && (pu8_hexOut < 0x60)){
					au8_slider = SecRight;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0x60) && (pu8_hexOut < 0x6C)){
					au8_slider = Left;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0x6C) && (pu8_hexOut < 0x78)){
					au8_slider = Middle;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0x78) && (pu8_hexOut < 0x84)){
					au8_slider = Right;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0x84) && (pu8_hexOut < 0x85)){
					au8_slider = SecLeft;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0x85) && (pu8_hexOut < 0x87)){
					au8_slider = SecRight;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0x87) && (pu8_hexOut < 0x93)){
					au8_slider = SecLeft;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0x93) && (pu8_hexOut < 0x9F)){
					au8_slider = SecRight;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0x9F) && (pu8_hexOut < 0xAB)){
					au8_slider = Left;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0xAB) && (pu8_hexOut < 0xB7)){
					au8_slider = Middle;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0xB7) && (pu8_hexOut < 0xC3)){
					au8_slider = Right;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0xC3) && (pu8_hexOut < 0xCF)){
					au8_slider = SecLeft;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0xCF) && (pu8_hexOut < 0xDB)){
					au8_slider = SecRight;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0xDB) && (pu8_hexOut < 0xE7)){
					au8_slider = Left;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu8_hexOut >= 0xE7) && (pu8_hexOut < 0xF3)){
					au8_slider = Middle;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else {
					au8_slider = Right;
					esos_lcd44780_writeChar(1,0,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				        esos_lcd44780_writeChar(1,1,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,2,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,3,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,4,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,5,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,6,0b01011111);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_slider);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}

				
                                //ESOS_TASK_WAIT_TICKS(5);		having this kind of helps the lcd not flash and move but not really
                                
		


	    } else {		//else, temperature
	      ESOS_ALLOCATE_CHILD_TASK(getADC);

				//make all the custom chars
	      			esos_lcd44780_setCustomChar(Blank, slider0);
	      			esos_lcd44780_setCustomChar(OneEight, slider1);
	      			esos_lcd44780_setCustomChar(TwoEight, slider2);
	      			esos_lcd44780_setCustomChar(ThreeEight, slider3);
	      			esos_lcd44780_setCustomChar(FourEight, slider4);	
				esos_lcd44780_setCustomChar(FiveEight, slider5);
				esos_lcd44780_setCustomChar(SixEight, slider6);		
				esos_lcd44780_setCustomChar(SevenEight, slider7);	

				//get the temp
				ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_ON_AVAILABLE_SENSOR, TEMP_CHANNEL, ESOS_SENSOR_VREF_3V3);
				ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_READ, &pu16_hexOut, u8_proccessConst, ESOS_SENSOR_FORMAT_VOLTAGE);
				ESOS_SENSOR_CLOSE();

				

				//temp convert to str
				convert_temp_to_str(pu16_hexOut, tempStrUpper, tempStrLower);

				//convert_temp_to_str(0x444, tempStrUpper, tempStrLower); used for testing

				//output to BullyCPP
				ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();   //now we wait to send our data
				ESOS_TASK_WAIT_ON_SEND_STRING("\nTemp hex: ");
				ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pu16_hexOut);
	 		        ESOS_TASK_WAIT_ON_SEND_STRING("\nTemp is ");
	  	 	        ESOS_TASK_WAIT_ON_SEND_STRING(tempStrUpper);
			        ESOS_TASK_WAIT_ON_SEND_STRING(".");
		       	        ESOS_TASK_WAIT_ON_SEND_STRING(tempStrLower);
	      			ESOS_TASK_WAIT_ON_SEND_STRING(" degrees Celsius\n");
	      			ESOS_TASK_WAIT_ON_SEND_UINT8('\n');      //slap in a newline to made it purty
	      			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();   //all done!


				
				//output stuff to the LCD that won't change
				esos_lcd44780_writeChar(0, 0, 'L');
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				esos_lcd44780_writeChar(0, 1, 'M');
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				esos_lcd44780_writeChar(0, 2, '6');
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				esos_lcd44780_writeChar(0, 3, '0');
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
        			esos_lcd44780_writeString(1, 0, tempStrUpper);
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
        			esos_lcd44780_writeChar(1, 3, 'C');
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				
				esos_lcd44780_writeString(1, 0, tempStrUpper);
				ESOS_TASK_WAIT_ON_LCD44780_REFRESH();

				if((pu16_hexOut >= 0x00) && (pu16_hexOut < 0x222)){
					au8_bar = OneEight;
					au8_bar2 = Blank;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu16_hexOut >= 0x222) && (pu16_hexOut < 0x333)){
					au8_bar = TwoEight;
					au8_bar2 = Blank;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					esos_lcd44780_writeChar(1,7,au8_bar);
				}
				else if((pu16_hexOut >= 0x333) && (pu16_hexOut < 0x444)){
					au8_bar = ThreeEight;
					au8_bar2 = Blank;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu16_hexOut >= 0x444) && (pu16_hexOut < 0x555)){
					au8_bar = FourEight;
					au8_bar2 = Blank;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu16_hexOut >= 0x555) && (pu16_hexOut < 0x666)){
					au8_bar = FiveEight;
					au8_bar2 = Blank;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu16_hexOut >= 0x666) && (pu16_hexOut < 0x777)){
					au8_bar = SixEight;
					au8_bar2 = Blank;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu16_hexOut >= 0x777) && (pu16_hexOut < 0x888)){
					au8_bar = SevenEight;
					au8_bar2 = Blank;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu16_hexOut >= 0x888) && (pu16_hexOut < 0x999)){
					au8_bar = SevenEight;
					au8_bar2 = OneEight;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu16_hexOut >= 0x999) && (pu16_hexOut < 0xAAA)){
					au8_bar = SevenEight;
					au8_bar2 = TwoEight;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu16_hexOut >= 0xAAA) && (pu16_hexOut < 0xBBB)){
					au8_bar = SevenEight;
					au8_bar2 = ThreeEight;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu16_hexOut >= 0xBBB) && (pu16_hexOut < 0xCCC)){
					au8_bar = SevenEight;
					au8_bar2 = FourEight;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu16_hexOut >= 0xCCC) && (pu16_hexOut < 0xDDD)){
					au8_bar = SevenEight;
					au8_bar2 = FourEight;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu16_hexOut >= 0xDDD) && (pu16_hexOut < 0xEEE)){
					au8_bar = SevenEight;
					au8_bar2 = FiveEight;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else if((pu16_hexOut >= 0xEEE) && (pu16_hexOut < 0xFFF)){
					au8_bar = SevenEight;
					au8_bar2 = SixEight;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}
				else {
					au8_bar = SevenEight;
					au8_bar2 = SevenEight;
					esos_lcd44780_writeChar(0,7,au8_bar2);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
					esos_lcd44780_writeChar(1,7,au8_bar);
					ESOS_TASK_WAIT_ON_LCD44780_REFRESH();
				}

				//ESOS_TASK_WAIT_TICKS(5); this kind of helps with the jumping but not really
		}

		} else {
			u16_timeout += 10;
			ESOS_TASK_WAIT_TICKS(10);
			continue;		//update our timeout and proceed
		}
    ESOS_TASK_YIELD();
  	}
  ESOS_TASK_END();
}

void user_init()
{
		//basic initializers
    config_esos_uiF14();
    esos_lcd44780_init();
    esos_lcd44780_configDisplay();
    esos_lcd44780_setCursorBlink( 0 );
    
    

		//now we register our tasks
    esos_RegisterTask(heartbeat);
    esos_RegisterTask(setDispState);
    esos_RegisterTask(info);
}


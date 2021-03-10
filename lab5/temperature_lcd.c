//team "keith help"
//eli lawrence, erin parker, carol pollard, kane simmons, drew stork
//temperature_lcd.c - flashes a heartbeat LED 1s with a 50% duty cycle
//              displays potentiometer value in 16b hex when SW1_PRESSED
//              SW2_PRESSED samples potentiometer voltage every 1s until SW2 or SW1 pressed
//              LCD screen displays a graphical representation of the potentio and temp value

//includes
#include "revF14.h"
#include "esos.h"
#include "esos_pic24.h"
#include "esos_f14ui.h"
#include "esos_sensor.h"
#include "esos_pic24_sensor.h"
#include "esos_lcd44780.h"
#include "esos_pic24_lcd44780.h"
#include "esos_lcd44780_customChars.h"

//defines
#define REFRESH_RATE 200

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
static uint8_t au8_slider[8];
static uint8_t pu8_hexOut;

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
				//custom chars Stuff
				esos_lcd44780_init_custom_chars_slider();
				//do the adc
              //esos_lcd44780_setCursorHome();
	      ESOS_ALLOCATE_CHILD_TASK(getADC);
	      ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_ON_AVAILABLE_SENSOR, POT_CHANNEL, ESOS_SENSOR_VREF_3V3);
	      //ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_QUICK_READ, &pu16_hexOut);
	      ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_QUICK_READ, &pu16_hexOut);
	      ESOS_SENSOR_CLOSE();                      //read once, close the sensor channel

				//Still communicate with BullyCPP
				ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
				ESOS_TASK_WAIT_ON_SEND_STRING("\nPotentiometer: ");
				ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pu16_hexOut);	//send out pot output to console
				ESOS_TASK_WAIT_ON_SEND_UINT8('\n');										 	 //make purty
				ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();									 //doneso








                                //esos_lcd44780_writeString(0, 0, "pot 0x");				//write to our lcd in the first 6 slots

				for (i=0; i<8; i++) {
					au8_slider[i] = SLIDER_LINE;		//create our slidey boi
				}





				pu8_hexOut = pu16_hexOut & 0x00FF; //change to a uint8 because the max is 0xFF for the output
				convert_pot8_to_str(pu8_hexOut, PotStr);

                                ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
                                ESOS_TASK_WAIT_ON_SEND_UINT8(PotStr[1]);	//send out pot output to console
				ESOS_TASK_WAIT_ON_SEND_UINT8(PotStr[2]);	//send out pot output to console
				ESOS_TASK_WAIT_ON_SEND_UINT8('\n');
				ESOS_TASK_WAIT_ON_SEND_STRING("\nHEX: ");
				ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pu8_hexOut);
                               // ESOS_TASK_WAIT_ON_SEND_UINT8(PotStr[2]);	//send out pot output to console
				ESOS_TASK_WAIT_ON_SEND_UINT8('\n');	 	 //make purty
				ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

				esos_lcd44780_setCursor(0,0);

				esos_lcd44780_writeChar(0,0,'p');
                                esos_lcd44780_writeChar(0,1,'o');
			        esos_lcd44780_writeChar(0,2,'t');
                                esos_lcd44780_writeChar(0,3,' ');
				esos_lcd44780_writeChar(0,4,'0');
				esos_lcd44780_writeChar(0,5,'x');

				esos_lcd44780_writeChar(0,6,PotStr[1]);
                                esos_lcd44780_writeChar(0,7,PotStr[2]);
				esos_lcd44780_setCursor(0,0);


                                ESOS_TASK_WAIT_TICKS(750);
                                /*
				i = pu16_hexOut >> 9;			//need to determine which cell gets our indicator
				au8_slider[i] = ((pu16_hexOut & 0x1FF) / 0x067) + 1;		//scale fifths
				convert_uint32_t_to_str(pu16_hexOut >> 4, potStr, 3, 16);
                                esos_lcd44780_writeString(0, 7, PotStr);

				//the display was being really weird :(((((
				if (potStr[1] == 0) {
           				 potStr[2] = potStr[1];
           				 potStr[1] = potStr[0];
           				 potStr[0] = '0';
      				}

				esos_lcd44780_writeString(0, 6, potStr);
				esos_lcd44780_writeBuffer(1, 0, au8_slider, 8);
				*/



	    } else {		//else, temperature
	      ESOS_ALLOCATE_CHILD_TASK(getADC);
				ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_ON_AVAILABLE_SENSOR, TEMP_CHANNEL, ESOS_SENSOR_VREF_3V3);
				ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_READ, &pu16_hexOut, u8_proccessConst, ESOS_SENSOR_FORMAT_VOLTAGE);
				ESOS_SENSOR_CLOSE();

				esos_lcd44780_init_custom_chars_bar();		//we got BARS baby

				//code to format our temperature stuff goes here
				convert_temp_to_str(pu16_hexOut, tempStrUpper, tempStrLower);
				ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();   //now we wait to send our data
	      ESOS_TASK_WAIT_ON_SEND_STRING("Temp is ");
	      ESOS_TASK_WAIT_ON_SEND_STRING(tempStrUpper);
	      ESOS_TASK_WAIT_ON_SEND_STRING(".");
	      ESOS_TASK_WAIT_ON_SEND_STRING(tempStrLower);
	      ESOS_TASK_WAIT_ON_SEND_STRING(" degrees Celsius\n");
	      ESOS_TASK_WAIT_ON_SEND_UINT8('\n');      //slap in a newline to made it purty
	      ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();   //all done!

				//above is all console output; we need stuff for lcd output
        esos_lcd44780_writeString(1, 0, tempStrUpper);
        esos_lcd44780_writeChar(1, 2, ' ');
        esos_lcd44780_writeChar(1, 3, 'C');

				//we need to determine how many bars to put; we can put it after number outputs
				uint8_t u8_barTop = ' ';
        uint8_t u8_barBot = ' ';	//keep these empty to start with

				if (pu16_hexOut < 200) {
            //no bars if it's below this threshold
        } else if (pu16_hexOut >= 200 && pu16_hexOut < 210) {
            u8_barBot = _1EIGTH_BAR;
        } else if (pu16_hexOut >= 210 && pu16_hexOut < 220) {
            u8_barBot = _2EIGTH_BAR;
        } else if (pu16_hexOut >= 220 && pu16_hexOut < 230) {
            u8_barBot = _3EIGTH_BAR;
        } else if (pu16_hexOut >= 230 && pu16_hexOut < 240) {
            u8_barBot = _4EIGTH_BAR;
        } else if (pu16_hexOut >= 240 && pu16_hexOut < 250) {
            u8_barBot = _5EIGTH_BAR;
        } else if (pu16_hexOut >= 250 && pu16_hexOut < 260) {
            u8_barBot = _6EIGTH_BAR;
        } else if (pu16_hexOut >= 260 && pu16_hexOut < 270) {
            u8_barBot = _7EIGTH_BAR;
        } else if (pu16_hexOut >= 270 && pu16_hexOut < 280) {
            u8_barBot = _8EIGTH_BAR;
        } else if (pu16_hexOut >= 280 && pu16_hexOut < 290) {
            u8_barBot = _8EIGTH_BAR;
            u8_barTop = _1EIGTH_BAR;
        } else if (pu16_hexOut >= 290 && pu16_hexOut < 300) {
            u8_barBot = _8EIGTH_BAR;
            u8_barTop = _2EIGTH_BAR;
        } else if (pu16_hexOut >= 300 && pu16_hexOut < 310) {
            u8_barBot = _8EIGTH_BAR;
            u8_barTop = _3EIGTH_BAR;
        } else if (pu16_hexOut >= 310 && pu16_hexOut < 320) {
            u8_barBot = _8EIGTH_BAR;
            u8_barTop = _4EIGTH_BAR;
        } else if (pu16_hexOut >= 320 && pu16_hexOut < 330) {
            u8_barBot = _8EIGTH_BAR;
            u8_barTop = _5EIGTH_BAR;
        } else if (pu16_hexOut >= 330 && pu16_hexOut < 340) {
            u8_barBot = _8EIGTH_BAR;
            u8_barTop = _6EIGTH_BAR;
        } else if (pu16_hexOut >= 340 && pu16_hexOut < 350) {
            u8_barBot = _8EIGTH_BAR;
            u8_barTop = _7EIGTH_BAR;
        } else {
					//when above this threshold, all the bars
            u8_barBot = _8EIGTH_BAR;
            u8_barTop = _8EIGTH_BAR;
        }
				//tidying up
				esos_lcd44780_writeChar(0, 7, u8_barTop);
        esos_lcd44780_writeChar(1, 7, u8_barBot);	//done
			}

			//finish up our console output
			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			//ESOS_TASK_WAIT_ON_SEND_STRING("Thank You.\n");	//politeness is key
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

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

		//now we register our tasks
    esos_RegisterTask(heartbeat);
    esos_RegisterTask(setDispState);
    esos_RegisterTask(info);
}

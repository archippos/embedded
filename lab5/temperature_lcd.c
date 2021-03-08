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

#define REFRESH_RATE 200

static uint8_t u8_state;
static uint8_t u8_pmode = '1';
static uint8_t u8_samples_input = '0';
static uint8_t u8_proccessConst = ESOS_SENSOR_ONE_SHOT;
static uint16_t pu16_hexOut;
static char tempStrUpper[16];
static char tempStrLower[16];
static bool b_dispState;
static uint16_t u16_timeout;
static char potStr[3];
static uint8_t au8_slider[8];

//TODO: USER TASK SET DISPLAY STATE (LCD)
ESOS_USER_TASK(setDispState)
{
	ESOS_TASK_BEGIN();
	b_dispState = 0;
	while (TRUE) {
		ESOS_TASK_WAIT_UNTIL_UF14_SW3_PRESSED();
		b_dispState = !b_dispState;
		esos_lcd44780_clearScreen();
		u16_timeout = REFRESH_RATE;
		ESOS_TASK_WAIT_UNTIL_UIF14_SW3_RELEASED();

		ESOS_TASK_YIELD();
		}
	ESOS_TASK_END();
}

// heartbeat on LED 3
//TODO: verify this meets the requirements of lab 5
ESOS_USER_TASK(heartbeat)
{
	ESOS_TASK_BEGIN();
	while(TRUE){
		esos_uiF14_toggleLED3();
		ESOS_TASK_WAIT_TICKS( 250 );
	}
	ESOS_TASK_END();
}

//TODO: LCD FUNCTIONALITY
ESOS_USER_TASK(info)
{
  static ESOS_TASK_HANDLE getADC; //this is the handle for the child we're gonna birth
	static int i;
  ESOS_TASK_BEGIN();
  while(TRUE)
  {
		if (u16_timeout == REFRESH_RATE) {
			u16_timeout = 0;
	    if (u8_state) {		//if yes, we do potentiometer stuff
				//custom chars Stuff
				esos_lcd44780_init_custom_chars_slider();
				//do the adc
	      ESOS_ALLOCATE_CHILD_TASK(getADC);
	      ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_ON_AVAILABLE_SENSOR, POT_CHANNEL, ESOS_SENSOR_VREF_3V3);
	      //ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_QUICK_READ, &pu16_hexOut);
	      ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_READ, &pu16_hexOut, u8_proccessConst, ESOS_SENSOR_FORMAT_VOLTAGE);
	      ESOS_SENSOR_CLOSE();                      //read once, close the sensor channel

				ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
				ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pu16_hexOut);	//send out pot output to console
				ESOS_TASK_WAIT_ON_SEND_UINT8('\n');											//make purty
				ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();									//doneso

				esos_lcd44780_writeString(0, 0, "pot 0x");				//write to our lcd

				for (i=0; i<0; i++) {
					au8_slider[i] = SLIDER_LINE;		//create our slidey boi
				}

				i = pu16_hexOut >> 9;			//need to determine which cell gets our indicator
				au8_slider[i] = ((pu16_hexOut & 0x1FF) / 0x067) + 1		//scale fifths
				convert_uint32_t_to_str(pu16_hexOut >> 4, potStr, 3, 16);
				esos_lcd44780_writeString(0, 6, potStr);
				esos_lcd44780_writeBuffer(1, 0, au8_slider, 8);

	    } else {		//else, temperature
	      //if u8_state case 2: output every 1s until "state" flag unset
	      ESOS_ALLOCATE_CHILD_TASK(getADC);

	      do {
	        //if u8_state case 1: output once before goto state=0
	        ESOS_ALLOCATE_CHILD_TASK(getADC);
	        ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_ON_AVAILABLE_SENSOR, TEMP_CHANNEL, ESOS_SENSOR_VREF_3V3);

	        //ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_QUICK_READ, &pu16_hexOut);
	        ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_READ, &pu16_hexOut, u8_proccessConst, ESOS_SENSOR_FORMAT_VOLTAGE);
	        ESOS_SENSOR_CLOSE();                      //read once, close the sensor channel

	        convert_temp_to_str(pu16_hexOut, tempStrUpper, tempStrLower);

	        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();   //now we wait to send our data
	        ESOS_TASK_WAIT_ON_SEND_STRING("Temp is ");
	        ESOS_TASK_WAIT_ON_SEND_STRING(tempStrUpper);
	        ESOS_TASK_WAIT_ON_SEND_STRING(".");
	        ESOS_TASK_WAIT_ON_SEND_STRING(tempStrLower);
	        ESOS_TASK_WAIT_ON_SEND_STRING(" degrees Celsius\n");
	        //ESOS_TASK_WAIT_ON_SEND_STRING("\nTemp: ");
	        // ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pu16_hexOut);
	        ESOS_TASK_WAIT_ON_SEND_UINT8('\n');      //slap in a newline to made it purty
	        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();   //all done!

	        ESOS_TASK_WAIT_TICKS(1000);             //1000ms = 1s
	      } while (u8_state == 2);		//dont think we need this
	     // ESOS_SENSOR_CLOSE();

			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_STRING("Thank You.\n");
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
			u8_state = 0;
		}
		} else {
			u16_timeout += 10;
			ESOS_TASK_WAIT_TICKS(10);
			continue;
		}
    ESOS_TASK_YIELD();
  	}
  ESOS_TASK_END();
}

void user_init()
{
    config_esos_uiF14();

    esos_RegisterTask(heartbeat);
    esos_RegisterTask(setDispState);
    esos_RegisterTask(potenInterface);
    esos_RegisterTask(info);
}

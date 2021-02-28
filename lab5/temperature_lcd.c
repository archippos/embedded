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
#include "esos_lcd44780_sensor.h"
#include "esos_pic24_lcd44780.h"
#include "esos_lcd44780_customChars.h"

static uint8_t u8_state;
static uint8_t u8_pmode = '1';
static uint8_t u8_samples_input = '0';
static uint8_t u8_proccessConst = ESOS_SENSOR_ONE_SHOT;
static uint16_t pu16_hexOut;
static char tempStrUpper[16];
static char tempStrLower[16];

//TODO: USER TASK SET DISPLAY STATE (LCD)

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
  ESOS_TASK_BEGIN();
  while(TRUE)
  {
    if(u8_state == 0) {
        //nothing needs to go here. we yield.
    } else if(u8_state == 1) {

      //Potintiometer stuff
      //if u8_state case 1: output once before goto state=0
      ESOS_ALLOCATE_CHILD_TASK(getADC);
      ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_ON_AVAILABLE_SENSOR, POT_CHANNEL, ESOS_SENSOR_VREF_3V3);
      //ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_QUICK_READ, &pu16_hexOut);
      ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_READ, &pu16_hexOut, u8_proccessConst, ESOS_SENSOR_FORMAT_VOLTAGE);
      ESOS_SENSOR_CLOSE();                      //read once, close the sensor channel

      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();   //now we wait to send our data
      ESOS_TASK_WAIT_ON_SEND_STRING("\nPotentiometer: ");
      ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pu16_hexOut);
      ESOS_TASK_WAIT_ON_SEND_UINT8('\n');      //slap in a newline to made it purty
      ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();   //all done!

      //Temp stuff
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

      u8_state = 0;                            //goto state 0 to yield
    } else if(u8_state == 2) {
      //if u8_state case 2: output every 1s until "state" flag unset
      ESOS_ALLOCATE_CHILD_TASK(getADC);

      do {
	//Pot Stuff
        ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_ON_AVAILABLE_SENSOR, POT_CHANNEL, ESOS_SENSOR_VREF_3V3);
        //ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_QUICK_READ, &pu16_hexOut);
	ESOS_TASK_WAIT_SENSOR_READ(pu16_hexOut, u8_proccessConst, ESOS_SENSOR_FORMAT_VOLTAGE);
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_STRING("\nPotentiometer: ");
        ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pu16_hexOut);
        ESOS_TASK_WAIT_ON_SEND_UINT8('\n');     //newline :)
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
	ESOS_SENSOR_CLOSE();

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
      } while (u8_state == 2);
     // ESOS_SENSOR_CLOSE();

		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("Thank You.\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		u8_state = 0;
	}
    ESOS_TASK_YIELD();
  }
  ESOS_TASK_END();
}

ESOS_USER_TASK(potenInterface)
{
  static BOOL b_switch1, b_switch2, b_switch3;
  ESOS_TASK_BEGIN();
  while(TRUE)
  {
    if(b_switch1 != esos_uiF14_isSW1Pressed()) {
      b_switch1 = esos_uiF14_isSW1Pressed();
      if(b_switch1) {
        //if we're in state 2, goto yield. else, output once, then yield
        if(u8_state == 2) u8_state = 0;
        else u8_state = 1;
      }
    }
    else if(b_switch2 != esos_uiF14_isSW2Pressed()) {
      b_switch2 = esos_uiF14_isSW2Pressed();
      if(b_switch2) {
        //if we're in state 2, goto yield. else, start continuous output
        if(u8_state == 2) u8_state = 0;
        else u8_state = 2;
      }
    }
	else if(b_switch3 != esos_uiF14_isSW3Pressed()) {
		b_switch3 = esos_uiF14_isSW3Pressed();
		if(b_switch3) {
			//enter state 3
			if(u8_state == 2) u8_state = 0;
			else u8_state = 3;
		}

	}
    ESOS_TASK_YIELD();
  }
  ESOS_TASK_END();
}

void user_init()
{
    config_esos_uiF14();

    esos_RegisterTask(heartbeat);
    esos_RegisterTask(potenInterface);
    esos_RegisterTask(info);
}

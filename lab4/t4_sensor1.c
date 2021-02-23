//team "keith help"
//eli lawrence, erin parker, carol pollard, kane simmons, drew stork
//t4_sensor.c - flashes a heartbeat LED every 250ms
//              displays potentiometer value in 16b hex when SW1_PRESSED
//              SW2_PRESSED samples potentiometer voltage every 1s until
//                          SW2 or SW1 pressed

//includes
#include "esos_f14ui.c"
#include "esos_sensor.h"
#include "esos_pic24_sensor.h"

static uint8_t u8_state;
static uint16_t pu16_hexOut

// heartbeat on LED 3
ESOS_USER_TASK(heartbeat)
{
	ESOS_TASK_BEGIN();
	while(TRUE){
		esos_uiF14_toggleLED3();
		ESOS_TASK_WAIT_TICKS( 250 );
	}
	ESOS_TASK_END();
}

//TODO: DISPLAY OUTPUT (REGISTER TASK)
ESOS_USER_TASK(info)
{
  //static uint8_t u8_state;        //this has to go outside if more than one task will use it
  static ESOS_TASK_HANDLE getADC; //this is the handle for the child we're gonna birth
  ESOS_TASK_BEGIN();
  while(TRUE)
  {
    if(u8_state == 0) {
        //nothing needs to go here. we yield.
    } else if(u8_state == 1) {
      //if u8_state case 1: output once before goto state=0
      ESOS_ALLOCATE_CHILD_TASK(getADC);
      //TODO: is VREF_3V0 ok? should it be 3V3? or lower? will need to check datasheets
      ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_ON_AVAILABLE_SENSOR, POT_CHANNEL, ESOS_SENSOR_VREF_3V0);
      ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_QUICK_READ, &pu16_hexOut);
      ESOS_SENSOR_CLOSE();                      //read once, close the sensor channel
      ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();   //now we wait to send our data
      //[logic to send out output as a hex string goes here] //This logic was used in lab 3
			ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pu16_hexOut);
      ESOS_TASK_WAIT_ON_SEND_UINT8('\n');      //slap in a newline to made it purty
      ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();   //all done!
      u8_state = 0;                            //goto state 0 to yield
    } else if(u8_state == 2) {
      //if u8_state case 2: output every 1s until "state" flag unset
      ESOS_ALLOCATE_CHILD_TASK(getADC);
      ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_ON_AVAILABLE_SENSOR, POT_CHANNEL, ESOS_SENSOR_VREF_3V0);
      do {    //use a do-while(state==2)???
        ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_QUICK_READ, &pu16_hexOut);
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pu16_hexOut);
        ESOS_TASK_WAIT_ON_SEND_UINT8('\n');     //newline :)
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_TICKS(1000);             //1000ms = 1s
      } while (u8_state == 2);
      ESOS_SENSOR_CLOSE();
    }
    ESOS_TASK_YIELD();
  }
  ESOS_TASK_END();
}

//TODO: POTENTIOMETER INTERFACE (REGISTER TASK)
ESOS_USER_TASK(potenInterface)
{
  static BOOL b_switch1, b_switch2;
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
    if(b_switch2 != esos_uiF14_isSW2Pressed()) {
      b_switch2 = esos_uiF14_isSW2Pressed();
      if(b_switch2) {
        //if we're in state 2, goto yield. else, start continuous output
        if(u8_state == 2) u8_state = 0;
        else u8_state = 2;
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

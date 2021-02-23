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
  static uint8_t u8_state;
  //static ESOS_TASK_HANDLE getADC; //we need something to feed us adc stuff
  ESOS_TASK_BEGIN();
  while(TRUE)
  {
    if(u8_state == 0) {

    }
    //if u8_state case 1: output once before goto state=0
    //if u8_state case 2: output every 1s until "state" flag unset
        //use a do-while(state==2)???
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

//TODO: VOID USER_INIT (call the tasks)
void user_init()
{
    config_esos_uiF14();

    esos_RegisterTask(heartbeat);
    esos_RegisterTask(potenInterface);
    //esos_RegisterTask(info);  //not yet done :(
}

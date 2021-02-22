//team "keith help"
//eli lawrence, erin parker, carol pollard, kane simmons, drew stork
//t4_sensor3.c - ditto sensor2 but display temperature

//includes
#include "revF14.h"
#include "esos.h"
#include "esos_pic24.h"
#include "esos_f14ui.h"
#include "esos_sensor.h"
#include "esos_pic24_sensor.h"

//static declares used in functions
static uint8_t u8_state;

ESOS_USER_TIMER(heartbeat)
{
    esos_uiF14_toggleLED3();
}

//TODO: DISPLAY OUTPUT (TASK) <- output temperatures somewhere in here
ESOS_USER_TASK(info)
{
  static ESOS_TASK_HANDLE getADC; //we need something to feed us adc stuff
  ESOS_TASK_BEGIN();
  while(TRUE)
  {
    //is output enabled?
    //else, is output continuous?
      //create a child task to getADC; then, spawn+wait for shit
      //use a do-while(continuous==TRUE)?
    //else, output once, then yield
      //create a child task to getADC; then, spawn+wait for shit
    ESOS_TASK_YIELD();
  }
  ESOS_TASK_END();
}

//TODO: ADC INTERFACE (TASK)

//TODO: VOID USER_INIT (call the tasks)
void user_init()
{
    config_esos_uiF14();

    esos_RegisterTimer(heartbeat, 250);
    //TODO: esos_RegisterTask(adcInterface);
    //TODO: esos_RegisterTask(info);
}

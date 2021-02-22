//team "keith help"
//eli lawrence, erin parker, carol pollard, kane simmons, drew stork
//t4_sensor.c - flashes a heartbeat LED every 250ms
//              displays potentiometer value in 16b hex when SW1_PRESSED
//              SW2_PRESSED samples potentiometer voltage every 1s until
//                          SW2 or SW1 pressed

//includes
#include "revF14.h"
#include "esos.h"
#include "esos_pic24.h"
#include "esos_f14ui.h"
#include "esos_sensor.h"
#include "esos_pic24_sensor.h"

//TODO: DISPLAY OUTPUT (REGISTER TASK)

//TODO: POTENTIOMETER INTERFACE (REGISTER TASK)

ESOS_USER_TIMER(heartbeat)
{
    esos_uiF14_toggleLED3();
}

//TODO: VOID USER_INIT (call the tasks)
void user_init()
{
    config_esos_uiF14();

    esos_RegisterTimer(heartbeat, 250);
    //TODO: esos_RegisterTask(potenInterface);
    //TODO: esos_RegisterTask(menuHandling);
}

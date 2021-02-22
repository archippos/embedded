//team "keith help"
//eli lawrence, erin parker, carol pollard, kane simmons, drew stork
//t4_sensor2.c - ditto sensor1 first two tasks + SW3 to stop output
//               SW3 press prompts for processing modes and sample rate

//includes
#include "revF14.h"
#include "esos.h"
#include "esos_pic24.h"
#include "esos_f14ui.h"
#include "esos_sensor.h"
#include "esos_pic24_sensor.h"

//TODO: DISPLAY OUTPUT (TASK)

//TODO: INTERFACE WITH ADC (TIMER)

ESOS_USER_TIMER(heartbeat)
{
    esos_uiF14_toggleLED3();
}

//TODO: VOID USER_INIT (call the tasks)
void user_init()
{
    config_esos_uiF14();

    esos_RegisterTimer(heartbeat, 250);
    //TODO: esos_RegisterTask(adcInterface);
    //TODO: esos_RegisterTask(menuHandling);
}

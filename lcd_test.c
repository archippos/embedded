//includes
#include "revF14.h"
#include "esos.h"
#include "esos_pic24.h"
#include "esos_f14ui.h"
#include "esos_sensor.h"
#include "esos_pic24_sensor.h"
#include "esos_lcd44780.h"
#include "esos_pic24_lcd44780.h"

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

ESOS_USER_TASK(lcd_output)
{
	ESOS_TASK_BEGIN();
	while(TRUE){
		__esos_lcd44780_pic24_setDataPins('m');
		ESOS_TASK_WAIT_TICKS( 2 );
	}
	ESOS_TASK_END();
}

// init
void user_init()
{
    config_esos_uiF14();
	esos_lcd44780_configDisplay();

    esos_RegisterTask(heartbeat);
    esos_RegisterTask(lcd_output);
}
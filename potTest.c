//this is code to test the functionality of our POTENTIOMETER
//it is not to be included with any lab submissions
//credit to rashoemake and garrett smith for this code

#include <esos.h>
#include "esos_pic24.h"
#include "esos_pic24_rs232.h"
#include <p33EP512GP806.h>
#include <pic24_all.h>

#include <esos_sensor.h>

#include <stdio.h>
#include <stdlib.h>

#define PRINT_DELAY 1000

char buffer[30];


ESOS_USER_TASK(print_pot) {
    static uint16_t u16_data;

    ESOS_TASK_BEGIN();

    for (;;) {
        //Grab the potentiometer
        ESOS_TASK_WAIT_ON_AVAILABLE_SENSOR(ESOS_SENSOR_CH02, ESOS_SENSOR_VREF_3V3);

        //Now we have exclusive use of the pot
        // ESOS_TASK_WAIT_SENSOR_READ(u16_data, ESOS_SENSOR_ONE_SHOT, ESOS_SENSOR_FORMAT_VOLTAGE);
        ESOS_TASK_WAIT_SENSOR_QUICK_READ(u16_data);

        //Grab the output
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        sprintf(buffer, "%d\n", u16_data);
        ESOS_TASK_WAIT_ON_SEND_STRING(buffer);
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

        ESOS_SENSOR_CLOSE();

        ESOS_TASK_WAIT_TICKS(PRINT_DELAY);
    }

    ESOS_TASK_END();
}

void user_init(void){
    config_esos_uiF14();

    esos_RegisterTask(print_pot);
}

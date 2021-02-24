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

static uint8_t u8_state;
static uint8_t u8_pmode = 1;
static uint8_t u8_samples_input = 0;
static uint8_t u8_proccessConst = ESOS_SENSOR_ONE_SHOT;
static uint16_t pu16_hexOut;

// determine processing constant
void sample_conversion (uint8_t u8_pmode, uint8_t u8_samples_input) {
	if (u8_pmode == 1 && u8_samples_input == 0) {
		u8_proccessConst = ESOS_SENSOR_ONE_SHOT;
	} else if (u8_pmode == 2 && u8_samples_input == 1 ) {
		u8_proccessConst = ESOS_SENSOR_AVG2;
	} else if (u8_pmode == 2 && u8_samples_input == 2 ) {
		u8_proccessConst = ESOS_SENSOR_AVG4;
	} else if (u8_pmode == 2 && u8_samples_input == 3 ) {
		u8_proccessConst = ESOS_SENSOR_AVG8;
	} else if (u8_pmode == 2 && u8_samples_input == 4 ) {
		u8_proccessConst = ESOS_SENSOR_AVG16;
	} else if (u8_pmode == 2 && u8_samples_input == 5 ) {
		u8_proccessConst = ESOS_SENSOR_AVG32;
	} else if (u8_pmode == 2 && u8_samples_input == 6 ) {
		u8_proccessConst = ESOS_SENSOR_AVG64;
	} else if (u8_pmode == 3 && u8_samples_input == 1 ) {
		u8_proccessConst = ESOS_SENSOR_MIN2;
	} else if (u8_pmode == 3 && u8_samples_input == 2 ) {
		u8_proccessConst = ESOS_SENSOR_MIN4;
	} else if (u8_pmode == 3 && u8_samples_input == 3 ) {
		u8_proccessConst = ESOS_SENSOR_MIN8;
	} else if (u8_pmode == 3 && u8_samples_input == 4 ) {
		u8_proccessConst = ESOS_SENSOR_MIN16;
	} else if (u8_pmode == 3 && u8_samples_input == 5 ) {
		u8_proccessConst = ESOS_SENSOR_MIN32;
	} else if (u8_pmode == 3 && u8_samples_input == 6 ) {
		u8_proccessConst = ESOS_SENSOR_MIN64;
	} else if (u8_pmode == 4 && u8_samples_input == 1 ) {
		u8_proccessConst = ESOS_SENSOR_MAX2;
	} else if (u8_pmode == 4 && u8_samples_input == 2 ) {
		u8_proccessConst = ESOS_SENSOR_MAX4;
	} else if (u8_pmode == 4 && u8_samples_input == 3 ) {
		u8_proccessConst = ESOS_SENSOR_MAX8;
	} else if (u8_pmode == 4 && u8_samples_input == 4 ) {
		u8_proccessConst = ESOS_SENSOR_MAX16;
	} else if (u8_pmode == 4 && u8_samples_input == 5 ) {
		u8_proccessConst = ESOS_SENSOR_MAX32;
	} else if (u8_pmode == 4 && u8_samples_input == 6 ) {
		u8_proccessConst = ESOS_SENSOR_MAX64;
	} else if (u8_pmode == 5 && u8_samples_input == 1 ) {
		u8_proccessConst = ESOS_SENSOR_MEDIAN2;
	} else if (u8_pmode == 5 && u8_samples_input == 2 ) {
		u8_proccessConst = ESOS_SENSOR_MEDIAN4;
	} else if (u8_pmode == 5 && u8_samples_input == 3 ) {
		u8_proccessConst = ESOS_SENSOR_MEDIAN8;
	} else if (u8_pmode == 5 && u8_samples_input == 4 ) {
		u8_proccessConst = ESOS_SENSOR_MEDIAN16;
	} else if (u8_pmode == 5 && u8_samples_input == 5 ) {
		u8_proccessConst = ESOS_SENSOR_MEDIAN32;
	} else if (u8_pmode == 5 && u8_samples_input == 6 ) {
		u8_proccessConst = ESOS_SENSOR_MEDIAN64;
	} 
}

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
	  
      //ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_QUICK_READ, &pu16_hexOut);
	  ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_READ, &pu16_hexOut, u8_proccessConst, ESOS_SENSOR_FORMAT_VOLTAGE);
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
        //ESOS_TASK_SPAWN_AND_WAIT(getADC, _WAIT_SENSOR_QUICK_READ, &pu16_hexOut);
		ESOS_TASK_WAIT_SENSOR_READ(pu16_hexOut, u8_proccessConst, ESOS_SENSOR_FORMAT_VOLTAGE);
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(pu16_hexOut);
        ESOS_TASK_WAIT_ON_SEND_UINT8('\n');     //newline :)
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_TICKS(1000);             //1000ms = 1s
      } while (u8_state == 2);
      ESOS_SENSOR_CLOSE();
    } else if(u8_state == 3) {  // prompt the user for processing mode operation

		// menu for processing modes
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("\n1. one-shot\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("2. average\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("3. minimum\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("4. maximum\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("5. median\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("Choose a processing mode: ");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

		ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
		ESOS_TASK_WAIT_ON_GET_UINT8(u8_pmode);
		ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_UINT8(u8_pmode);
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

		// menu for number of samples
		if (u8_pmode != 1) {
			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_STRING("\n\n1. two\n");
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_STRING("2. four\n");
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_STRING("3. eight\n");
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_STRING("4. sixteen\n");
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_STRING("5. thirty-two\n");
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_STRING("6. sixty-four\n");
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_STRING("How many samples: ");
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

			ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
			ESOS_TASK_WAIT_ON_GET_UINT8(u8_samples_input);
			ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_UINT8(u8_samples_input);
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

			ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
			ESOS_TASK_WAIT_ON_SEND_STRING("\n");
			ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
			
		} else {
			u8_samples_input = 0;   // 1 sample taken if one-shot
		}
		sample_conversion(u8_pmode, u8_samples_input);  // determine the processing constant
		u8_state = 0;
	}
    ESOS_TASK_YIELD();
  }
  ESOS_TASK_END();
}

//TODO: POTENTIOMETER INTERFACE (REGISTER TASK)
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

//TODO: VOID USER_INIT (call the tasks)
void user_init()
{
    config_esos_uiF14();

    esos_RegisterTask(heartbeat);
    esos_RegisterTask(potenInterface);
    esos_RegisterTask(info);
}

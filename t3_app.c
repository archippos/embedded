/*

NAME: GROUP KEITH HELP
DATE: 2/12/2021
ASSIGNMENT: LAB #3
PREPARED FOR: KEITH POWELL

*/

#include "esos_f14ui.c"
#include "esos_f14ui.h"

// heartbeat with period of 500 ms on led 3
ESOS_USER_TASK( heartbeat_led ) {

	ESOS_TASK_BEGIN();
	while(TRUE) {
		LED3 = !LED3;
		ESOS_TASK_WAIT_TICKS(500);
	}
	ESOS_TASK_END();
}

ESOS_USER_TASK( SW1_state ) {

	ESOS_TASK_BEGIN();
	while(TRUE) {
		ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("SW1 is pressed\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_UNTIL_UIF14_SW1_RELEASED();
	}
	ESOS_TASK_END();

}

ESOS_USER_TASK( LED1_task ) {

	ESOS_TASK_BEGIN();
	while(TRUE) {
		if ( (esos_uiF14_isSW1Pressed() && esos_uiF14_isSW3Released()) || (esos_uiF14_isSW2Pressed() && esos_uiF14_isSW3Pressed()) ) {
			esos_uiF14_turnLED1On();
		} else {
			esos_uiF14_turnLED1Off();
		}
		ESOS_TASK_WAIT_TICKS(10);
	}
	ESOS_TASK_END();
}

// task for the serial port menu
ESOS_USER_TASK( menu_task ) {
	static uint8_t u8_char;
	static uint8_t state = 0;

	ESOS_TASK_BEGIN();
	while(TRUE) {

		if (state == 0) {  // initial print out of the menu

		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("What would you like to change?\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("1: Switch 1 double press period\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("2: Switch 2 double press period\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("3: Switch 3 double press period\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("4: RPG slow notification threshold\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("5: RPG medium notification threshold\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("6: RPG fast notification threshold\n" );
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("7: Close menu\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

		// ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
		// ESOS_TASK_WAIT_ON_GET_UINT8(u8_char);
		// ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

		// ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		// ESOS_TASK_WAIT_ON_SEND_UINT8(u8_char);
		// ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		state = 1;

		} else {  // other options



		}
		ESOS_TASK_WAIT_TICKS(500);
	}
	ESOS_TASK_END();
}

// user intialization
void user_init(void) {

	__esos_unsafe_PutString( HELLO_MSG );

	config_esos_uiF14();

	esos_RegisterTask( heartbeat_led );
	esos_RegisterTask( LED1_task );
	esos_RegisterTask( SW1_state );
	esos_RegisterTask( menu_task );
}

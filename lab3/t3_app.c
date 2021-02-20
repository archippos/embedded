/*

NAME: GROUP KEITH HELP
DATE: 2/12/2021
ASSIGNMENT: LAB #3
PREPARED FOR: KEITH POWELL

*/

#include  "esos.h"
#include  "esos_pic24.h"
#include "esos_f14ui.c"
#include "revF14.h"

uint16_t _DOUBLE_DELAY = 500;
uint16_t _SLOW_SPEED = 10;
uint16_t _MED_SPEED = 15;
uint16_t _FAST_SPEED = 20;


// heartbeat with period of 500 ms on led 3
ESOS_USER_TASK( heartbeat_led ) {

	ESOS_TASK_BEGIN();
	while(TRUE) {
		LED3 = !LED3;
		ESOS_TASK_WAIT_TICKS(500);
	}
	ESOS_TASK_END();
}

ESOS_USER_TASK(SW1_STATE) {

	ESOS_TASK_BEGIN();
	while(TRUE) {
		ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("SW1 is pressed\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_UNTIL_UIF14_SW1_RELEASED();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("SW1 is released\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
	}
	ESOS_TASK_END();

}

ESOS_USER_TASK(SW2_STATE) {

	ESOS_TASK_BEGIN();
	while(TRUE) {
		ESOS_TASK_WAIT_UNTIL_UIF14_SW2_PRESSED();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("SW2 is pressed\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_UNTIL_UIF14_SW2_RELEASED();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("SW2 is released\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
	}
	ESOS_TASK_END();

}

ESOS_USER_TASK(SW3_STATE) {

	ESOS_TASK_BEGIN();
	while(TRUE) {
		ESOS_TASK_WAIT_UNTIL_UIF14_SW3_PRESSED();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("SW3 is pressed\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_UNTIL_UIF14_SW3_RELEASED();
		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("SW3 is released\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
	}
	ESOS_TASK_END();

}

ESOS_USER_TASK(SW1_DOUBLE_STATE) {
	ESOS_TASK_BEGIN();
  while(TRUE) {
		ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED();
		ESOS_TASK_WAIT_UNTIL_UIF14_SW1_RELEASED();
		ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED();
		ESOS_TASK_WAIT_UNTIL_UIF14_SW1_RELEASED();

		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("SW1 is double pressed\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  }
	ESOS_TASK_END();
}

ESOS_USER_TASK(LED1_TASK) {

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

//task to have led2 flash as rpg turns
//TODO: fix reference errors in compile
ESOS_USER_TASK (LED2_RPG_TASK) {
  ESOS_TASK_BEGIN();

  while(TRUE) {
    if (esos_uiF14_isRPGTurningSlow()) {
      //slow
      esos_uiF14_turnLED2On();     //slow = solid light
    } else if (esos_uiF14_isRPGTurningMedium()) {
      //medium
      esos_uiF14_flashLED2(500);  //medium = 500ms flash
    } else if (esos_uiF14_isRPGTurningFast()) {
      //fast
      esos_uiF14_flashLED2(100);  //fast = 100ms flash
    } else if (!esos_uiF14_isRPGTurning()){
      //no movement
      esos_uiF14_turnLED2Off();   //no rotation, turn off the LED
    }
    ESOS_TASK_YIELD();
  }
  ESOS_TASK_END();
}

// task for the serial port menu
ESOS_USER_TASK(MENU_TASK) {
	static uint8_t u8_char;
	static uint8_t u8_updown = 'z';
	static uint8_t u8_hold;
	static uint16_t u16_num;
	static uint8_t state = 0;

	ESOS_TASK_BEGIN();
	while(TRUE) {

		if (state == 0) {  // initial print out of the menu

		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("\nWhat would you like to change?\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("a: Switch double press period\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("b: RPG slow notification threshold\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("c: RPG medium notification threshold\n");
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

		ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
		ESOS_TASK_WAIT_ON_SEND_STRING("d: RPG fast notification threshold\n\n" );
		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

		state = 1;

		} else {  // other options

		ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
		ESOS_TASK_WAIT_ON_GET_UINT8(u8_char);
		ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

		if(u8_char == 'm' || u8_char == 'M'){
			u8_hold = 'w';
			state = 0;
		}

		if(u8_char == 'a'){
				u16_num = _DOUBLE_DELAY;

				while(u8_hold != 's'){
					if(u8_updown == 'z'){
						ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
						ESOS_TASK_WAIT_ON_SEND_STRING("Use plus or minus keys to change.\n");
						ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

						ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
						ESOS_TASK_WAIT_ON_SEND_STRING("Press s to leave this function.\n");
						ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
					}

					ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
					ESOS_TASK_WAIT_ON_SEND_STRING("\nCurrent Value is: ");
					ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

					ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
					ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING((uint32_t)u16_num);
					ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

					ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
					ESOS_TASK_WAIT_ON_GET_UINT8(u8_updown);
					ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

					if(u8_updown == '-'){
						u16_num = u16_num - 10;
						_DOUBLE_DELAY = u16_num;
					}

					else if(u8_updown == '='){
						u16_num = u16_num + 10;
						_DOUBLE_DELAY = u16_num;
					}
					else{
						if(u8_updown == 's'){
							u8_hold = 's';
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
							u8_updown = 'z';
						}
						else{
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\nNot a valid entry.\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("Use plus or minus keys to change.\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("Press s to leave this function.\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
						}
					}
				}

				u8_hold = 'w';
				state = 0;
		}

		if(u8_char == 'b'){
			u16_num = _SLOW_SPEED;
			while(u8_hold != 's'){
					if(u8_updown == 'z'){
						ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
						ESOS_TASK_WAIT_ON_SEND_STRING("Use plus or minus keys to change.\n");
						ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

						ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
						ESOS_TASK_WAIT_ON_SEND_STRING("Press s to leave this function.\n");
						ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
					}

					ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
					ESOS_TASK_WAIT_ON_SEND_STRING("\nCurrent Slow Value is: ");
					ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

					ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
					ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING((uint32_t)u16_num);
					ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

					ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
					ESOS_TASK_WAIT_ON_GET_UINT8(u8_updown);
					ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

					if(u8_updown == '-'){
						u16_num--;
						if( 0 < u16_num){
							_SLOW_SPEED = u16_num;
						}
						else{
							u16_num++;
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\nSlow Threshold can't be lower than 0.\n ");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
						}
					}

					else if(u8_updown == '='){
						u16_num++;
						if( u16_num < _MED_SPEED){
							_SLOW_SPEED = u16_num;
						}
						else{
							u16_num--;
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\nSlow Speed can't be greater than Med Speed.\n ");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
						}
					}
					else{
						if(u8_updown == 's'){
							u8_hold = 's';
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
							u8_updown = 'z';
						}
						else{
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\nNot a valid entry.\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("Use plus or minus keys to change.\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("Press s to leave this function.\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
						}
					}
				}

				u8_hold = 'w';
				state = 0;
		}

		if(u8_char == 'c'){
			u16_num = _MED_SPEED;
			while(u8_hold != 's'){
					if(u8_updown == 'z'){
						ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
						ESOS_TASK_WAIT_ON_SEND_STRING("Use plus or minus keys to change.\n");
						ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

						ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
						ESOS_TASK_WAIT_ON_SEND_STRING("Press s to leave this function.\n");
						ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
					}

					ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
					ESOS_TASK_WAIT_ON_SEND_STRING("\nCurrent Medium Value is: ");
					ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

					ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
					ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING((uint32_t)u16_num);
					ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

					ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
					ESOS_TASK_WAIT_ON_GET_UINT8(u8_updown);
					ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

					if(u8_updown == '-'){
						u16_num--;
						if( _SLOW_SPEED < u16_num){
							_MED_SPEED = u16_num;
						}
						else{
							u16_num++;
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\nMed Speed can't be lower than Slow Speed.\n ");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
						}
					}

					else if(u8_updown == '='){
						u16_num++;
						if( u16_num < _FAST_SPEED){
							_MED_SPEED = u16_num;
						}
						else{
							u16_num--;
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\nMed Speed can't be greater than Fast Speed.\n ");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
						}
					}
					else{
						if(u8_updown == 's'){
							u8_hold = 's';
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
							u8_updown = 'z';
						}
						else{
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\nNot a valid entry.\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("Use plus or minus keys to change.\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("Press s to leave this function.\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
						}
					}
				}

				u8_hold = 'w';
				state = 0;
		}

		if(u8_char == 'd'){
			u16_num = _FAST_SPEED;
			while(u8_hold != 's'){
					if(u8_updown == 'z'){
						ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
						ESOS_TASK_WAIT_ON_SEND_STRING("Use plus or minus keys to change.\n");
						ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

						ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
						ESOS_TASK_WAIT_ON_SEND_STRING("Press s to leave this function.\n");
						ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
					}

					ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
					ESOS_TASK_WAIT_ON_SEND_STRING("\nCurrent Fast Value is: ");
					ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

					ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
					ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING((uint32_t)u16_num);
					ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

					ESOS_TASK_WAIT_ON_AVAILABLE_IN_COMM();
					ESOS_TASK_WAIT_ON_GET_UINT8(u8_updown);
					ESOS_TASK_SIGNAL_AVAILABLE_IN_COMM();

					if(u8_updown == '-'){
						u16_num--;
						if( _MED_SPEED < u16_num){
							_FAST_SPEED = u16_num;
						}
						else{
							u16_num++;
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\nFast Speed can't be lower than Med Speed.\n ");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
						}
					}

					else if(u8_updown == '='){
						u16_num++;
						if( u16_num < 100){
							_FAST_SPEED = u16_num;
						}
						else{
							u16_num--;
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\nMFast Speed can't be greater than 100.\n ");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
						}
					}
					else{
						if(u8_updown == 's'){
							u8_hold = 's';
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
							u8_updown = 'z';
						}
						else{
							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("\nNot a valid entry.\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("Use plus or minus keys to change.\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

							ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
							ESOS_TASK_WAIT_ON_SEND_STRING("Press s to leave this function.\n");
							ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
						}
					}
				}

				u8_hold = 'w';
				state = 0;
		}
		u8_hold = 'w';
		state = 0;
		}
		ESOS_TASK_WAIT_TICKS(1);
	}
	ESOS_TASK_END();
}

// user intialization
void user_init(void) {

	__esos_unsafe_PutString(HELLO_MSG);

	config_esos_uiF14();

	esos_RegisterTask( heartbeat_led );
	esos_RegisterTask(LED1_TASK);
	esos_RegisterTask(SW1_STATE);
	esos_RegisterTask(SW2_STATE);
	esos_RegisterTask(SW3_STATE);
  esos_RegisterTask(LED2_RPG_TASK);
	esos_RegisterTask(SW1_DOUBLE_STATE);
	esos_RegisterTask(MENU_TASK);
}

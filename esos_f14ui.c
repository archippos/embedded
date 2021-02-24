/*******************************************************************
 *
 * C code framework for ESOS user-interface (UI) service
 *
 *    requires the EMBEDDED SYSTEMS target rev. F14
 *
 * ****************************************************************/
 #include "esos.h"
 #include "esos_pic24.h"

 #include "revF14.h"
 #include "esos_f14ui.h"

// PRIVATE FUNCTIONS
volatile _st_esos_uiF14Data_t _st_esos_uiF14Data;

inline void _esos_uiF14_setRPGCounter (uint16_t newValue) {
    _st_esos_uiF14Data.u16_RPGCounter = newValue;
    return;
}

inline void _esos_uiF14_setLastRPGCounter (uint16_t newValue) {
    _st_esos_uiF14Data.u16_lastRPGCounter = newValue;
    return;
}

inline void _esos_uiF14_setSW1Pressed (void) {
	if (!SW1) {
		_st_esos_uiF14Data.b_SW1Pressed = TRUE;

	} else {
		_st_esos_uiF14Data.b_SW1Pressed = FALSE;
	}
}

inline void _esos_uiF14_setSW1DoublePressed (void) {
	_st_esos_uiF14Data.b_SW1DoublePressed = TRUE;
	_st_esos_uiF14Data.b_SW1Pressed = FALSE;
}

inline void _esos_uiF14_clearSW1DoublePressed (void) {
	_st_esos_uiF14Data.b_SW1DoublePressed = FALSE;
}

inline void _esos_uiF14_setSW2Pressed (void) {
	if (!SW2) {
		_st_esos_uiF14Data.b_SW2Pressed = TRUE;
	} else {
		_st_esos_uiF14Data.b_SW2Pressed = FALSE;
	}
}

inline void _esos_uiF14_setSW3Pressed (void) {
	if (!SW3) {
		_st_esos_uiF14Data.b_SW3Pressed = TRUE;
	} else {
		_st_esos_uiF14Data.b_SW3Pressed = FALSE;
	}
}

// ------------------------
// PUBLIC SWITCH FUNCTIONS
// ------------------------

// Switch 1
inline BOOL esos_uiF14_isSW1Pressed (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==TRUE);
}

inline BOOL esos_uiF14_isSW1Released (void) {
    return (_st_esos_uiF14Data.b_SW1Pressed==FALSE);
}

inline BOOL esos_uiF14_isSW1DoublePressed (void) {
    return (_st_esos_uiF14Data.b_SW1DoublePressed==TRUE);
}

// Switch 2
inline BOOL esos_uiF14_isSW2Pressed (void) {
    return (_st_esos_uiF14Data.b_SW2Pressed==TRUE);
}

inline BOOL esos_uiF14_isSW2Released (void) {
    return (_st_esos_uiF14Data.b_SW2Pressed==FALSE);
}

inline BOOL esos_uiF14_isSW2DoublePressed (void) {
    return (_st_esos_uiF14Data.b_SW2DoublePressed==TRUE);
}

// Switch 3
inline BOOL esos_uiF14_isSW3Pressed (void) {
    return (_st_esos_uiF14Data.b_SW3Pressed==TRUE);
}

inline BOOL esos_uiF14_isSW3Released (void) {
    return (_st_esos_uiF14Data.b_SW3Pressed==FALSE);
}

inline BOOL esos_uiF14_isSW3DoublePressed (void) {
    return (_st_esos_uiF14Data.b_SW3DoublePressed==TRUE);
}


// --------------------
// PUBLIC LED FUNCTIONS
// --------------------

// LED1
inline BOOL esos_uiF14_isLED1On (void) {
    return (_st_esos_uiF14Data.b_LED1On==TRUE);
}

inline BOOL esos_uiF14_isLED1Off (void) {
    return (_st_esos_uiF14Data.b_LED1On==FALSE);
}

inline void esos_uiF14_turnLED1On (void) {
    _st_esos_uiF14Data.b_LED1On = TRUE;
    return;
}

inline void esos_uiF14_turnLED1Off (void) {
    _st_esos_uiF14Data.b_LED1On = FALSE;
    return;
}

inline void esos_uiF14_toggleLED1 (void) {
    _st_esos_uiF14Data.b_LED1On ^= 1;
    return;
}

inline void esos_uiF14_flashLED1( uint16_t u16_period) {
    _st_esos_uiF14Data.u16_LED1FlashPeriod = u16_period;
    return;
}

// LED2
inline BOOL esos_uiF14_isLED2On (void) {
    return (_st_esos_uiF14Data.b_LED2On==TRUE);
}

inline BOOL esos_uiF14_isLED2Off (void) {
    return (_st_esos_uiF14Data.b_LED2On==FALSE);
}

inline void esos_uiF14_turnLED2On (void) {
    _st_esos_uiF14Data.b_LED2On = TRUE;
    return;
}

inline void esos_uiF14_turnLED2Off (void) {
    _st_esos_uiF14Data.b_LED2On = FALSE;
    return;
}

inline void esos_uiF14_toggleLED2 (void) {
    _st_esos_uiF14Data.b_LED2On ^= 1;
    return;
}

inline void esos_uiF14_flashLED2( uint16_t u16_period) {
    _st_esos_uiF14Data.u16_LED2FlashPeriod = u16_period;
    return;
}

// LED3

inline BOOL esos_uiF14_isLED3On (void) {
    return (_st_esos_uiF14Data.b_LED3On==TRUE);
}

inline BOOL esos_uiF14_isLED3Off (void) {
    return (_st_esos_uiF14Data.b_LED3On==FALSE);
}

inline void esos_uiF14_turnLED3On (void) {
    _st_esos_uiF14Data.b_LED3On = TRUE;
    return;
}

inline void esos_uiF14_turnLED3Off (void) {
    _st_esos_uiF14Data.b_LED3On = FALSE;
    return;
}

inline void esos_uiF14_toggleLED3 (void) {
    _st_esos_uiF14Data.b_LED3On ^= 1;
    return;
}

inline void esos_uiF14_flashLED3( uint16_t u16_period) {
    _st_esos_uiF14Data.u16_LED3FlashPeriod = u16_period;
    return;
}


/****** RED, GREEN, and YELLOW functions need to be created *******/

// PUBLIC RPG FUNCTIONS

//gets data from the encoder
inline uint16_t esos_uiF14_getRpgValue_u16 ( void ) {
    return _st_esos_uiF14Data.u16_RPGCounter;
}

inline uint16_t esos_uiF14_getLastRpgValue_u16 ( void ) {
    return _st_esos_uiF14Data.u16_lastRPGCounter;
}

//determines whether or not the encoder is turning
inline BOOL esos_uiF14_isRpgTurning ( void ) {
  //if it's turning, velocity is not 0
  if (esos_uiF14_getRpgVelocity_i16() != 0) {
		return TRUE;
	} else {return FALSE;}
}

//is the new-old delta between 1 and 10?
inline BOOL esos_uiF14_isRpgTurningSlow( void ) {
  // static char sz_reportSlow[64];
  // sz_reportSlow = "Turning slow";
  // //TODO: use getRpgVelocity for this (ditto on medium and fast)
  // //code for determining slow goes here
  // // this is the code for console output --carol
  // ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  // ESOS_TASK_WAIT_ON_SEND_STRING(sz_reportSlow);
  // ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
}

//is the new-old delta between 11 and 24?
inline BOOL esos_uiF14_isRpgTurningMedium( void ) {
  // static char sz_reportMed[64];
  // sz_reportMed = "Turning medium";
  // // code for determining medium goes here
  // //code for console output
  // ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  // ESOS_TASK_WAIT_ON_SEND_STRING(sz_reportMed);
  // ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
}

//is the new-old delta above 25?
inline BOOL esos_uiF14_isRpgTurningFast( void ) {
  // static char sz_reportFast[64];
  // sz_reportFast = "Turning fast";
  // // code for determining speedy boi goes here
  // //code for console output
  // ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  // ESOS_TASK_WAIT_ON_SEND_STRING(sz_reportFast);
  // ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
}

//determines if the encoder turning clockwise
inline BOOL esos_uiF14_isRpgTurningCW( void ) {
  // static char sz_reportCW[64];
  // sz_reportCW = "Turning clockwise";
  // // code for determining CW goes here
  // //code for console output
  // ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  // ESOS_TASK_WAIT_ON_SEND_STRING(sz_reportCW);
  // ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
}

//is the encoder turning counterclockwise
inline BOOL esos_uiF14_isRpgTurningCCW( void ) {
  // static char sz_reportCCW[64];
  // sz_reportCCW = "Turning counterclockwise";
  // // code for determining ccw goes here
  // //code for console output
  // ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  // ESOS_TASK_WAIT_ON_SEND_STRING(sz_reportCCW);
  // ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
}

//obtains the velocity of the encoder
int16_t esos_uiF14_getRpgVelocity_i16( void ) {
	return (esos_uiF14_getRpgValue_u16 - esos_uiF14_getLastRpgValue_u16);
}

// task for double pressing sw1
ESOS_USER_TASK( SW1_double_press ) {
	static uint16_t sw1_dp_tmr;
	static BOOL sw1_dp_hasReleased;

	ESOS_TASK_BEGIN();
	while(TRUE) {
		if( esos_uiF14_isSW1Pressed() ) { // identify if sw1 has been pressed
			_esos_uiF14_clearSW1DoublePressed();
			sw1_dp_tmr = 0;               // start timer for double press
			sw1_dp_hasReleased = FALSE;

			while( sw1_dp_tmr < 500  ) {    // loop while timer not expired
				if ( esos_uiF14_isSW1Pressed() && sw1_dp_hasReleased ) {
					_esos_uiF14_setSW1DoublePressed();
					break;
				} else if ( esos_uiF14_isSW1Released() && !sw1_dp_hasReleased) {
					sw1_dp_hasReleased = TRUE;
				}

				sw1_dp_tmr += 1;
				ESOS_TASK_WAIT_TICKS( 1 );
			}

		}
		ESOS_TASK_WAIT_TICKS(__ESOS_UIF14_UI_PERIOD_MS);
	}
	ESOS_TASK_END();

}


// UIF14 INITIALIZATION FUNCTION

void config_esos_uiF14() {
  // setup your UI implementation
  CONFIG_LED1();
  CONFIG_LED2();
  CONFIG_LED3();

  CONFIG_SW1();
  CONFIG_SW2();
  CONFIG_SW3();

  CONFIG_RPG();

  esos_RegisterTask( SW1_double_press );
  esos_RegisterTask( __esos_uiF14_task );
}

// UIF14 task to manage user-interface
ESOS_USER_TASK( __esos_uiF14_task ){


  ESOS_TASK_BEGIN();
  while(TRUE) {
    // do your UI stuff here

	// check for switch presses
	_esos_uiF14_setSW1Pressed();
	_esos_uiF14_setSW2Pressed();
	_esos_uiF14_setSW3Pressed();

	// check if LED1 on
	if ( esos_uiF14_isLED1On() ) {
		LED1 = 1;
	} else {
		LED1 = 0;
	}

	// check if LED2 on
	if ( esos_uiF14_isLED2On() ) {
		LED2 = 1;
	} else {
		LED2 = 0;
	}

	// check if LED3 on
	if ( esos_uiF14_isLED3On() ) {
		LED3 = 1;
	} else {
		LED3 = 0;
	}


    ESOS_TASK_WAIT_TICKS( __ESOS_UIF14_UI_PERIOD_MS );
  }
  ESOS_TASK_END();
}

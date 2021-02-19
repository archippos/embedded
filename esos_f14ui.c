/*******************************************************************
 *
 * C code framework for ESOS user-interface (UI) service
 *
 *    requires the EMBEDDED SYSTEMS target rev. F14
 *
 * ****************************************************************/
#include "esos_f14ui.h"

// PRIVATE FUNCTIONS
inline void _esos_uiF14_setRPGCounter (uint16_t newValue) {
    _st_esos_uiF14Data.u16_RPGCounter = newValue;
    return;
}

inline void _esos_uiF14_setLastRPGCounter (uint16_t newValue) {
    _st_esos_uiF14Data.u16_lastRPGCounter = newValue;
    return;
}

// ------------------------
// PUBLIC SWITCH FUNCTIONS
// ------------------------

inline void _esos_uiF14_setSW1Pressed (void) {
	if (!SW1) {
		_st_esos_uiF14Data.b_SW1Pressed = TRUE;
	} else {
		_st_esos_uiF14Data.b_SW1Pressed = FALSE;
	}
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


/****** RED, GREEN, and YELLOW functions*******/
inline void esos_uiF14_turnRedLEDOn (void) {
    //red LED = LED1
    esos_uiF14_turnLED1On();
}

inline void esos_uiF14_turnRedLEDOff (void) {
    esos_uiF14_turnLED1Off();
}

inline void esos_uiF14_turnGreenLEDOn (void) {
    //green LED = LED3
    esos_uiF14_turnLED3On();
}

inline void esos_uiF14_turnGreenLEDOff (void) {
    esos_uiF14_turnLED3Off();
}

inline void esos_uiF14_turnYellowLEDOn (void) {
    //yellow LED = LED2
    esos_uiF14_turnLED2On();
}

inline void esos_uiF14_turnYellowLEDOff (void) {
    esos_uiF14_turnLED2Off();
}

// PUBLIC RPG FUNCTIONS

//gets data from the encoder
inline uint16_t esos_uiF14_getRpgValue_u16(void) {
    return _st_esos_uiF14Data.u16_RPGCounter;
}

inline uint16_t esos_uiF14_getLastRpgValue_u16 (void) {
    return _st_esos_uiF14Data.u16_lastRPGCounter;
}

//determines whether or not the encoder is turning
inline BOOL esos_uiF14_isRpgTurning ( void ) {
  //if it's turning, velocity is not 0
  return (esos_uiF14_getRpgVelocity_i16() != 0);
}

//is the new-old delta between 1 and 10?
inline BOOL esos_uiF14_isRpgTurningSlow( void ) {
  // static char sz_reportSlow[64];
  // sz_reportSlow = "Turning slow";
  // this is the code for console output --carol
  // ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  // ESOS_TASK_WAIT_ON_SEND_STRING(sz_reportSlow);
  // ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  uint16_t vel = ABS(_st_esos_uiF14Data.i16_RPGVelocity);
  return esos_uiF14_getRPGSlowThreshold() <= vel && vel < esos_uiF14_getRPGMediumThreshold();
}

//is the new-old delta between 11 and 24?
inline BOOL esos_uiF14_isRpgTurningMedium( void ) {
  // static char sz_reportMed[64];
  // sz_reportMed = "Turning medium";
  //code for console output
  // ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  // ESOS_TASK_WAIT_ON_SEND_STRING(sz_reportMed);
  // ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  uint16_t vel = ABS(_st_esos_uiF14Data.i16_RPGVelocity);
	return esos_uiF14_getRPGMediumThreshold() <= vel && vel < esos_uiF14_getRPGFastThreshold();
}

//is the new-old delta above 25?
inline BOOL esos_uiF14_isRpgTurningFast( void ) {
  // static char sz_reportFast[64];
  // sz_reportFast = "Turning fast";
  //code for console output
  // ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  // ESOS_TASK_WAIT_ON_SEND_STRING(sz_reportFast);
  // ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  return esos_uiF14_getRPGFastThreshold() < ABS(_st_esos_uiF14Data.u16_RPGVelocity);
}

//determines if the encoder turning clockwise
inline BOOL esos_uiF14_isRpgTurningCW( void ) {
  // static char sz_reportCW[64];
  // sz_reportCW = "Turning clockwise";
  //code for console output
  // ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  // ESOS_TASK_WAIT_ON_SEND_STRING(sz_reportCW);
  // ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  uint16_t vel = _st_esos_uiF14Data.i16_RPGVelocity;
  return (vel > 0) && (esos_uiF14_getRPGSlowThreshold() <= ABS(vel));
}

//is the encoder turning counterclockwise
inline BOOL esos_uiF14_isRpgTurningCCW( void ) {
  // static char sz_reportCCW[64];
  // sz_reportCCW = "Turning counterclockwise";
  //code for console output
  // ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
  // ESOS_TASK_WAIT_ON_SEND_STRING(sz_reportCCW);
  // ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
  uint16_t vel = _st_esos_uiF14Data.i16_RPGVelocity;
  return (vel < 0) && (esos_uiF14_getRPGSlowThreshold() <= ABS(vel));
}

//obtains the velocity of the encoder
int16_t esos_uiF14_getRpgVelocity_i16( void ) {
	return (esos_uiF14_getRpgValue_u16 - esos_uiF14_getLastRpgValue_u16);
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

  esos_RegisterTask( __esos_uiF14_task );
}

// UIF14 task to manage user-interface
ESOS_USER_TASK( __esos_uiF14_task ){

  ESOS_TASK_BEGIN();
  while(TRUE) {
    // do your UI stuff here
	_esos_uiF14_setSW1Pressed();
	_esos_uiF14_setSW2Pressed();
	_esos_uiF14_setSW3Pressed();

	if ( esos_uiF14_isLED1On() ) {
		LED1 = 1;
	} else {
		LED1 = 0;
	}

  //double press stuff for sw1
  if (esos_uiF14_isSW1Released() && SW1_changed \
                      && _st_esos_uiF14Data.u16_timeBetweenSW1Presses \
                      >= _st_esos_uiF14Data.u16_doublePressPeriodSW1)
  //TODO: should doublepress time be __DOUBLE_PRESS_TIME?
          //reset timer if button not pressed in time
          _st_esos_uiF14Data.u16_timeBetweenSW1Presses = 0;
          _st_esos_uiF14Data.b_SW1DoublePressed = 0;
      }
      else if (esos_uiF14_isSW1Pressed() && SW1_changed \
                      && _st_esos_uiF14Data.u16_timeBetweenSW1Presses <= _st_esos_uiF14Data.u16_doublePressPeriodSW1 \
                      && _st_esos_uiF14Data.u16_timeBetweenSW1Presses >= 250) //verify this is how it works
      {
          //button was double pressed
          _st_esos_uiF14Data.u16_timeBetweenSW1Presses = 0;
          _st_esos_uiF14Data.b_SW1DoublePressed = 1;
      }

  //double press stuff for s2
  if (esos_uiF14_isSW2Released() && SW2_changed \
                    && _st_esos_uiF14Data.u16_timeBetweenSW2Presses \
                    >= _st_esos_uiF14Data.u16_doublePressPeriodSW2)
    {
        //reset timer if button not pressed in time
        _st_esos_uiF14Data.u16_timeBetweenSW2Presses = 0;
        _st_esos_uiF14Data.b_SW2DoublePressed = 0;
    }
    else if (esos_uiF14_isSW2Pressed() && SW2_changed \
                    && _st_esos_uiF14Data.u16_timeBetweenSW2Presses <= _st_esos_uiF14Data.u16_doublePressPeriodSW2 \
                    && _st_esos_uiF14Data.u16_timeBetweenSW2Presses >= 250) //verify this is how it works
    {
        //button was double pressed
        _st_esos_uiF14Data.u16_timeBetweenSW2Presses = 0;
        _st_esos_uiF14Data.b_SW2DoublePressed = 1;
    }

    //now double press stuff for SW3
    if (esos_uiF14_isSW3Released() && SW3_changed \
                   && _st_esos_uiF14Data.u16_timeBetweenSW3Presses \
                   >= _st_esos_uiF14Data.u16_doublePressPeriodSW3)
    {
        //reset timer if button not pressed in time
        _st_esos_uiF14Data.u16_timeBetweenSW3Presses = 0;
        _st_esos_uiF14Data.b_SW3DoublePressed = 0;
    }
else if (esos_uiF14_isSW3Pressed() && SW3_changed \
                && _st_esos_uiF14Data.u16_timeBetweenSW3Presses <= _st_esos_uiF14Data.u16_doublePressPeriodSW3 \
                && _st_esos_uiF14Data.u16_timeBetweenSW3Presses >= 250)  //verify this is how it works
    {
        //button was double pressed
        _st_esos_uiF14Data.u16_timeBetweenSW3Presses = 0;
        _st_esos_uiF14Data.b_SW3DoublePressed = 1;
    }

    ESOS_TASK_WAIT_TICKS( __ESOS_UIF14_UI_PERIOD_MS );
  }
  ESOS_TASK_END();
}

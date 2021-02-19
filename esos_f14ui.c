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
//#define ABS(x) (x < 0 ? -x : x);

// PRIVATE FUNCTIONS
inline void _esos_uiF14_setRPGCounter (uint16_t newValue) {
    return _st_esos_uiF14Data.u16_RPGCounter = newValue;
}

inline void _esos_uiF14_setLastRPGCounter (uint16_t newValue) {
    return _st_esos_uiF14Data.u16_lastRPGCounter = newValue;
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

inline void esos_uiF14_setSW1DoublePressPeriod(uint16_t value) {
    _st_esos_uiF14Data.u16_doublePressPeriodSW1 = value;
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

inline void esos_uiF14_setSW2DoublePressPeriod(uint16_t value) {
    _st_esos_uiF14Data.u16_doublePressPeriodSW2 = value;
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

inline void esos_uiF14_setSW3DoublePressPeriod(uint16_t value) {
    _st_esos_uiF14Data.u16_doublePressPeriodSW3 = value;
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

inline uint16_t esos_uiF14_getLastRpgValue_u16(void) {
    return _st_esos_uiF14Data.u16_lastRPGCounter;
}

//determines whether or not the encoder is turning
inline BOOL esos_uiF14_isRpgTurning(void) {
  //if it's turning, velocity is not 0
  return (esos_uiF14_getRpgVelocity_i16() != 0);
}

//is the new-old delta between 1 and 10?
inline BOOL esos_uiF14_isRpgTurningSlow(void) {
  /*
  uint16_t vel = ABS(_st_esos_uiF14Data.i16_RPGVelocity);
  return esos_uiF14_getRPGSlowThreshold() <= vel && vel < esos_uiF14_getRPGMediumThreshold();
  */
  return _st_esos_uiF14Data.b_isRPGSlow;
}

//is the new-old delta between 11 and 24?
inline BOOL esos_uiF14_isRpgTurningMedium(void) {
  /*
  uint16_t vel = ABS(_st_esos_uiF14Data.i16_RPGVelocity);
	return esos_uiF14_getRPGMediumThreshold() <= vel && vel < esos_uiF14_getRPGFastThreshold();
  */
  return return _st_esos_uiF14Data.b_isRPGMedium;
}

//is the new-old delta above 25?
inline BOOL esos_uiF14_isRpgTurningFast(void) {
  //return esos_uiF14_getRPGFastThreshold() < ABS(_st_esos_uiF14Data.u16_RPGVelocity);
  return _st_esos_uiF14Data.b_isRPGFast;
}

//determines if the encoder turning clockwise
inline BOOL esos_uiF14_isRpgTurningCW(void) {
  uint16_t vel = _st_esos_uiF14Data.i16_RPGVelocity;
  return (vel > 0) && (esos_uiF14_getRPGSlowThreshold() <= ABS(vel));
}

//is the encoder turning counterclockwise
inline BOOL esos_uiF14_isRpgTurningCCW(void) {
  uint16_t vel = _st_esos_uiF14Data.i16_RPGVelocity;
  return (vel < 0) && (esos_uiF14_getRPGSlowThreshold() <= ABS(vel));
}

//obtains the velocity of the encoder
//returns -1 for ccw, 1 for cw, 0 for no spin
inline int16_t esos_uiF14_getRpgVelocity_i16(void) {
	return _st_esos_uiF14Data.i16_RPGVelocity;
}

//these are for testing:
//test slow period (manual set)
//below medium means we're slow
inline void esos_uiF14_setRPGTurningSlow( uint16_t vel) {
  if (vel <= _st_esos_uiF14Data.u16_RPGMediumSpeed) return;
  _st_esos_uiF14Data.u16_RPGSlowSpeed = vel;
}
//test medium speed (manual set)
//between fast and slow means we're medium
inline void esos_uiF14_setRPGTurningMedium( uint16_t vel) {
  if (vel >= _st_esos_uiF14Data.u16_RPGSlowSpeed) return;
  if (vel <= _st_esos_uiF14Data.u16_RPGFastSpeed) return;
  _st_esos_uiF14Data.u16_RPGMediumSpeed = vel;
}
//test fast speed (manual set)
//above medium means we're SPEEDY SPEEDY
inline void esos_uiF14_setRPGTurningFast( uint16_t vel) {
  if (vel >= _st_esos_uiF14Data.u16_RPGMediumSpeed) return;
  _st_esos_uiF14Data.u16_RPGFastSpeed = vel;
}

//TODO: write a func to interpret numbers passed to the console?

// UIF14 INITIALIZATION FUNCTION

void config_esos_uiF14(void) {
  // setup your UI implementation
  //CONFIG_LED1();
  //CONFIG_LED2();
  //CONFIG_LED3();

  //CONFIG_SW1();
  //CONFIG_SW2();
  //CONFIG_SW3();

  //CONFIG_RPG();

  esos_RegisterTask(__esos_uiF14_task);
  //set up the led flash periods
  _st_esos_uiF14Data.u16_LED1FlashPeriod = 0;
  _st_esos_uiF14Data.u16_LED2FlashPeriod = 0;
  _st_esos_uiF14Data.u16_LED3FlashPeriod = 0;
  _st_esos_uiF14Data.u16_LED1FlashCounter = 0;
  _st_esos_uiF14Data.u16_LED2FlashCounter = 0;
  _st_esos_uiF14Data.u16_LED3FlashCounter = 0;
  //set up the switch press intervals
  _st_esos_uiF14Data.u16_timeBetweenSW1Presses = 0;
  _st_esos_uiF14Data.u16_timeBetweenSW2Presses = 0;
  _st_esos_uiF14Data.u16_timeBetweenSW3Presses = 0;
  //set value of double press to 250ms for ALL switches
  _st_esos_uiF14Data.u16_doublePressPeriodSW1 = __ESOS_DOUBLE_PRESS_TIME_MULT;
  _st_esos_uiF14Data.u16_doublePressPeriodSW2 = __ESOS_DOUBLE_PRESS_TIME_MULT;
  _st_esos_uiF14Data.u16_doublePressPeriodSW3 = __ESOS_DOUBLE_PRESS_TIME_MULT;
  //set up the thresholds for RPG speeds
  _st_esos_uiF14Data.u16_RPGFastSpeed = 250; // 250ms
  _st_esos_uiF14Data.u16_RPGMediumSpeed = 1500; // 1.5s
  _st_esos_uiF14Data.u16_RPGSlowSpeed = 3000; // 3s
  _st_esos_uiF14Data.u16_RPGCounter = _st_esos_uiF14Data.u16_RPGSlowSpeed;
  _st_esos_uiF14Data.u16_lastRPGCounter = _st_esos_uiF14Data.u16_RPGSlowSpeed;
  //we don't know anything about RPG velocity, so set all to false
  _st_esos_uiF14Data.b_isRPGSlow = FALSE;
  _st_esos_uiF14Data.b_isRPGMedium = FALSE;
  _st_esos_uiF14Data.b_isRPGFast = FALSE;
}

// UIF14 task to manage user-interface
ESOS_USER_TASK(__esos_uiF14_task) {

  ESOS_TASK_BEGIN();
  while(TRUE) {
    // do your UI stuff here
    /*
  	_esos_uiF14_setSW1Pressed();
  	_esos_uiF14_setSW2Pressed();
  	_esos_uiF14_setSW3Pressed();

  	if ( esos_uiF14_isLED1On() ) {
  		LED1 = 1;
  	} else {
  		LED1 = 0;
  	}
    */
    //go ahead and zero out these values there pardner...
    _st_esos_uiF14Data.b_SW1DoublePressed = 0;
    _st_esos_uiF14Data.b_SW2DoublePressed = 0;
    _st_esos_uiF14Data.b_SW3DoublePressed = 0;
    //TODO: do we need to do debouncing? or can we bounce that booty like a basketball
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

      //i guess we gotta handle some RPGs or smth bro. idk
      //inc the counter
      _st_esos_uiF14Data.u16_RPGCounter += __ESOS_UIF14_UI_PERIOD_MS;
      //if it's gte then set = 65500
      if (_st_esos_uiF14Data.u16_RPGCounter >= 65500) _st_esos_uiF14Data.u16_RPGCounter = 65500;
      //oh hey stuff is going on ig
      if (RPGA != RPGB && _st_esos_uiF14Data.u16_RPGCounter >= _st_esos_uiF14Data.u16_RPGFastSpeed)
      {
        //take the diff
        _st_esos_uiF14Data.u16_RPGCounter = _st_esos_uiF14Data.u16_RPGCounter - _st_esos_uiF14Data.u16_lastRPGCounter;
        //we have what we need, now update lastRPGCounter to latest value
        _st_esos_uiF14Data.u16_lastRPGCounter = _st_esos_uiF14Data.u16_RPGCounter;
        //figure out velocity of rpg
        if (_st_esos_uiF14Data.b_RPGAHigh == RPGB)
        {
          _st_esos_uiF14Data.i16_RPGVelocity = -1; //CCW baybee
        } else {
          _st_esos_uiF14Data.i16_RPGVelocity = 1;  //CW  baybee
        }
        _st_esos_uiF14Data.b_RPGAHigh = RPGA;
        _st_esos_uiF14Data.b_RPGBHigh = RPGB;
      } else {
        //not turning. sad!
        _st_esos_uiF14Data.i16_RPGVelocity = 0;
      }

    //i have a need. a need for (rpg) speed.
    if (_st_esos_uiF14Data.u16_RPGCounter <= _st_esos_uiF14Data.u16_RPGFastSpeed)
    {
        _st_esos_uiF14Data.b_isRPGFast = TRUE;    //if above is true, then we're speedy
        _st_esos_uiF14Data.b_isRPGMedium = FALSE;
        _st_esos_uiF14Data.b_isRPGSlow = FALSE;
    } else if ( _st_esos_uiF14Data.u16_RPGCounter > _st_esos_uiF14Data.u16_RPGFastSpeed \
            && _st_esos_uiF14Data.u16_RPGCounter <= _st_esos_uiF14Data.u16_RPGMediumSpeed)
    {
        _st_esos_uiF14Data.b_isRPGFast = FALSE;
        _st_esos_uiF14Data.b_isRPGMedium = TRUE;  //if above is true, then we're medium-y
        _st_esos_uiF14Data.b_isRPGSlow = FALSE;
    } else if ( _st_esos_uiF14Data.u16_RPGCounter > _st_esos_uiF14Data.u16_RPGMediumSpeed \
            && _st_esos_uiF14Data.u16_RPGCounter <= _st_esos_uiF14Data.u16_RPGSlowSpeed)
    {
        _st_esos_uiF14Data.b_isRPGFast = FALSE;
        _st_esos_uiF14Data.b_isRPGMedium = FALSE;
        _st_esos_uiF14Data.b_isRPGSlow = TRUE;    //if above is true, then we're slow.
    } else {
        _st_esos_uiF14Data.b_isRPGFast = FALSE;
        _st_esos_uiF14Data.b_isRPGMedium = FALSE;
        _st_esos_uiF14Data.b_isRPGSlow = FALSE;
        //otherwise, we're not moving at all
    }

    ESOS_TASK_WAIT_TICKS( __ESOS_UIF14_UI_PERIOD_MS );
  }
  ESOS_TASK_END();
}

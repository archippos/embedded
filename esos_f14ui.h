/*******************************************************************
 *
 * C header file for ESOS user-interface (UI) service
 *
 *    requires the EMBEDDED SYSTEMS target rev. F14
 *
 * ****************************************************************/

// necessary includes for ESOS functions and types to work
#ifndef ESOS_UIF14_H
#define ESOS_UIF14_H
#include "esos.h"
#include "esos_pic24.h"

#include "revF14.h"
/*
#include "revF14.h"
#include "all_generic.h"
#include "esos.h"
#include "esos_pic24.h"
#include "esos_task.h"
//#include "esos_f14ui.c"
*/

// DEFINEs and CONSTANTs
// Constants
#define __ESOS_UIF14_SW_POLL_RATE 64
#define __ESOS_UIF14_RPG_POLL_RATE 1
#define __ESOS_UIF14_RPG_VEL_RATE 128
#define __ESOS_UIF14_DEFAULT_SWDP_PERIOD 500
#define __ESOS_UIF14_DEFAULT_RPGS_THRESHOLD (0x10)
#define __ESOS_UIF14_DEFAULT_RPGM_THRESHOLD (0x1000)
#define __ESOS_UIF14_DEFAULT_RPGF_THRESHOLD (0x2200)
#define __ESOS_UIF14_RPG_CLICKS_PER_REV 12

#define __ESOS_TICKS_TO_MS(x)           (x/1)
#define __ESOS_MS_TO_TICKS(x)           (x*1)
#define __ESOS_UIF14_UI_PERIOD_MS       10
#define __DOUBLE_PRESS_TIME_MULT        250         //250ms = period for double press
//#define __ESOS_UIF14_DEFAULT_RPGS_THRESHOLD (10)    //threshold for slow
//#define __ESOS_UIF14_DEFAULT_RPGM_THRESHOLD (24)    //threshold for med
//#define __ESOS_UIF14_DEFAULT_RPGF_THRESHOLD (35)    //threshold for fast


// STRUCTURES

typedef struct _st_esos_uiF14Data {
    BOOL b_SW1Pressed;
    BOOL b_SW1DoublePressed;
    uint16_t u16_timeBetweenSW1Presses;
    uint16_t u16_doublePressPeriodSW1;
    BOOL b_SW2Pressed;
    BOOL b_SW2DoublePressed;
    uint16_t u16_timeBetweenSW2Presses;
    uint16_t u16_doublePressPeriodSW2;
    BOOL b_SW3Pressed;
    BOOL b_SW3DoublePressed;
    uint16_t u16_timeBetweenSW3Presses;
    uint16_t u16_doublePressPeriodSW3;

    BOOL b_RPGAHigh;
    BOOL b_RPGBHigh;

    BOOL b_LED1On;
    uint16_t u16_LED1FlashPeriod;
    BOOL b_LED2On;
    uint16_t u16_LED2FlashPeriod;
    BOOL b_LED3On;
    uint16_t u16_LED3FlashPeriod;

    uint16_t u16_RPGCounter;
    int16_t i16_lastRPGCounter;
    uint16_t i16_RPGCounter;
    uint16_t i16_RPGVelocity;
    //uint16_t u16_RPGSlowestSpeed;  //uint
    int16_t u16_RPGSlowThreshold;
    int16_t u16_RPGMediumThreshold;
    int16_t u16_RPGFastThreshold;
    //uint16_t u16_RPGSlowSpeed;   //hee
    //uint16_t u16_RPGMediumSpeed;
    //uint16_t u16_RPGFastSpeed;

    //BOOL b_isRPGSlow;
    //BOOL b_isRPGMedium;
    //BOOL b_isRPGFast;
} _st_esos_uiF14Data_t;

// PRIVATE DATA

extern volatile _st_esos_uiF14Data_t _st_esos_uiF14Data;

// PRIVATE FUNCTION PROTOTYPES

inline uint16_t esos_uiF14_getRPGCounter (void);
inline uint16_t esos_uiF14_getRPGCounter_i16 (void);
void esos_ui_setRPGCounter (uint16_t);

uint16_t esos_uiF14_getLastRPGCounter (void);
void esos_ui_setLastRPGCounter (uint16_t);

ESOS_USER_TASK( __esos_uiF14_task );
ESOS_USER_TIMER(__esos_uiF14_rpg_poll);
ESOS_USER_TIMER(__esos_uiF14_rpg_vel);
//ESOS_USER_TIMER(__esos_uiF14_sw_poll);
// PUBLIC API FUNCTION PROTOTYPES

inline BOOL esos_uiF14_isSW1Pressed (void);
inline BOOL esos_uiF14_isSW1Released (void);
inline BOOL esos_uiF14_isSW1DoublePressed (void);

inline BOOL esos_uiF14_isSW2Pressed (void);
inline BOOL esos_uiF14_isSW2Released (void);
inline BOOL esos_uiF14_isSW2DoublePressed (void);

inline BOOL esos_uiF14_isSW3Pressed (void);
inline BOOL esos_uiF14_isSW3Released (void);
inline BOOL esos_uiF14_isSW3DoublePressed (void);

inline void esos_uiF14_turnLED1On (void);
inline void esos_uiF14_turnLED1Off (void);
inline void esos_uiF14_toggleLED1 (void);
inline void esos_uiF14_flashLED1 (uint16_t);

inline void esos_uiF14_turnLED2On (void);
inline void esos_uiF14_turnLED2Off (void);
inline void esos_uiF14_toggleLED2 (void);
inline void esos_uiF14_flashLED2 (uint16_t);

inline void esos_uiF14_turnLED3On (void);
inline void esos_uiF14_turnLED3Off (void);
inline void esos_uiF14_toggleLED3 (void);
inline void esos_uiF14_flashLED3 (uint16_t);

inline void esos_uiF14_turnRedLEDOn(void);
inline void esos_uiF14_turnRedLEDOff(void);
inline void esos_uiF14_turnGreenLEDOn(void);
inline void esos_uiF14_turnGreenLEDOff(void);
inline void esos_uiF14_turnYellowLEDOn(void);
inline void esos_uiF14_turnYellowLEDOff(void);

inline void esos_uiF14_setSW1DoublePressPeriod (uint16_t);
inline void esos_uiF14_setSW2DoublePressPeriod (uint16_t);
inline void esos_uiF14_setSW3DoublePressPeriod (uint16_t);

//inline uint16_t esos_uiF14_getRPGValue_u16(void);
inline int16_t esos_uiF14_getRPGValue_i16(void);
inline int16_t esos_uiF14_getRPGVelocity_i16(void);
inline BOOL esos_uiF14_isRPGTurning(void);
inline BOOL esos_uiF14_isRPGTurningSlow(void);
inline BOOL esos_uiF14_isRPGTurningMedium(void);
inline BOOL esos_uiF14_isRPGTurningFast(void);
//inline void esos_uiF14_setRPGTurningSlow(uint16_t);
//inline void esos_uiF14_setRPGTurningMedium(uint16_t);
//inline void esos_uiF14_setRPGTurningFast(uint16_t);
inline BOOL esos_uiF14_isRPGTurningCW(void);
inline BOOL esos_uiF14_isRPGTurningCCW(void);
inline void esos_uiF14_resetRPG(void);
inline BOOL esos_uiF14_isRPGTurning(void);
inline BOOL esos_uiF14_isRPGTurningSlow(void);
inline BOOL esos_uiF14_isRPGTurningMedium(void);
inline BOOL esos_uiF14_isRPGTurningFast(void);
inline BOOL esos_uiF14_isRPGTurningCW(void);
inline BOOL esos_uiF14_isRPGTurningCCW(void);
inline int16_t esos_uiF14_getRPGSlowThreshold(void);
inline void esos_uiF14_setRPGSlowThreshold(int16_t threshold);
inline int16_t esos_uiF14_getRPGMediumThreshold(void);
inline void esos_uiF14_setRPGMediumThreshold(int16_t threshold);
inline int16_t esos_uiF14_getRPGFastThreshold(void);
inline void esos_uiF14_setRPGFastThreshold(int16_t threshold);

void config_esos_uiF14();
inline int16_t esos_uiF14_getRPGVelocity_i16(void);

// PUBLIC API ESOS TASK MACROS

#define ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED()              ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW1Pressed() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW1_RELEASED()             ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW1Released() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED_AND_RELEASED() do {  \
                            ESOS_TASK_WAIT_UNTIL_UIF14_SW1_PRESSED(); \
                            ESOS_TASK_WAIT_UNTIL_UIF14_SW1_RELEASED();\
                          } while (0)
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW1_DOUBLE_PRESSED()       ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW1DoublePressed() )

#define ESOS_TASK_WAIT_UNTIL_UIF14_SW2_PRESSED()              ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW2Pressed() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW2_RELEASED()             ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW2Released() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW2_PRESSED_AND_RELEASED() do {    \
                            ESOS_TASK_WAIT_UNTIL_UIF14_SW2_PRESSED(); \
                            ESOS_TASK_WAIT_UNTIL_UIF14_SW2_RELEASED();\
                          } while (0)
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW2_DOUBLE_PRESSED()       ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW2DoublePressed() )

#define ESOS_TASK_WAIT_UNTIL_UIF14_SW3_PRESSED()              ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW3Pressed() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW3_RELEASED()             ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW3Released() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW3_PRESSED_AND_RELEASED() do {    \
                            ESOS_TASK_WAIT_UNTIL_UIF14_SW3_PRESSED(); \
                            ESOS_TASK_WAIT_UNTIL_UIF14_SW3_RELEASED();\
                          } while (0)
#define ESOS_TASK_WAIT_UNTIL_UIF14_SW3_DOUBLE_PRESSED()       ESOS_TASK_WAIT_UNTIL( esos_uiF14_isSW3DoublePressed() )

#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_UNTIL_TURNS()          ESOS_TASK_WAIT_UNTIL( esos_uiF14_isRpgTurning() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_UNTIL_TURNS_CW()       ESOS_TASK_WAIT_UNTIL( esos_uiF14_isRpgTurningCW() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_UNTIL_TURNS_CCW()      ESOS_TASK_WAIT_UNTIL( esos_uiF14_isRpgTurningCCW() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_SLOW()           ESOS_TASK_WAIT_UNTIL( esos_uiF14_isRpgTurningSlow() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_SLOW_CW()        ESOS_TASK_WAIT_UNTIL( esos_uiF14_isRpgTurningSlow() && esos_uiF14_isRpgTurningCW())
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_SLOW_CCW()       ESOS_TASK_WAIT_UNTIL( esos_uiF14_isRpgTurningSlow() && esos_uiF14_isRpgTurningCCW())
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_MEDIUM()         ESOS_TASK_WAIT_UNTIL( esos_uiF14_isRpgTurningMedium() )
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_MEDIUM_CW()      ESOS_TASK_WAIT_UNTIL( esos_uiF14_isRpgTurningMedium() && esos_uiF14_isRpgTurningCW())
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_MEDIUM_CCW()     ESOS_TASK_WAIT_UNTIL( esos_uiF14_isRpgTurningMedium() && esos_uiF14_isRpgTurningCCW())
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_FAST()           ESOS_TASK_WAIT_UNTIL( esos_uiF14_isRpgTurningFast())
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_FAST_CW()        ESOS_TASK_WAIT_UNTIL( esos_uiF14_isRpgTurningFast() && esos_uiF14_isRpgTurningCW())
#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_TURNS_FAST_CCW()       ESOS_TASK_WAIT_UNTIL( esos_uiF14_isRpgTurningFast() && esos_uiF14_isRpgTurningCCW())

#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_MAKES_REV(y)                                                                    \
    {                                                                                                                  \
        int16_t i16_cntr = _esos_uiF14_getRPGValue_i16();                                                              \
        ESOS_TASK_WAIT_UNTIL(_esos_uiF14_getRPGCounter() == i16_cntr + (y * __ESOS_UIF14_RPG_TURNS_PER_REV) ||         \
                             _esos_uiF14_getRPGCounter() == i16_cntr - (y * __ESOS_UIF14_RPG_TURNS_PER_REV));          \
    }

#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_MAKES_CW_REV(y)                                                                 \
    {                                                                                                                  \
        int16_t i16_cntr = _esos_uiF14_getRPGValue_i16();                                                              \
        ESOS_TASK_WAIT_UNTIL(_esos_uiF14_getRPGCounter() == i16_cntr + (y * __ESOS_UIF14_RPG_TURNS_PER_REV));          \
    }

#define ESOS_TASK_WAIT_UNTIL_UIF14_RPG_MAKES_CCW_REV(y)                                                                \
    {                                                                                                                  \
        int16_t i16_cntr = _esos_uiF14_getRPGValue_i16();                                                              \
        ESOS_TASK_WAIT_UNTIL(_esos_uiF14_getRPGCounter() == i16_cntr - (y * __ESOS_UIF14_RPG_TURNS_PER_REV));          \
    }
#define __RPGA_CLEAN_PIN _RD6
#define __RPGB_CLEAN_PIN _RD7
#define __RPG_VALUE (((uint8_t)__RPGA_CLEAN_PIN << 1) | __RPGB_CLEAN_PIN)
#define __RPG_UI_CONFIG()                                                                                              \
    {                                                                                                                  \
        CONFIG_RPG();                                                                                                  \
        CONFIG_RD6_AS_DIG_OUTPUT(); /* Use RD6 as debounced output from RPGA */                                        \
        CONFIG_RD7_AS_DIG_OUTPUT(); /* Use RD7 as debounced output from RPGB */                                        \
    }

#endif    // ESOS_UIF14_H

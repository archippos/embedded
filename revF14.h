#ifndef REVF14_H
#define REVF14_H

#ifndef __dsPIC33EP512GP806__
#define __dsPIC33EP512GP806__
#endif

#include "pic24_all.h"

//set up the LEDs: led1
#define   CONFIG_LED1()   CONFIG_RF4_AS_DIG_OUTPUT()
#define   LED1            _LATF4
#define   LED1_ON()       (_LATF4 = 1)
#define   LED1_OFF()      (_LATF4 = 0)
#define   LED1_TOGGLE()   (_LATF4 = !_LATF4)
//LED2
#define   CONFIG_LED2()   CONFIG_RB14_AS_DIG_OUTPUT()
#define   LED2            _LATB14
#define   LED2_ON()       (_LATB14 = 1)
#define   LED2_OFF()      (_LATB14 = 0)
#define   LED2_TOGGLE()   (_LATB14 = !_LATB14)
//LED3
#define   CONFIG_LED3()   CONFIG_RB15_AS_DIG_OUTPUT()
#define   LED3            _LATB15
#define   LED3_ON()       (_LATB15 = 1)
#define   LED3_OFF()      (_LATB15 = 0)
#define   LED3_TOGGLE()   (_LATB15 = !_LATB15)

//set up the switches: sw1
#define   CONFIG_SW1()    {   CONFIG_RB13_AS_DIG_INPUT();  \
                              ENABLE_RB13_PULLUP();  \
                              DELAY_US(1);  \
                          }
#define   SW1             _RB13
#define   SW1_PRESSED     (_RB13 == 0)
#define   SW1_RELEASED    (_RB13 == 1)
//sw2
#define   CONFIG_SW2()    {   CONFIG_RB12_AS_DIG_INPUT();  \
                              ENABLE_RB12_PULLUP();  \
                              DELAY_US(1);  \
                          }
#define   SW2             _RB12
#define   SW2_PRESSED     (_RB12 == 0)
#define   SW2_RELEASED    (_RB12 == 1)
//sw3
#define   CONFIG_SW3()    {   CONFIG_RC15_AS_DIG_INPUT();  \
                              ENABLE_RC15_PULLUP();  \
                              DELAY_US(1);  \
                          }
#define   SW3             _RC15

#define RPGA          _RB8
#define RPGB          _RB9

#define CONFIG_RPG() {    CONFIG_RB8_AS_DIG_INPUT(); \
                          CONFIG_RB9_AS_DIG_INPUT(); \
                          ENABLE_RB8_PULLUP(); \
                          ENABLE_RB9_PULLUP(); \
                          DELAY_US(1); \
                      }
//high/low for A
#define RPGA_LOW (_RB8 == 0)
#define RPGA_HIGH (_RB8 == 1)
//high/low for B
#define RPGB_LOW (_RB9 == 0)
#define RPGB_HIGH (_RB9 == 1)

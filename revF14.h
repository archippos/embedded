//#ifndef REVF14_H
//#define REVF14_H

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

//configure for analog periphs
#define CONFIG_ANALOG()  {                                                                                             \                       \
        CONFIG_RB2_AS_ANALOG();                                                                                        \
        CONFIG_RB3_AS_ANALOG();                                                                                        \
    }

#define POT1 (RB2_AN) //RB2 as analog
#define TEMP1 (RB3_AN) //RB3 as analog

//http://ww1.microchip.com/downloads/en/devicedoc/70621c.pdf   pic 33 interface code for dac//
#define CONFIG_DAC()  {
        /*      AD1CON1  : ADC Control Register 1 page 16-(7,8)       */
        AD1CON1bits.ADON = 0;    /*turn the thing off    */
        AD1CON1bits.ADSIDL = 0;  /* Idle mode      */

        /* no effect from this because ADDMAEN in CON4 is set to 0
        AD1CON1bits.ADDMABM = 1; DMA buffers are written as they're converted*/

        AD1CON1bits.DMABM = 1;   /* dma buffers    */
        AD1CON1bits.AD12B = 1;   /* 12b mode, 1 channel             */
        AD1CON1bits.FORM = 0b00;    /* integer output mode */


        //These affect the bits of SAMC in Control register 3
        AD1CON1bits.SSRC = 0b0111;  /*auto convert enable*/
        AD1CON1bits.SSRCG = 0;   /* sample source clock group */


        AD1CON1bits.ASAM = 0;    /* auto sample toggle - 0=no */
        AD1CON1bits.SAMP = 0;    /* disable sample */
        AD1CON1bits.DONE = 0;    /* clear done flag */


        /*      AD1CON2     : ADC Control Register 2 page 16-(9,10)     */
        AD1CON2bits.VCFG = 0b01; /*use extern vref+ & internal vref-*/   //This might not be right
        AD1CON2bits.CSCNA = 0;    /* disable input scan */
        AD1CON2bits.CHPS = 0b00;  /* channel select bits, this is 0 for the 12 bit one channel */
        AD1CON2bits.SMPI = 0b00000; /*create interrupt each conversion*/
        AD1CON2bits.BUFM = 0;   /*Starts filling buffer from the start*/                                         \
        AD1CON2bits.ALTS = 0;    /*Always uses channel input selects for Sample A*/

        /*      AD1CON3      : ADC Control Register 3 page 16-11    */
        AD1CON3bits.ADRC = 1;     /*RC clock*/
        AD1CON3bits.SAMC = 0b11111;      /*31 TAD*/

        /*      AD1CON4          */
        AD1CON4bits.ADDMAEN = 0;
        AD1CON1bits.ADON = 1;   /*turn it back on*/
    }

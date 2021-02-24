/*
 * "Copyright (c) 2008 Robert B. Reese, Bryan A. Jones, J. W. Bruce ("AUTHORS")"
 * All rights reserved.
 * (R. Reese, reese_AT_ece.msstate.edu, Mississippi State University)
 * (B. A. Jones, bjones_AT_ece.msstate.edu, Mississippi State University)
 * (J. W. Bruce, jwbruce_AT_ece.msstate.edu, Mississippi State University)
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the authors appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE "AUTHORS" BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE "AUTHORS"
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE "AUTHORS" SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE "AUTHORS" HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 * Please maintain this header in its entirety when copying/modifying
 * these files.
 *
 *
 */

/**
 * \addtogroup ESOS_Task_Sensor_Service
 * @{
 */

/*** I N C L U D E S *************************************************/
#include "esos_pic24_sensor.h"
#include "esos_sensor.h"
#include "pic24_adc.h"

/*** T H E   C O D E *************************************************/

/*********************************************************
 * Public functions intended to be called by other files *
 *********************************************************/

/** \file
 *  Sensor support functions. \see hwxxx_sensor.h for details.
 */

/**
Configure and enable the sensor module for hwxxx hardware.
\param e_senCh   specifies sensor channel
\param e_senVRef specifies sensor voltage reference
\hideinitializer
 */
void esos_sensor_config_hw (esos_sensor_ch_t e_senCh, esos_sensor_vref_t e_senVRef)
{
  CONFIG_ANALOG();
  CONFIG_DAC();

  AD1CON1bits.ADON = 0;  //turn the thing off
  AD1CHS0bits.CH0SA = e_senCh; //AN for ch0 has to be 000000 to 111111
  AD1CHS0bits.CH0NA = 0;       //vref for ch0 is 0
  //shouldn't need Sample B bit

  AD1CHS123 = 0x0000;              //this is an alternate input method we don't need
  AD1CSSH = 0x0000;                //input scan select high
  AD1CSSL = 0x0000;                //input scan select low
  AD1CON1bits.ADON = 1; //turn the thing back on
}

/**
Determine truth of: the sensor is currently converting
\hideinitializer
 */
BOOL esos_sensor_is_converting_hw (void)
{
  if (AD1CON1bits.DONE == 1) {
    //if done, we clear the flag and return false
          esos_ClearUserFlag(ESOS_SENSOR_IS_CONVERTING_FLAG);
          return FALSE;
      }
      //otherwise, we're convering; return true
      return esos_IsUserFlagSet(ESOS_SENSOR_IS_CONVERTING_FLAG);
}

/**
Initiate a conversion for a configured sensor
\hideinitializer
 */
void esos_sensor_initiate_hw (void)
{
  //set the conversion flag and sample bits
  esos_SetUserFlag(ESOS_SENSOR_IS_CONVERTING_FLAG);
  AD1CON1bits.SAMP = 1; //start sampling
}

/**
Receive the value from a conversion that has already been initiated
\hideinitializer
 */
uint16_t esos_sensor_getvalue_u16_hw (void)
{
  //check if the user flag is clear or not
  if (!esos_IsUserFlagClear(ESOS_SENSOR_IS_CONVERTING_FLAG)) {
      return 0;   //if not, exit
  }
  //otherwise, we want that sweet data. gimme gimme gimme gimme
  uint16_t u16_sensor_value = (ADC1BUF0);
  return u16_sensor_value; //return the value of the sensor
}

/**
Release any pending conversions for the sensor
\hideinitializer
 */
void esos_sensor_release_hw (void)
{
  //clear the flag and turn off the adc
  esos_ClearUserFlag(ESOS_SENSOR_IS_CONVERTING_FLAG);
  AD1CON1bits.ADON = 0; //turn the thing off
}

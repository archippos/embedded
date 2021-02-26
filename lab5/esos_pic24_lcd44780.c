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
 * \addtogroup ESOS_Task_LCD_Service
 * @{
 */

/*** I N C L U D E S *************************************************/
#include "revF14.h"
#include "esos_pic24_lcd.h"

/*** T H E   C O D E *************************************************/
void __esos_lcd44780_pic24_config ( void )
{
    // Set up the hardware aspects of the interface of the LCD module service
    //    direction, thresholds, etc beyond what is already done in esos_lcd44780_config()
    LCD44780_E_CONFIG();
    LCD44780_RW_CONFIG();
    LCD44780_RS_CONFIG();
}

void __esos_lcd44780_pic24_setDataPins( uint8_t u8_data) {
	// write the hardware-specific code to take the u8_data passed in
	// and place it on the appropriate data pins
  //one-hot encoding:
    LCD44780_D0 = (u8_data & 0b00000001);
    LCD44780_D1 = (u8_data & 0b00000010) >> 1;
    LCD44780_D2 = (u8_data & 0b00000100) >> 2;
    LCD44780_D3 = (u8_data & 0b00001000) >> 3;
    LCD44780_D4 = (u8_data & 0b00010000) >> 4;
    LCD44780_D5 = (u8_data & 0b00100000) >> 5;
    LCD44780_D6 = (u8_data & 0b01000000) >> 6;
    LCD44780_D7 = (u8_data & 0b10000000) >> 7;
}

uint8_t __esos_lcd44780_pic24_getDataPins( void ) {
	// write the hardware-specific code to read the appropriate data pins
	// and create the uint8 data to return to the caller
    return (LCD44780_D7 << 7) | (LCD44780_D6 << 6) | (LCD44780_D5 << 5) |
           (LCD44780_D4 << 4) | (LCD44780_D3 << 3) | (LCD44780_D2 << 2) |
           (LCD44780_D1 << 1) | (LCD44780_D0);    //whos telling the truth,,,,
}

void __esos_lcd44780_pic24_configDataPinsAsInput( void ) {
	// write the hardware-specific code to set the LCD character module
	// data pins to be "inputs" to prepare for a read of the LCD module
    LCD44780_D4_CONFIG_INPUT();
    LCD44780_D5_CONFIG_INPUT();
    LCD44780_D6_CONFIG_INPUT();
    LCD44780_D7_CONFIG_INPUT();

    __ESOS_LCD44780_HW_SET_RW_READ();
}

void __esos_lcd44780_pic24_configDataPinsAsOutput( void ) {
	// write the hardware-specific code to set the LCD character module
	// data pins to be "outputs" to prepare for a write to the LCD module
    __ESOS_LCD44780_HW_SET_RW_WRITE();

    LCD44780_D4_CONFIG_OUTPUT();
    LCD44780_D5_CONFIG_OUTPUT();
    LCD44780_D6_CONFIG_OUTPUT();
    LCD44780_D7_CONFIG_OUTPUT();
}

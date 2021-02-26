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
#include "esos_pic24_lcd44780.h"

/*** T H E   C O D E *************************************************/
void __esos_lcd44780_pic24_config ( void )
{
    // Set up the hardware aspects of the interface of the LCD module service
    //    direction, thresholds, etc beyond what is already done in esos_lcd44780_config()


}

void __esos_lcd44780_pic24_setDataPins( uint8_t u8_data) {
	// write the hardware-specific code to take the u8_data passed in
	// and place it on the appropriate data pins

}

uint8_t __esos_lcd44780_pic24_getDataPins( void ) {
	// write the hardware-specific code to read the appropriate data pins
	// and create the uint8 data to return to the caller

}

void __esos_lcd44780_pic24_configDataPinsAsInput( void ) {
	// write the hardware-specific code to set the LCD character module
	// data pins to be "inputs" to prepare for a read of the LCD module

}

void __esos_lcd44780_pic24_configDataPinsAsOutput( void ) {
	// write the hardware-specific code to set the LCD character module
	// data pins to be "outputs" to prepare for a write to the LCD module

}

void __esos_lcd44780_configDisplay() {
  // initialize the LCD memory buffers
}

void __esos_lcd44780_init() {
  // execute LCD initialization (refer to datasheet to begin power-up / refreshing screen)
}

// house-keeping functions found in esos_lcd44780.c and to be written by us
void esos_lcd44780_clearScreen( void )
{
	uint8_t u8_height;
	uint8_t u8_width;
	// Clears the buffer
	for(u8_height = 0; u8_height < ESOS_LCD44780_MEM_HEIGHT; u8_height++){
		for(u8_width = 0; u8_width < ESOS_LCD44780_MEM_WIDTH; u8_width++){
			esos_lcd44780_vars.aac_lcdBuffer[u8_height][u8_width] = ' ';
			esos_lcd44780_vars.ab_lcdBufferNeedsUpdate[u8_height][u8_width] = TRUE;
		}
	}

	esos_lcd44780_setCursor(0,0);
	esos_lcd44780_vars.b_cursorPositionNeedsUpdate = TRUE;
}

void esos_lcd44780_setCursor( uint8_t u8_row, uint8_t u8_column )
{
    // Move cursor to (u8_row,u8_column) without changing memory buffer or the display
	// TODO:  Write hardware-independent code here
}

void esos_lcd44780_writeChar( uint8_t u8_row, uint8_t u8_column, uint8_t u8_data )
{
	esos_lcd44780_vars.aac_lcdBuffer[u8_row][u8_column] = u8_data;
	esos_lcd44780_vars.ab_lcdBufferNeedsUpdate[u8_row][u8_column] = TRUE;
}

uint8_t esos_lcd44780_getChar( uint8_t u8_row, uint8_t u8_column )
{
	return esos_lcd44780_vars.aac_lcdBuffer[u8_row][u8_column];
}

void esos_lcd44780_writeBuffer( uint8_t u8_row, uint8_t u8_column, uint8_t *pu8_data, uint8_t u8_bufflen )
{
    // Write u8_bufflen characters from pu8_data to (u8_row,u8_column)
	// TODO:  Write hardware-independent code here
}

void esos_lcd44780_getBuffer( uint8_t u8_row, uint8_t u8_column, uint8_t *pu8_data, uint8_t u8_bufflen )
{
    // Return pu8_data with u8_bufflen characters currently displayed beginning at (u8_row,u8_column)
	// TODO:  Write hardware-independent code here
}

void esos_lcd44780_writeString( uint8_t u8_row, uint8_t u8_column, char *psz_data )
{
    // Write zero-terminated string psz_data to location starting at (u8_row,u8_column)
	// TODO:  Write hardware-independent code here
}

void esos_lcd44780_setCursorDisplay( BOOL u8_state )
{
    // Set cursor display state to u8_state
	// TODO:  Write hardware-independent code here
}

BOOL esos_lcd44780_getCursorDisplay( void )
{
    // Return cursor display state
	// TODO:  Write hardware-independent code here
}

void esos_lcd44780_setCursorBlink( BOOL u8_state )
{
    // Set cursor blink state to u8_state
	// TODO:  Write hardware-independent code here
}

BOOL esos_lcd44780_getCursorBlink( void )
{
    // Return cursor blink state
	// TODO:  Write hardware-independent code here
}

void esos_lcd44780_setDisplayVisible( BOOL u8_state )
{
    // Set display visible state to u8_state
	// TODO:  Write hardware-independent code here
}

BOOL esos_lcd44780_getDisplayVisible( void )
{
    // Return display visible state
	// TODO:  Write hardware-independent code here
}

void esos_lcd44780_setCustomChar( uint8_t u8_charSlot, uint8_t *pu8_charData )
{
    // Set custom character memory for u8_charSlot to data in pu8_charData
	// TODO:  Write hardware-independent code here
}

void esos_lcd44780_getCustomChar( uint8_t u8_charSlot, uint8_t *pu8_charData )
{
    // Return pu8_charData with custom character memory for u8_charSlot
	// TODO:  Write hardware-independent code here
}

// nneded for refresh esos task
BOOL esos_lcd44780_isCurrent( void )
{
	uint8_t u8_row, u8_column;

	if(esos_lcd44780_vars.b_cursorPositionNeedsUpdate ||
	   esos_lcd44780_vars.b_cursorBlinkNeedsUpdate ||
	   esos_lcd44780_vars.b_displayVisibleNeedsUpdate) {
		return FALSE;
	}

	for(u8_row = 0; u8_row < ESOS_LCD44780_MEM_HEIGHT; ++u8_row) {
		for(u8_column = 0; u8_column < ESOS_LCD44780_MEM_WIDTH; ++u8_column) {
			if(esos_lcd44780_vars.ab_lcdBufferNeedsUpdate[u8_row][u8_column]) {
				return FALSE;
			}
		}
	}

	for(u8_column = 0; u8_column < ESOS_LCD44780_NUM_CUSTOM_CHARS; ++u8_column) {
		if(esos_lcd44780_vars.ab_customCharNeedsUpdate[u8_column]) {
			return FALSE;
		}
	}

	return TRUE;
}

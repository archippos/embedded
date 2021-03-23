//this is our application for doing the wave and sensing temeprature

//includes
#include "revF14.h"
#include "fall17lib.h"
#include "esos_menu.h"
#include "esos_f14ui.h"
#include "esos_sensor.h"
#include "esos_pic24.h"
#include "esos_pic24_sensor.h"
#include "esos_pic24_spi.h"
#include "esos_pic24_i2c.h"
#include "esos_pic24_irq.h"
#include "esos_at24c02d.h"
#include "pic24_timer.h"

//defines
#define SINE_WAVFORM_ADDR 0
#define USER_WAVFORM_ADDR 128

#define TRI_WAVFORM 0     //triangle
#define SQUARE_WAVFORM 1  //square
#define SINE_WAVFORM 2    //sine
#define USER_WAVFORM 3    //user-defined waveform

//static variables
static BOOL b_updateLM60;   //update LM60 temp sensor
static BOOL b_updateDS1631; //update DS1631 temp sensor
static uint16_t waveformData[128];  //store our waveform data here

//main menu struct
static esos_menu_longmenu_t home = {
  .u8_numitems= 0,
  .u8_choice = 0,
  .ast_items =
    {
      {"Set","wavfrm", 0},
      {"Set","freqcy", 0},
      {"Set","ampltd", 0},
      {"Set","duty", 1},
      {"Read","LM60", 0},
      {"Read","DS1631", 0},
      {"", "about", 0},
    },
};

//waveform selection struct
static esos_menu_longmenu_t wavfrm = {
  .u8_numitems = 4,
  .u8_choice = 0,
  .ast_items =
  {
    {"Select","triang", 0},
    {"Select","square", 0},
    {"Select","sine", 0},
    {"Select","user", 0},
  },
};

//TODO: frequency struct

//TODO: amplitude struct

//TODO: duty cycle struct

//TODO: lm60 sliderbar struct

//TODO: ds1631 struct

//TODO: about page struct

//TODO: MCP2942 SPI config   funct

//TODO: write to the SPI bus  funct

//TODO: write to the DAC  funct

//TODO: IRQ interrupt  interrupt

//TODO: update waveform  child task

//TODO: lcd menu  user task

//TODO: set LED  user task

//TODO: update the lm60  user task

//TODO: update ds1631  user task

void user_init() {
  //menu task?
  //TODO: call configI2C1, config SPI1, register all tasks, call user interrupt

  /*
  //configure period, timer, fcy, etc
  T4CONbits.T32 = 0;
  T4CON = T4_PS_1_8 | T4_SOURCE_INT;
  TMR4 = 0;
  PR4 = FCY / 8 / 128 / freq.entries[0].value;
  TMR4 = 0;
  ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED(ESOS_IRQ_PIC24_T4);
  T4CONbits.TON = 1;
  */

  //enable T4 user interrupt
}

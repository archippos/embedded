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
//#include "esos_at24c02d.h"
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
      {"Set", "LEDs", 0},
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

//frequency struct
static esos_menu_entry_t freqcy = {
  .entries[0].label = "Freq = ",
  .entries[0].value = 1000, //default is 1k Hz
  .entries[0].min = 64,
  .entries[0].max = 2047,
};

//amplitude struct
//we want to multiply the value by 10 in order to eliminate decimals
static esos_menu_entry_t ampltd = {
  .entries[0].label = "Ampl = ",
  .entries[0].value = 10, //default is 1V
  .entries[0].min = 0,
  .entries[0].max = 30,
};

//duty cycle struct
static esos_menu_entry_t duty = {
  .entries[0].label = "Duty = ",
  .entries[0].value = 50, //default is 50%
  .entries[0].min = 0,
  .entries[0].max = 100,
};

static esos_menu_entry_t leds = {
  .entries[0].label = "LEDs = ",
  .entries[0].value = 3,
  .entries[0].min = 0,
  .entries[0].max = 7,
};

//slider bar stuff for the LM60
static esos_menu_sliderbar_t lm60 = {
  .value = 2500,
  .min = 2000,
  .max = 3500,
  .div = 100,
  .type = 1,
  .lines = {{ "LM60" }, { "" }},
};

//slider bar stuff for the DS1631
static esos_menu_sliderbar_t _1631 = {
  .value = 25,
  .min = 18,
  .max = 34,
  .div = 1,
  .type = 1,
  .lines = {{ "DS1631" }, { "" }},  //display
};

// about menu
static esos_menu_staticmenu_t about = {
  .u8_numlines = 2,                   //2 lines of text
  .u8_currentline = 0,
  .lines = {{ "keith" }, { "help" }}, //display
};

//MCP2942 SPI config   funct
void configSPI1(void) {
  //configure our pins:
  MCP4922_SCK_CONFIG();   //SCK. sick nasty.
  MCP4922_SDI_CONFIG();   //SDI. serial digital interface
  MCP4922_SDO_CONFIG();   //SDO. Super DOg (part of the SDI stuff)
  MCP4922_CS_CONFIG();    //CS: part of the interface logic
  MCP4922_SHDN_CONFIG();  //SHDN: shutdown DAC channels

  //sety mroe stuff
  CONFIG_SCK1OUT_TO_RP(RD5_RP); //this is digital output from eeprom; sck
  CONFIG_SDO1_TO_RP(RD4_RP);    //diogital output for sdo
  CONFIG_SDI1_TO_RP(RD9_RP);    //digital output for sdi

  //now create our config bits for the SPI
  SPI1CON1 = PRI_PRESCAL_1_1 |      // 1:1 primary prescale
             SEC_PRESCAL_3_1 |      // 3:1 secondary prescale
             CLK_POL_ACTIVE_HIGH |  // clock active high (CKP = 0)
             SPI_CKE_ON |           // out changes inactive to active (CKE=0)
             SPI_MODE16_ON |        // 8-bit mode
             MASTER_ENABLE_ON;      // master mode

  MCP4922_ON();           //wakery wakey motherfucker
  SLAVE_DISABLE();        //disable chip select. im your daddy now
  SPI1STATbits.SPIEN = 1; //now set SPI enable bit to 1. yay!
}

//write to the SPI bus  funct
void writeSPI(uint16_t *pu16_out, uint16_t  *pu16_in, uint16_t u16_count) {
  //define some static variables
  static uint16_t *pu16_tempPtrIn;
  static uint16_t *pu16_tempPtrOut;
  static uint16_t u16_tempCount;
  static uint16_t u16_i;
  static uint8_t u8_isReading;
  static uint8_t u8_isWriting;
  static uint8_t u8_waveformIndex = 0;  //start index at 0

  //holds stuff from the SPI1BUF
  uint16_t u16_junk;

  //assign our incoming variables to the ones in this func
  pu16_tempPtrOut = pu16_out;
  pu16_tempPtrIn = pu16_in;
  u16_tempCount = u16_count;

  //if we havea nullptr, then we're not reading
  if (pu16_tempPtrIn == NULLPTR) u8_isReading = FALSE;
  else u8_isReading = TRUE;

  //if we have a nullptr, then we're not writing
  if (pu16_tempPtrOut == NULLPTR) u8_isWriting = FALSE;
  else u8_isWriting = TRUE;

  //clear any overflow bits we may have
  if (SPI1STATbits.SPIROV) SPI1STATbits.SPIROV = 0;
  _SPI1IF = 0;          //clear interrupt; we want to start writing
  u16_junk = SPI1BUF;   //clear out our buffer

  for (u16_i = 0; u16_i < u16_tempCount; u16_i++) {
    //first, check if we're writing
    if (u8_isWriting) {
      //then, store current item of PtrOut into the buffer
      SPI1BUF = *pu16_tempPtrOut;
      pu16_tempPtrOut++;
    } else {
      SPI1BUF = 0;  //otherwise, buffer gets zilch
    }

    //if our buffers are full, do jack shit
    while (SPI1STAT & SPI_TX_BUFFER_FULL) {} //don't do anything
    while (!(SPI1STAT & SPI_RX_BUFFER_FULL)) {} //don't do anything

    u16_junk = SPI1BUF;   //clear out the buffer again

    if (u8_isReading) {
      *pu16_tempPtrIn = u16_junk; //maybe i shouldnt have named it junk. sorry
      pu16_tempPtrIn++;
    }
  }
}

//write to the DAC  funct
/*  command format is: 0b<a/b><buf><GA><SHDN><d11:0>
    eg, ob0011         (max 16b number)
*/
void writeDAC(uint16_t u16_data) {
  //send the write command bb
  u16_data = 0x3000 | (u16_data >> 4);

  SLAVE_ENABLE();                   //cmere, you
  writeSPI(&u16_data, NULLPTR, 1);  //have some data
  SLAVE_DISABLE();                  //ok i'm bored of you
}

//IRQ interrupt  interrupt
ESOS_USER_INTERRUPT(ESOS_IRQ_PIC24_T4) {
  writeDAC(waveformData[u8_waveformIndex]);
  //we want the modulo 128 because there's two halves of our EEPROM storage:
  //first half is for the sine wave, second half for user defined wave.
  u8_waveformIndex = ++u8_waveformIndex % 128;
  ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED(ESOS_IRQ_PIC24_T4);
}

//update waveform  child task
ESOS_CHILD_TASK(updateWaveform, uint8_t u8_type, uint8_t u8_duty, uint8_t u8_ampl) {
  ESOS_TASK_BEGIN();

  //define some static variables
  static uint8_t u16_addr;
  static uint16_t u16_scaledData;
  static uint8_t u8_rawData;
  static uint16_t i;
  static uint8_t u8_currAmpl

  u8_currAmpl = u8_ampl * UINT8_MAX / 30;

  //formula for triangle waveform
  if (u8_type == TRI_WAVFORM) {
    // i am going wupwards
    for (i = 0; i < 64; i++) {
        u8_rawData = i * 2; //even bitches only
        u16_scaledData = u8_rawData * u8_currAmpl;
        waveformData[i] = u16_scaledData * 2;
    }

    // down
    for (i = 64; i > 0; i--) {
        u8_rawData = i * 2;
        u16_scaledData = u8_rawData * u8_currAmpl;
        waveformData[64 + (64 - i)] = u16_scaledData * 2;
    }
  }

  //be there or be square (wave)
  if (u8_type == SQUARE_WAVFORM) {
    // turn duty cycle into fraction of the 128 points per wave period
    u8_duty = 128 * u8_duty / 100;
    for (i = 0; i < 128; i++) {
        u8_rawData = (i < u8_duty) ? 255 : 0;
        u16_scaledData = u8_rawData * u8_currAmpl;

        waveformData[i] = u16_scaledData;
    }
  }

  //read out the sine or user waves:
  //first, check which one we're doing, then go to approp. addr
  if (u8_type == SINE_WAVFORM || u8_type == USER_WAVFORM) {
    if (u8_type == SINE_WAVFORM) {
        u16_addr = SINE_WAVFORM_ADDR;
    } else {
        u16_addr = USER_WAVFORM_ADDR;
    }
/*
    //disable T4 interrupt
    ESOS_DISABLE_PIC24_USER_INTERRUPT(ESOS_IRQ_PIC24_T4);
    //for every piece of the 128 thing...
    for (i = 0; i < 128; i++) {
        ESOS_TASK_WAIT_ON_AVAILABLE_I2C();
        ESOS_TASK_WAIT_ON_WRITE1I2C1(AT24C02D_ADDR, u16_addr + i);
        ESOS_TASK_WAIT_ON_READ1I2C1(AT24C02D_ADDR, u8_rawData);
        ESOS_TASK_SIGNAL_AVAILABLE_I2C(); //release I2C once done
        ESOS_TASK_WAIT_TICKS(1);

        uint16_t a = u8_rawData;
        uint16_t b = u8_currAmpl;
        u16_scaledData = a*b;

        waveformData[i] = u16_scaledData;   //munch
    } */
    //we're done; raise T4
    ESOS_ENABLE_PIC24_USER_INTERRUPT(ESOS_IRQ_PIC24_T4);
  }
  ESOS_TASK_YIELD();
  ESOS_TASK_END();
}

//lcd menu  user task
ESOS_USER_TASK(menuScreen) {
  //define static variables
  static ESOS_TASK_HANDLE updateDisp;

  ESOS_TASK_BEGIN();
  ESOS_ALLOCATE_CHILD_TASK(updateDisp);
  ESOS_TASK_SPAWN_AND_WAIT(updateDisp, updateWaveform, wavfrm.u8_choice, duty.entries[0].value, ampl.entries[0].value);

  while (TRUE) {
    //keep the dispaly static until user presses SW3 to confirm a choice

    //when square wave chosen, show duty cycle (make it not hidden)
    if (wavfrm.u8_choice == 1) {
      mainMenu.ast_items[3].b_hidden = FALSE;
    } else {
      mainMenu.ast_items[3].b_hidden = TRUE;
    }

    //now do long menu stuff
    ESOS_TASK_WAIT_ESOS_MENU_LONGMENU(mainMenu);
    if (mainMenu.u8_choice == 0) {
      //waveform selection menu
      ESOS_TASK_WAIT_ESOS_MENU_ENTRY(wavfrm);
      ESOS_ALLOCATE_CHILD_TASK(updateDisp);
      ESOS_TASK_SPAWN_AND_WAIT(updateDisp, updateWaveform, wavfrm.u8_choice, duty.entries[0].value, ampltd.entries[0].value);
    } else if (mainMenu.u8_choice == 1) {
      //frequency value menu
      ESOS_TASK_WAIT_ESOS_MENU_ENTRY(freqcy);
      PR4 = FCY/8/128/freqcy.entries[0].value;
      ESOS_TASK_WAIT_ON_SEND_UINT32_AS_HEX_STRING(PR4);
      ESOS_TASK_WAIT_ON_SEND_UINT8('\n');
    } else if (mainMenu.u8_choice == 2) {
      //amplitude menu
      ESOS_TASK_WAIT_ESOS_MENU_ENTRY(ampltd);
      ESOS_TASK_SPAWN_AND_WAIT(updateDisp, updateWaveform, wavfrm.u8_choice, duty.entries[0].value, ampltd.entries[0].value);
    } else if (mainMenu.u8_choice == 3) {
      //duty cycle menu
      ESOS_TASK_WAIT_ESOS_MENU_ENTRY(duty);
      ESOS_TASK_SPAWN_AND_WAIT(updateDisp, updateWaveform, wavfrm.u8_choice, duty.entries[0].value, ampltd.entries[0].value);
    } else if (mainMenu.u8_choice == 4) {
      //read from the lm60
      b_updateLM60 = 1;
      ESOS_TASK_WAIT_ESOS_MENU_SLIDERBAR(lm60);  //should this be upper or lower?
      b_updateLM60 = 0;
    } else if (mainMenu.u8_choice == 5) {
      //read from the ds1631
      b_updateDS1631 = 1;
      ESOS_TASK_WAIT_ESOS_MENU_SLIDERBAR(_1631);
      b_updateDS1631 = 0;
    } else if (mainMenu.u8_choice == 6) {
      //set the leds
      ESOS_TASK_WAIT_ESOS_MENU_ENTRY(leds);
    } else if (mainMenu.u8_choice == 7) {
      //go to the about menu
      ESOS_TASK_WAIT_ESOS_MENU_STATICMENU(about);
    }
  }
  ESOS_TASK_END();
}

//set LED  user task
ESOS_USER_TASK(setLED) {
  ESOS_TASK_BEGIN();

  //turn them all off
  LED1_OFF();
  LED2_OFF();
  LED3_OFF();

  //now go through and do the actual setting of stuff
  while (TRUE) {
    LED1 = (leds.entries[0].value & 0b100) == 0b100;
    LED2 = (leds.entries[0].value & 0b010) == 0b010;
    LED3 = (leds.entries[0].value & 0b001) != 0b001;

    ESOS_TASK_WAIT_TICKS(50);   //delay
  }

  ESOS_TASK_END();
}

//TODO: update the ds1631  user task
ESOS_USER_TASK(updateDS1631) {
  ESOS_TASK_BEGIN();

  //define some static variables
  static uint8_t i, u8_low, u8_hi;
  static char str_temp32[12];

  //initialize i2c
  ESOS_TASK_WAIT_ON_AVAILABLE_I2C();
  ESOS_TASK_WAIT_ON_WRITE2I2C1(DS1631ADDR, 0xAC, 0x0C);
  ESOS_TASK_WAIT_ON_WRITE1I2C1(DS1631ADDR, 0x51);
  ESOS_TASK_SIGNAL_AVAILABLE_I2C();
  ESOS_TASK_WAIT_TICKS(250); // delay

  while (TRUE) {
    if (b_updateDS1631) {
      ESOS_TASK_WAIT_ON_AVAILABLE_I2C();
      ESOS_TASK_WAIT_ON_WRITE1I2C1(DS1631ADDR, 0xAA); //0xAA = read cmd
      ESOS_TASK_WAIT_ON_READ2I2C1(DS1631ADDR, u8_hi, u8_low);
      ESOS_TASK_SIGNAL_AVAILABLE_I2C();

      convert_uint32_t_to_str(u8_hi, str_temp32, 12, 10);
      str_temp32[2] = '.';
      u8_low = u8_low * 100 / 256;
      convert_uint32_t_to_str(u8_low, str_temp32 + 3, 8, 10);
      //if between these two values...
      if (u8_low >= 0 && u8_low <= 9) {
          //...do this
          str_temp32[4] = str_temp32[3];
          str_temp32[3] = '0';
      }

      str_temp32[5] = 'C';  //celsius :)

      for (i = 0; i < 8; i++) {
        _1631.lines[1][i] = str_temp32[i];
      }
      _1631.value = u8_hi;

      ESOS_TASK_WAIT_TICKS(750); //delay 750ms - polling rate of 12b mode
      continue;
    }
    ESOS_TASK_YIELD();
  }
  ESOS_TASK_END();
}

//TODO: update lm60  user task
ESOS_USER_TASK(updateLM60) {
  static ESOS_TASK_HANDLE read_temp;
  static uint16_t pu16_out;
  static uint32_t pu32_out, temp32_ipart, temp32_fpart;
  static char str_temp32[12];
  static uint8_t i;

  ESOS_TASK_BEGIN();

  while (TRUE) {
    if (b_updateLM60) {
      ESOS_ALLOCATE_CHILD_TASK(read_temp);
      ESOS_TASK_SPAWN_AND_WAIT(read_temp, _WAIT_ON_AVAILABLE_SENSOR, TEMP_CHANNEL, ESOS_SENSOR_VREF_3V0);
      ESOS_TASK_SPAWN_AND_WAIT(read_temp, _WAIT_SENSOR_READ, &pu16_out, ESOS_SENSOR_ONE_SHOT,
                               ESOS_SENSOR_FORMAT_VOLTAGE);
      ESOS_SENSOR_CLOSE();

      pu32_out = (uint32_t)pu16_out * 1000; //we don't want decimals, so *1000
      pu32_out = (pu32_out - 424000) / 625; //take mV -> temperature

      temp32_ipart = pu32_out / 100; //get just the integer part
      temp32_fpart = pu32_out - temp32_ipart * 100; //now get just the decimal part

      convert_uint32_t_to_str(temp32_ipart, str_temp32, 12, 10);
      str_temp32[2] = '.';
      convert_uint32_t_to_str(temp32_fpart, str_temp32 + 3, 8, 10);

      if (temp32_fpart >= 0 && temp32_fpart <= 9) {
        str_temp32[4] = str_temp32[3];
        str_temp32[3] = '0';
      }

      str_temp32[5] = 'C';

      for (i = 0; i < 8; i++) {
        lm60.lines[1][i] = str_temp32[i];
      }

      lm60.value = pu32_out;
    }
      ESOS_TASK_WAIT_TICKS(125);
  }
  ESOS_TASK_END();
}

void user_init() {
  //init/call/register our stuff
  esos_menu_init();
  esos_pic24_configI2C1(400);   //baud rate of 400
  configSPI1();
  esos_RegisterTask(menuScreen);
  esos_RegisterTask(setLED);
  esos_RegisterTask(updateLM60);
  esos_RegisterTask(updateDS1631);

  //register user interrupt
  ESOS_REGISTER_PIC24_USER_INTERRUPT(ESOS_IRQ_PIC24_T4, ESOS_USER_IRQ_LEVEL2, _T4Interrupt);

  //configure period, timer, fcy, etc
  T4CONbits.T32 = 0;
  T4CON = T4_PS_1_8 | T4_SOURCE_INT;
  TMR4 = 0;
  PR4 = FCY / 8 / 128 / freq.entries[0].value;
  TMR4 = 0;
  ESOS_MARK_PIC24_USER_INTERRUPT_SERVICED(ESOS_IRQ_PIC24_T4);
  T4CONbits.TON = 1;

  //enable T4 user interrupt
  ESOS_ENABLE_PIC24_USER_INTERRUPT(ESOS_IRQ_PIC24_T4);
}

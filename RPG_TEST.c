#include "esos_f14ui.h"
#include "esos.h"
#include "esos_pic24.h"
//#include "esos_lcd44780.h"
//#include "esos_pic24_lcd44780.h"
static uint8_t state = 1;
static uint8_t sw3_flag = 0;

#define   CONFIG_SW3()    {   CONFIG_RC15_AS_DIG_INPUT();  \
                              ENABLE_RC15_PULLUP();  \
                              DELAY_US(1);  \
                          }
#define   SW3             _RC15

ESOS_USER_TASK(rpg_interface)
{
    ESOS_TASK_BEGIN();
    //static uint16_t i16_rpg_fast_val;
    static BOOL cw, ccw;

    while (1) {

        if(cw == 1)
        {

          state++;

          cw = 0;
          ESOS_TASK_YIELD();
          ESOS_TASK_WAIT_TICKS(1400);

        }

        if(esos_uiF14_isRPGTurningCW())
        {
          cw = 1;
        }


        if(ccw == 1)
        {

          state--;

          ccw = 0;
          ESOS_TASK_YIELD();
          ESOS_TASK_WAIT_TICKS(1400);

        }

        if(esos_uiF14_isRPGTurningCCW())
        {
          ccw = 1;
        }

        if(SW3 == 0)
        {
          sw3_flag = 1;
        }
        else{
          sw3_flag = 0;
        }


        ESOS_TASK_WAIT_TICKS(10);
        ESOS_TASK_YIELD();
    }

        ESOS_TASK_END();
};


ESOS_USER_TASK(menu)
{
    static uint8_t prev_state = -1;
    ESOS_TASK_BEGIN();
    //static uint16_t i16_rpg_fast_val;
    while(1)
    {

      if ( sw3_flag == 0 )
      {

        if(state != prev_state)
        {
          if(state > 0x000000008)
          {
            state = 0x000000001;

          }
          if(state < 0x000000001)
          {
            state = 0x000000008;

          }

          if ( state == 0x000000001 )
          {
            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        		ESOS_TASK_WAIT_ON_SEND_STRING("Set wvform\n");
        		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
          }
          if ( state == 0x000000002 )
          {
            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        		ESOS_TASK_WAIT_ON_SEND_STRING("Set freq\n");
        		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
          }
          if ( state == 0x000000003 )
          {
            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        		ESOS_TASK_WAIT_ON_SEND_STRING("Set ampltd\n");
        		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
          }
          if ( state == 0x000000004 )
          {
            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        		ESOS_TASK_WAIT_ON_SEND_STRING("Set duty\n");
        		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
          }
          if ( state == 0x000000005 )
          {
            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        		ESOS_TASK_WAIT_ON_SEND_STRING("Read LM60\n");
        		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
          }
          if ( state == 0x000000006 )
          {
            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        		ESOS_TASK_WAIT_ON_SEND_STRING("Read 1631\n");
        		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
          }
          if ( state == 0x000000007 )
          {
            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        		ESOS_TASK_WAIT_ON_SEND_STRING("Set LEDs\n");
        		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
          }
          if ( state == 0x000000008 )
          {
            ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        		ESOS_TASK_WAIT_ON_SEND_STRING("About...\n");
        		ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
          }

        }
      }
      else{

        ESOS_TASK_WAIT_TICKS(100); //temporary

        if ( state == 0x000000001 )
        {
          ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
          ESOS_TASK_WAIT_ON_SEND_STRING("State 1 display\n");
          ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        }
        if ( state == 0x000000002 )
        {
          ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
          ESOS_TASK_WAIT_ON_SEND_STRING("State 2 display\n");
          ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        }
        if ( state == 0x000000003 )
        {
          ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
          ESOS_TASK_WAIT_ON_SEND_STRING("State 3 display\n");
          ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        }
        if ( state == 0x000000004 )
        {
          ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
          ESOS_TASK_WAIT_ON_SEND_STRING("State 4 display\n");
          ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        }
        if ( state == 0x000000005 )
        {
          ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
          ESOS_TASK_WAIT_ON_SEND_STRING("State 5 display\n");
          ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        }
        if ( state == 0x000000006 )
        {
          ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
          ESOS_TASK_WAIT_ON_SEND_STRING("State 6 display\n");
          ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        }
        if ( state == 0x000000007 )
        {
          ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
          ESOS_TASK_WAIT_ON_SEND_STRING("State 7 display\n");
          ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        }
        if ( state == 0x000000008 )
        {
          ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
          ESOS_TASK_WAIT_ON_SEND_STRING("State 8 display\n");
          ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        }

      }
      prev_state = state;
      ESOS_TASK_YIELD();
    }

    ESOS_TASK_END();
};




void user_init()
{
    config_esos_uiF14();
    esos_RegisterTask(rpg_interface);
    esos_RegisterTask(menu);

    CONFIG_SW3();
}

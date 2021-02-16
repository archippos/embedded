/*

NAME: GROUP *
DATE: 2/6/2021
ASSIGNMENT: LAB #2
PREPARED FOR: KEITH POWELL

*/

#include    "esos.h"
#include "revF14.h"
#ifdef __linux
#include    "esos_pc.h"
#include    "esos_pc_stdio.h"
#include "all_generic.h"
#include "esos_task.h"
#include "esos_mail.h"
#include "esos_comm.h"
#include "esos_irq.h"

// INCLUDE these so that printf() and our PC hacks work
#include <stdio.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>

#else
#include    "esos_pic24.h"
#include    "esos_pic24_rs232.h"
#endif

// DEFINEs go here
#ifndef __linux


// #define   CONFIG_LED1()   CONFIG_RF4_AS_DIG_OUTPUT()
// #define   LED1            _LATF4
// #define   CONFIG_LED2()   CONFIG_RB14_AS_DIG_OUTPUT()
// #define   LED2            _LATB14
// #define   CONFIG_LED3()   CONFIG_RB15_AS_DIG_OUTPUT()
// #define   LED3            _LATB15

// #define   CONFIG_SW1()    {   CONFIG_RB13_AS_DIG_INPUT();  \
                              // ENABLE_RB13_PULLUP();  \
                              // DELAY_US(1);  \
                          // }
// #define   SW1             _RB13


// #define   CONFIG_SW2()    {   CONFIG_RB12_AS_DIG_INPUT();  \
                              // ENABLE_RB12_PULLUP();  \
                              // DELAY_US(1);  \
                          // }
// #define   SW2             _RB12


// #define   CONFIG_SW3()    {   CONFIG_RC15_AS_DIG_INPUT();  \
                              // ENABLE_RC15_PULLUP();  \
                              // DELAY_US(1);  \
                          // }
// #define   SW3             _RC15



#else
//#define   CONFIG_LED3()   printf("Called CONFIG_LED3()\n");
//uint8_t     LED1 = 0;
//uint8_t     LED2 = 0;
//uint8_t     LED3 = 0;      // LED3 is initially "on"



#endif

// PROTOTYPEs go here

// GLOBALs go here
//  Generally, the user-created semaphores will be defined/allocated here


#ifdef __linux
/*
 * Simulate the timer ISR found on a MCU
 *   The PC doesn't have a timer ISR, so this task will periodically
 *   call the timer services callback instead.
 *   USED ONLY FOR DEVELOPMENT AND TESTING ON PC.
 *   Real MCU hardware doesn't need this task
 */
ESOS_USER_TASK( __simulated_isr ) {
  ESOS_TASK_BEGIN();
  while (TRUE) {
    // call the ESOS timer services callback just like a real H/W ISR would
    __esos_tmrSvcsExecute();
    ESOS_TASK_WAIT_TICKS( 1 );

  } // endof while(TRUE)
  ESOS_TASK_END();
} // end child_task
#endif

/************************************************************************
 * User supplied functions
 ************************************************************************
 */
/*
 * An ESOS task to mimic the heartbeat LED found
 * in the PIC24 support library code used in Chapters 8-13.
 *
 * Toggle LED1, wait 250ms, repeat forever.
 *
 * \note Since this heartbeat is performed in an ESOS task,
 * a flashing LED indicates that the ESOS scheduler is still
 * running properly.  If the LED quits flashing, the ESOS
 * scheduler is no longer rotating through the runnable task
 * list.  The most likely reason is that some task has ceased
 * "yielding" the processor, and is caught in some deadlock
 * or otherwise infinite loop.
 * \hideinitializer
 */



ESOS_USER_TASK(main_task) {

  ESOS_TASK_BEGIN();

  while (1)
  {



#ifdef __linux
    if (LED2) {
      printf("\a");
      fflush(stdout);
    }
#endif

    //ESOS_TASK_WAIT_TICKS( 500 );
  } // endof while(TRUE)
  ESOS_TASK_END();
} // end heartbeat_LED task

/****************************************************
 *  user_init()
 ****************************************************
 */
void user_init(void) {

  // Call the hardware-provided routines to print the
  // HELLO_MSG to the screen.  Must use this call because
  // the ESOS communications subsystems is not yet fully
  // initialized, since this call is in user_init()
  //
  // In general, users should call hardware-specific
  // function like this.

  __esos_unsafe_PutString( HELLO_MSG );

#ifdef __linux
  // register our little ESOS task to mimic MCU's TIMER T1 IRQ which kicks off
  // the ESOS S/W timers when they expire
  esos_RegisterTask( __simulated_isr );
  //esos_RegisterTimer(swTimerLED, 250);
#endif

  // configure our hardware as needed by the tasks
  CONFIG_LED1();
  CONFIG_LED2();
  CONFIG_LED3();

  CONFIG_SW1();
  CONFIG_SW2();
  CONFIG_SW3();

  // user_init() should register at least one user task

  //calls task
  esos_RegisterTask(main_task);




} // end user_init()

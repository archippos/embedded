/*

NAME: GROUP *
DATE: 2/6/2021
ASSIGNMENT: LAB #2
PREPARED FOR: KEITH POWELL

*/

#include    "esos.h"
#include "revF14.h"
#include "esos_f14ui.h"
//#include "esos_f14ui.c"
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




#else




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

//Creating threshold variables for double presses and RPG counter
//Threshold values adjusted for 10ms period and TBD
int SW1_double_threshold = 25;
int SW2_double_threshold = 25;
int SW3_double_threshold = 25;
//RPG thresholds TBD
int RPG_slow_threshold = 0;
int RPG_medium_threshold = 10;
int RPG_fast_threshold = 30;

ESOS_USER_TIMER(heartbeat){
  ESOS_TASK_BEGIN();
  while (1){
    LED3 = !LED3;
  }//end of while
  ESOS_TASK_END();

}

ESOS_USER_TASK(l1_task){
  ESOS_TASK_BEGIN();
  while (1){

  }//end of while
  ESOS_TASK_END();

}

ESOS_USER_TASK(l2_task){
  ESOS_TASK_BEGIN();
  while (1){

  }//end of while
  ESOS_TASK_END();

}

ESOS_USER_TASK(menu_task) {

  ESOS_TASK_BEGIN();

  while (1)
  {
    //Print out menu with options for encoder and double press periods
    //Yes I know this looks awful
    int select;
    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("What would you like to change?");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("1: Switch 1 double press period");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("2: Switch 2 double press period");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("3: Switch 3 double press period");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("4: RPG slow notification threshold");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("5: RPG medium notification threshold");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("6: RPG fast notification threshold" );
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
    ESOS_TASK_WAIT_ON_SEND_STRING("7: Close menu");
    ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

    cin >> select;
    //Begin state machine for menu
    switch(select) {
      case 1:
        //Wait for keyboard input
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_STRING("Use the up and down arrows to change input");
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();

        //PROTOTYPE FOR READING ARROW KEYS
        if (getch() == '\033') { // if the first value is esc
          getch(); // skip the [
          switch(getch()) { // the real value
              case 'A':
                  // code for arrow up
                  break;
              case 'B':
                  // code for arrow down
                  break;
              default:
                cout "Invalid button press. Please use the up and down arrow keys." << endl;
                break;
          }
      }

      case 2:
        //Wait for keyboard input
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_STRING("Use the up and down arrows to change input");
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        if (getch() == '\033') { // if the first value is esc
          getch(); // skip the [
          switch(getch()) { // the real value
              case 'A':
                  // code for arrow up
                  break;
              case 'B':
                  // code for arrow down
                  break;
              default:
                cout "Invalid button press. Please use the up and down arrow keys." << endl;
                break;
      case 3:
        //Wait for keyboard input
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_STRING("Use the up and down arrows to change input");
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        if (getch() == '\033') { // if the first value is esc
          getch(); // skip the [
          switch(getch()) { // the real value
              case 'A':
                  // code for arrow up
                  break;
              case 'B':
                  // code for arrow down
                  break;
              default:
                cout "Invalid button press. Please use the up and down arrow keys." << endl;
                break;
      case 4:
        //Wait for keyboard input
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_STRING("Use the up and down arrows to change input");
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        if (getch() == '\033') { // if the first value is esc
          getch(); // skip the [
          switch(getch()) { // the real value
              case 'A':
                  // code for arrow up
                  break;
              case 'B':
                  // code for arrow down
                  break;
              default:
                cout "Invalid button press. Please use the up and down arrow keys." << endl;
                break;
      case 5:
        //Wait for keyboard input
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_STRING("Use the up and down arrows to change input");
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        if (getch() == '\033') { // if the first value is esc
          getch(); // skip the [
          switch(getch()) { // the real value
              case 'A':
                  // code for arrow up
                  break;
              case 'B':
                  // code for arrow down
                  break;
              default:
                cout "Invalid button press. Please use the up and down arrow keys." << endl;
                break;
      case 6:
        //Wait for keyboard input
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_STRING("Use the up and down arrows to change input");
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        if (getch() == '\033') { // if the first value is esc
          getch(); // skip the [
          switch(getch()) { // the real value
              case 'A':
                  // code for arrow up
                  break;
              case 'B':
                  // code for arrow down
                  break;
              default:
                cout "Invalid button press. Please use the up and down arrow keys." << endl;
                break;
      case 7:
        ESOS_TASK_WAIT_ON_AVAILABLE_OUT_COMM();
        ESOS_TASK_WAIT_ON_SEND_STRING("Menu closed");
        ESOS_TASK_SIGNAL_AVAILABLE_OUT_COMM();
        break;
    }

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
  /*
  CONFIG_LED1();
  CONFIG_LED2();
  CONFIG_LED3();

  CONFIG_SW1();
  CONFIG_SW2();
  CONFIG_SW3();

  */
  config_esos_uiF14();
  // user_init() should register at least one user task

  //calls task
  esos_RegisterTask(menu_task);
  esos_RegisterTimer(heartbeat, 500);
  esos_RegisterTask(l1_task);
  esos_RegisterTask(l2_task);



} // end user_init()

/****************************************************************************
 Module
   experiment.c

 Description
   Module for implementing the main UI for gathering and recording user feedback.

 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 01/15/16 10:53 mal     Begin coding
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "conio.h"

/*---------------------------- Macro Definitions ---------------------------*/
#define MAX_CUR_POS         11 //Maximum cursor position
#define CURSOR_ASCII_VAL    124 //ASCII for a block

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this module. */
static void print_slidebar( uint8_t );
static void *RunExperiment( void *);
static void *DummyThreadFunc( void *);

/****************************************************************************
 Function
    main

 Parameters
    nothing
 Returns
    nothing
 Description
    This is the main function. It uses multi-threading (pthread.c) which is generally supported by
different computer systems. The UI experiment will run on a separate thread. If there
are any other control loops run them as new thread as well. Please be careful with
the use of variables shared between threads (if using them consider using locks
to not have racing conditions)

 Author
     Michael Lin, 01/15/16, 11:25
****************************************************************************/
void main( void )
{
    int success;   //Used to indicate whether pthread was initiated succesfully

    /* If creating another thread make your own pointer to the thread */
    pthread_t experiment_pth; //Pointer to the thread that runs the main experiment loop.
    pthread_t new_pth; //Example of a new pointer to a new thread

    success = pthread_create(&experiment_pth, NULL, RunExperiment, NULL); //Kick off the experiment thread
    success |= pthread_create(&new_pth, NULL, DummyThreadFunc, NULL); //Kick off demo thread

    if (success == 0) {
        printf("\r\n Successfully created thread(s)\n\r");
    } else {
        printf("\r\n Failed to create thread(s)\n\r");
    }
    pthread_join(experiment_pth, NULL);
}



/****************************************************************************
 Function
    RunExperiment

 Parameters
    nothing
 Returns
    nothing
 Description
    Prints the slide bar cursor in the commanded position. This is a thread function.

 Author
     Michael Lin, 01/15/16, 11:25
****************************************************************************/
void *RunExperiment( void *arg)
{
    static char c;
    static uint8_t cursor_pos = 5; //Begin cursor position in the middle
    // Main loop
    printf("\rPlease enter left or right\n");
    while (1)
    {
        print_slidebar(cursor_pos);
        c = getch();
        // Press l for moving right
        if (c == 'l') {
            cursor_pos = (cursor_pos + 1) % MAX_CUR_POS;
        // Press h for moving left
        } else if (c == 'h') {
            if (cursor_pos == 0) {
                cursor_pos = MAX_CUR_POS - 1;
            } else {
                cursor_pos = (cursor_pos - 1);
            }
        } else if (c == 't') {
            printf("\b");
        } else if (c == 32) {
            printf("\n");
            return NULL;
        }
    } 
}

/****************************************************************************
 Function
    DummyThreadFunc

 Parameters
    arg: this is the arguments that can be passed in when calling pthread_create
 Returns
    nothing
 Description
    Example of a new thread function. This thread terminates once this function returns.

 Author
     Michael Lin, 01/15/16, 11:25
****************************************************************************/
void *DummyThreadFunc( void *arg)
{
    printf("\rYou entered dummy thread! :)\n");
    printf("\rNow you are exiting dummy thread!\n");
    return NULL;
}

/****************************************************************************
 Function
     print_slidebar

 Parameters
     uint8_t: where the slide bar should be positioned

 Returns
     nothing
 Description
    Prints the slide bar cursor in the commanded position
 Notes

 Author
     Michael Lin, 01/13/16, 11:25
****************************************************************************/
void print_slidebar( uint8_t cursor_pos)
{
    switch (cursor_pos) {
        case 0:
            printf("\r%c . . . . . . . . . .", CURSOR_ASCII_VAL);
            break;
        case 1:
            printf("\r. %c . . . . . . . . .", CURSOR_ASCII_VAL);
            break;
        case 2:
            printf("\r. . %c . . . . . . . .", CURSOR_ASCII_VAL);
            break;
        case 3:
            printf("\r. . . %c . . . . . . .", CURSOR_ASCII_VAL);
            break;
        case 4:
            printf("\r. . . . %c . . . . . .", CURSOR_ASCII_VAL);
            break;
        case 5:
            printf("\r. . . . . %c . . . . .", CURSOR_ASCII_VAL);
            break;
        case 6:
            printf("\r. . . . . . %c . . . .", CURSOR_ASCII_VAL);
            break;
        case 7:
            printf("\r. . . . . . . %c . . .", CURSOR_ASCII_VAL);
            break;
        case 8:
            printf("\r. . . . . . . . %c . .", CURSOR_ASCII_VAL);
            break;
        case 9:
            printf("\r. . . . . . . . . %c .", CURSOR_ASCII_VAL);
            break;
        case 10:
            printf("\r. . . . . . . . . . %c", CURSOR_ASCII_VAL);
            break;

    }
}

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
#include <string.h>
#include <pthread.h>
#include "conio.h"
#include "experiment.h"
#include "data_collect.h"

/*---------------------------- Macro Definitions ---------------------------*/
#define MAX_CUR_POS         23 //Maximum cursor position
#define CURSOR_ASCII_VAL    124 //ASCII for a block
#define NUM_INIT_MSGS       3
#define NUM_EXP_MSGS        2

#define StringServiceNumber       0
#define ValueServiceNumber        1

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this module. */
static void RunExperiment( void );
static void print_slidebar( uint8_t );
void *InitExperiment( void *);

/*---------------------------- Module Variables ---------------------------*/
/* The following are the variables that will be accessed by the data collector. Be
   very careful with these since I am not using mutex, I am ensuring that I flag
   the data collector only after I have written the user response to the variable */
char *StringResp;
static uint8_t ValResp;
static uint8_t cursor_pos;
static uint8_t myLock;   //Used to lock current thread if data is being pulled out

/* Define the array of questions to be asked when InitExperiment happens */
static const char *InitMsgsArray[NUM_INIT_MSGS] = {
    "\r\nPlease enter your first name\n\r",
    "\r\nPlease enter your last name\n\r",
    "\r\nPlease enter your subject number\n\r"
};


/* Define the array of questions to be asked during the experiment */
static const char *ExperimentMsgsArray[NUM_INIT_MSGS] = {
    "\r\nHow far to the left or right did you feel you were going?\n\r",
    "\r\nHow strong did you feel the feedback?\n\r"
};

static const char *ExperimentScaleMsgsArray[NUM_INIT_MSGS][3] = {
    {"Left", "Center", "Right"},
    {"Weak", "", "Strong"}
};


/****************************************************************************
 Function
    InitExperiment

 Parameters
    nothing
 Returns
    nothing
 Description
    Ask subject some basic questions in order to log their background info

 Author
     Michael Lin, 01/15/16, 11:25
****************************************************************************/
void *InitExperiment( void *arg )
{
    printf("\rYou've entered Experiment thread\n");
    StringResp = (char *) malloc(MAX_STR_RESP_LEN*sizeof(char));
    char response[MAX_STR_RESP_LEN];
    // Main loop
    for (int i = 0; i < NUM_INIT_MSGS; i++) {
        while (myLock == 1); //Wait if my data is currently locked
        printf("%s", InitMsgsArray[i]);
        //pthread_mutex_lock(&stdin_lock);
        scanf("%[^\n]%*c", StringResp);
        //fgets(StringResp, sizeof(StringResp), stdin);
        //pthread_mutex_unlock(&stdin_lock);
        setFlag(StringServiceNumber);
    }
    while (myLock == 1); //Wait if my data is currently locked
    free(StringResp);
    StringResp = NULL;
    RunExperiment(); //Pass it on to run experiment
    return NULL;
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
void RunExperiment( void )
{
    static char c;
    cursor_pos = 5; //Begin cursor position in the middle
    static uint8_t counter = 0; //counter for the current message being sent
    static uint8_t qprint_flag = 0; //Flag to indicate when to print question message
    // Main loop
    while (counter < 2)
    {
        while (myLock == 1); //Wait if my data is currently locked
        if (qprint_flag == 0) {
            printf("%s", ExperimentMsgsArray[counter]);
            printf("\r\n%s<<<<<<<<<<<<<<<<<<<<%s>>>>>>>>>>>>>>>>>>>%s\n\n\r", ExperimentScaleMsgsArray[counter][0], 
                                                                              ExperimentScaleMsgsArray[counter][1],
                                                                              ExperimentScaleMsgsArray[counter][2]);
            qprint_flag = 1;
        }
        //printf("hi");
        print_slidebar(cursor_pos);
        c = getch();
        // Press l for moving right
        if (c == 67) {
            if (cursor_pos < MAX_CUR_POS) {
              cursor_pos++;
            }
        // Press h for moving left
        } else if (c == 68) {
            if (cursor_pos > 0) {
                cursor_pos--;
            }
        } else if (c == '\n') {
            ValResp = cursor_pos;
            setFlag(ValueServiceNumber);
            counter++;
            qprint_flag = 0;
        } else if (c == 't') {
            printf("\b");
        } else if (c == 32) {
            printf("\n");
        }
    } 
    printf("\r\n Congrats! You are done with the experiment. \n\r");
    printf("\r\n CTRL+C to finish. \n\r");
}

/****************************************************************************
 Function
    getExperimentThreadHandler

 Parameters
    nothing
 Returns
    void*: pointer to the function being used by the thread
 Description

 Author
     Michael Lin, 01/21/16, 11:25
****************************************************************************/
void *getExperimentThreadHandler( void )
{
    return &InitExperiment;
}


/****************************************************************************
 Function
    intHandler

 Parameters
    nothing
 Returns
    nothing
 Description
    called when CTRL C happens. Frees the memory allocated

 Author
     Michael Lin, 01/21/16, 11:25
****************************************************************************/
void intHandler( void )
{
    printf("\r\n caught ctrl-c! \n\r");
    if (StringResp != NULL) {
      free(StringResp);
    }
    close_file();
    exit(0);
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
            printf("\r%c . . . . . . . . . . . . . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                                       
        case 1:                                          
            printf("\r. %c . . . . . . . . . . . . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                                       
        case 2:                                          
            printf("\r. . %c . . . . . . . . . . . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                                       
        case 3:                                          
            printf("\r. . . %c . . . . . . . . . . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                                       
        case 4:                                          
            printf("\r. . . . %c . . . . . . . . . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                                       
        case 5:                                          
            printf("\r. . . . . %c . . . . . . . . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                                       
        case 6:                                          
            printf("\r. . . . . . %c . . . . . . . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                                       
        case 7:                                          
            printf("\r. . . . . . . %c . . . . . . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                                       
        case 8:                                          
            printf("\r. . . . . . . . %c . . . . . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                                       
        case 9:                                          
            printf("\r. . . . . . . . . %c . . . . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;
        case 10:
            printf("\r. . . . . . . . . . %c . . . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                 
        case 11:
            printf("\r. . . . . . . . . . . %c . . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                 
        case 12:
            printf("\r. . . . . . . . . . . . %c . . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                 
        case 13:                   
            printf("\r. . . . . . . . . . . . . %c . . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                 
        case 14:                   
            printf("\r. . . . . . . . . . . . . . %c . . . . . . . . ", CURSOR_ASCII_VAL);
            break;                 
        case 15:                   
            printf("\r. . . . . . . . . . . . . . . %c . . . . . . . ", CURSOR_ASCII_VAL);
            break;                 
        case 16:                   
            printf("\r. . . . . . . . . . . . . . . . %c . . . . . . ", CURSOR_ASCII_VAL);
            break;                 
        case 17:                   
            printf("\r. . . . . . . . . . . . . . . . . %c . . . . . ", CURSOR_ASCII_VAL);
            break;                 
        case 18:                   
            printf("\r. . . . . . . . . . . . . . . . . . %c . . . . ", CURSOR_ASCII_VAL);
            break;                 
        case 19:                   
            printf("\r. . . . . . . . . . . . . . . . . . . %c . . . ", CURSOR_ASCII_VAL);
            break;                 
        case 20:                   
            printf("\r. . . . . . . . . . . . . . . . . . . . %c . . ", CURSOR_ASCII_VAL);
            break;                 
        case 21:                   
            printf("\r. . . . . . . . . . . . . . . . . . . . . %c . ", CURSOR_ASCII_VAL);
            break;                 
        case 22:                   
            printf("\r. . . . . . . . . . . . . . . . . . . . . . %c ", CURSOR_ASCII_VAL);
            break;

    }
}

/****************************************************************************
 Function
    getStringResp

 Parameters
    nothing
 Returns
    char *: String
 Description
    Returns the string response from the user
 Author
     Michael Lin, 01/21/16, 11:25
****************************************************************************/
char *getStringResp( void )
{
    return StringResp;
}

/****************************************************************************
 Function
    getValResp

 Parameters
    nothing
 Returns
    uint8_t: the value input that user gave
 Description
    Returns the string response from the user
 Author
     Michael Lin, 01/21/16, 11:25
****************************************************************************/
uint8_t getValResp( void )
{
    return ValResp;
}

/****************************************************************************
 Function
    lockExperimentThread

 Parameters
    nothing
 Returns
    nothing
 Description
    sets my lock to 1 so that I dont touch the data being written until it is done
 Author
     Michael Lin, 01/21/16, 11:25
****************************************************************************/
void lockExperimentThread( void )
{
    myLock = 1;
}

/****************************************************************************
 Function
    unlockExperimentThread

 Parameters
    nothing
 Returns
    nothing
 Description
    sets my lock to 0 to indicate that data has been written and it is safe to update new values
 Author
     Michael Lin, 01/21/16, 11:25
****************************************************************************/
void unlockExperimentThread( void )
{
    myLock = 0;
}

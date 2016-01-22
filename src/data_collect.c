/****************************************************************************
 Module
   data_collect.c

 Description
   Module for data collection and storage. It uses FIFO Queue system.

 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 01/20/16 10:53 mal     Begin coding
****************************************************************************/
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "data_collect.h"
#include "experiment.h"
#include "main.h"

/*----------------------------- Module Defines ----------------------------*/
#define NUM_SERVICES    2
#define ARRAY_SIZE(x)   (sizeof(x)/sizeof(x[0]))

/*---------------------------- Module Variables ---------------------------*/
static uint8_t flagCollect[NUM_SERVICES];
static FILE *fp;

/*---------------------------- Module Prototypes ---------------------------*/
void *RunDataService( void* );
void clearFlag( uint8_t );

/****************************************************************************
 Function
    RunDataService

 Parameters
    nothing
 Returns
    void*
 Description
    Initializes the necessary Queues for storing data

 Author
     Michael Lin, 01/20/16, 11:25
****************************************************************************/
void *RunDataService( void *arg )
{
    fp = fopen("data/test_data.csv", "w");
    while (1) { //forever loop in run data service
        for (int i=0; i< ARRAY_SIZE(flagCollect); i++) {
            if (flagCollect[i]) {  //If it is not zero then we need to process it
                char stringResp[MAX_STR_RESP_LEN];
                uint8_t ValResp;
                lockExperimentThread();
                pthread_mutex_lock(&data_lock);
                switch (i) {
                    case 0:  //Flag came from experiment thread to log string
                        strcpy(stringResp, getStringResp());
                        fprintf(fp, "%s\n", stringResp);
                        clearFlag(i); //clear the flag
                        break;
                    case 1: //Flag came from experiment thread to log values
                        ValResp = getValResp();
                        fprintf(fp, "%d\n", ValResp);
                        clearFlag(i); //clear the flag
                        break;
                }
                pthread_mutex_unlock(&data_lock);
                unlockExperimentThread();
            }
        }
    }
}


/****************************************************************************
 Function
    getDataThreadHandler

 Parameters
    nothing
 Returns
    void*: pointer to the function being used by the thread
 Description

 Author
     Michael Lin, 01/21/16, 11:25
****************************************************************************/
void *getDataThreadHandler( void )
{
    return RunDataService;
}


/****************************************************************************
 Function
    setFlag

 Parameters
    uint8_t: which flag to set high
 Returns
    void
 Description
    Sets a flag high to indicate what data to collect

 Author
     Michael Lin, 01/21/16, 11:25
****************************************************************************/
void setFlag ( uint8_t whichFlag )
{
    if (whichFlag < NUM_SERVICES) {
      flagCollect[whichFlag] = 1;
    }
}

/****************************************************************************
 Function
    clearFlag

 Parameters
    uint8_t: which flag to clear
 Returns
    void
 Description
    Clears a flag that indicates data to collect

 Author
     Michael Lin, 01/21/16, 11:25
****************************************************************************/
void clearFlag ( uint8_t whichFlag )
{
    if (whichFlag < NUM_SERVICES) {
      flagCollect[whichFlag] = 0;
    }
}

/****************************************************************************
 Function
    closeFile

 Parameters
    void
 Returns
    void
 Description
    closes the file that is being written to

 Author
     Michael Lin, 01/21/16, 11:25
****************************************************************************/
void close_file( void )
{
    printf("\r\nClosed data file\n\r");
    fclose(fp);
}

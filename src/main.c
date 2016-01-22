/****************************************************************************
 Module
   main.c

 Description
   Module for implementing the main loop.

 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 01/21/16 10:53 mal     Begin coding
****************************************************************************/
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include "experiment.h"
#include "data_collect.h"
#include "main.h"

/*---------------------------- Module Functions ---------------------------*/
static void *DummyThreadFunc( void *);


/****************************************************************************
 Function
    main

 Parameters
    nothing
 Returns
    int: indicate success or failure
 Description
    This is the main function. It uses multi-threading (pthread.c) which is generally supported by
different computer systems. The UI experiment will run on a separate thread. If there
are any other control loops run them as new thread as well. Please be careful with
the use of variables shared between threads (if using them consider using locks
to not have racing conditions)

 Author
     Michael Lin, 01/15/16, 11:25
****************************************************************************/
int main( void )
{
    signal(SIGINT, intHandler);
    
    int success;   //Used to indicate whether pthread was initiated succesfully

    /* Using mutex lock for shared data */
    success = pthread_mutex_init(&data_lock, NULL);
    success |= pthread_mutex_init(&stdin_lock, NULL);

    /* If creating another thread make your own pointer to the thread */
    pthread_t experiment_pth; //Pointer to the thread that runs the main experiment loop.
    pthread_t data_collect_pth; //Pointer to the thread that runs the main data collection loop.
    pthread_t new_pth; //Example of a new pointer to a new thread

    /* Start off all threads */
    success |= pthread_create(&experiment_pth, NULL, getExperimentThreadHandler(), NULL); //Kick off the experiment thread
    success |= pthread_create(&data_collect_pth, NULL, getDataThreadHandler(), NULL); //Kick off the experiment thread
    success |= pthread_create(&new_pth, NULL, DummyThreadFunc, NULL); //Kick off demo thread

    if (success == 0) {
        printf("\r\n Successfully created thread(s)\n\r");
    } else {
        printf("\r\n Failed to create thread(s)\n\r");
    }
    pthread_join(experiment_pth, NULL);
    pthread_join(data_collect_pth, NULL);
    pthread_join(new_pth, NULL);
    pthread_mutex_destroy(&data_lock);
    pthread_mutex_destroy(&stdin_lock);
    return 0; //to indicate success
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
    //printf("\rYou entered dummy thread! :)\n");
    //printf("\rNow you are exiting dummy thread!\n");
    return NULL;
}

#if 0
/****************************************************************************
 Function
    getLock

 Parameters
    nothing
 Returns
    pthread_mutex_t*: pointer to the mutex lock
 Description
    Returns the mutex lock used for sharing variables

 Author
     Michael Lin, 01/15/16, 11:25
****************************************************************************/
pthread_mutex_t *getLock( void )
{
    return &lock;
}
#endif

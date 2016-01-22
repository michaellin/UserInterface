typedef enum {  ES_NO_EVENT,
                ES_SUBJECT_INPUT,
                ES_ENCODER_DATA
            } ES_EventTyp_t;

typedef struct ES_Event_t {
    ES_EventTyp_t EventType;    // what kind of event?
    uint16_t   EventParam;      // parameter value for use w/ this event
}ES_Event;


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
#include "data_collect.h"
#include "ES_Queue.h"

/*----------------------------- Module Defines ----------------------------*/

typedef struct {
    ES_Event *pMem;       // pointer to the memory
    uint8_t Size;      // how big is it
}ES_QueueDesc_t;

#define NUM_SERVICES 2
#define ARRAY_SIZE(x)  (sizeof(x)/sizeof(x[0]))

/*---------------------------- Module Variables ---------------------------*/

/****************************************************************************/
// The queues for the services

static ES_Event Queue0[10];
#if NUM_SERVICES > 1
static ES_Event Queue1[10];
#endif
#if NUM_SERVICES > 2
static ES_Event Queue2[10];
#endif
#if NUM_SERVICES > 3
static ES_Event Queue3[10];
#endif
#if NUM_SERVICES > 4
static ES_Event Queue4[10];
#endif
#if NUM_SERVICES > 5
static ES_Event Queue5[10];
#endif
#if NUM_SERVICES > 6
static ES_Event Queue6[10];
#endif
#if NUM_SERVICES > 7
static ES_Event Queue7[10];
#endif
#if NUM_SERVICES > 8
static ES_Event Queue8[10];
#endif
#if NUM_SERVICES > 9
static ES_Event Queue9[10];
#endif
#if NUM_SERVICES > 10
static ES_Event Queue10[10];
#endif

/****************************************************************************/
// array of queue descriptors for posting by priority level

static ES_QueueDesc_t const EventQueues[NUM_SERVICES] = {
  { Queue0, ARRAY_SIZE(Queue0) }
#if NUM_SERVICES > 1
, { Queue1, ARRAY_SIZE(Queue1) }
#endif
#if NUM_SERVICES > 2
, { Queue2, ARRAY_SIZE(Queue2) }
#endif
#if NUM_SERVICES > 3
, { Queue3, ARRAY_SIZE(Queue3) }
#endif
#if NUM_SERVICES > 4
, { Queue4, ARRAY_SIZE(Queue4) }
#endif
#if NUM_SERVICES > 5
, { Queue5, ARRAY_SIZE(Queue5) }
#endif
#if NUM_SERVICES > 6
, { Queue6, ARRAY_SIZE(Queue6) }
#endif
#if NUM_SERVICES > 7
, { Queue7, ARRAY_SIZE(Queue7) }
#endif
#if NUM_SERVICES > 8
, { Queue8, ARRAY_SIZE(Queue8) }
#endif
#if NUM_SERVICES > 9
, { Queue9, ARRAY_SIZE(Queue9) }
#endif
#if NUM_SERVICES > 10
, { Queue10, ARRAY_SIZE(Queue10) }
#endif
};



/****************************************************************************
 Function
    InitService

 Parameters
    nothing
 Returns
    nothing
 Description
    Initializes the necessary Queues for storing data

 Author
     Michael Lin, 01/20/16, 11:25
****************************************************************************/
void InitDataService( void )
{
    // Initialize all Queues
    for (int i=0; i< ARRAY_SIZE(EventQueues); i++) {
        ES_InitQueue( EventQueues[i].pMem, EventQueues[i].Size );
    }
}


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
    while (1) {
        for (int i=0; i< ARRAY_SIZE(EventQueues); i++) {
            ES_QueueDesc_t CurrentQueue = EventQueues[i];
            ES_Event NewEvent;
            if (ES_IsQueueEmpty(CurrentQueue[i]) == false) {
                ES_DeQueue(CurrentQueue, &NewEvent);
                switch (NewEvent) {
                    case ES_SUBJECT_INPUT:
                        

                }
            }
        }
    }
}

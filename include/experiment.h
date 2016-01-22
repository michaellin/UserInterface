#ifndef Experiment_H
#define Experiment_H

#include <stdint.h>
#include <stdio.h>

/*----------------------------- Public Defines ----------------------------*/
#define MAX_STR_RESP_LEN    512 //Max String Response length

/*---------------------------- Module Variables ---------------------------*/
pthread_mutex_t stdin_lock;

/*----------------------------- Module Prototypes ----------------------------*/
void *getExperimentThreadHandler( void );
char *getStringResp( void );
uint8_t getValResp( void );
void intHandler( void );
void lockExperimentThread( void );
void unlockExperimentThread( void );

#endif

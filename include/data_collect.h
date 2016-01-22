#ifndef DataCollect_H
#define DataCollect_H

#include <stdint.h>
#include <stdbool.h>

/*---------------------------- Module Variables ---------------------------*/
pthread_mutex_t data_lock;

/*----------------------------- Module Prototypes ----------------------------*/
void *getDataThreadHandler( void );
void setFlag( uint8_t );
void close_file( void );

#endif

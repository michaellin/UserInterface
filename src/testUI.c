/****************************************************************************
 Module
   testUI.c

 Description
   Module for simple test of ASCII based UI for recording user input. This Demo should
 Display a sliding bar in ASCII and change with respect to the user input of left or right
 arrows.

 Notes

 History
 When           Who     What/Why
 -------------- ---     --------
 01/13/16 10:53 mal     Begin coding
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include "conio.h"

//#define TEST
#define MAX_CUR_POS         11 //Maximum cursor position
#define CURSOR_ASCII_VAL    124 //ASCII for a block

/*---------------------------- Module Functions ---------------------------*/
/* prototypes for private functions for this module. */
static void print_slidebar( uint8_t );

#ifdef TEST
int main( void )
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
        }
    } 

}

#endif
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

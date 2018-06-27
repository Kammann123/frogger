#include "gui_types.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*************************/
/* Definiciones publicas */
/*************************/

/* map_position */
POSITION map_position(int32_t x, int32_t y){
    POSITION pos = {
        .x = x,
        .y = y
    };

    return pos;
}

/* testing_msg */
void testing_msg(char *str){
#ifdef TESTING
    printf("[MONITOR] %s\n", str);
#endif
}

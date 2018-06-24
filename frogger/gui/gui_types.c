#include "gui_types.h"

#include <stdlib.h>

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
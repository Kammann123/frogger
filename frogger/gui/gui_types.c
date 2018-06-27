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

/***********************/
/* GAME_STAGE handlers */
/***********************/

/* stage_init */
GAME_STAGE stage_init(void){
    GAME_STAGE stage = {
        .value = DEFAULT_GAME_STAGE,
        .hasChanged = false
    };
    
    return stage;
}

/* change_stage */
void change_stage(GAME_STAGE* stage, STAGE_VALUES value){
    
    /* Cambio el valor del estado */
    stage->value = value;
    stage->hasChanged = true;
}

/* is_stage */
bool is_stage(GAME_STAGE* stage, STAGE_VALUES value){
    
    if( stage->value == value ){
        return true;
    }
    return false;
}
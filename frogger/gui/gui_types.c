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

/* save_score */
void save_score(GAME_STAGE* stage, uint32_t score){
    /* Agrego el puntaje */
    add_score(stage->username, score, stage->topScore);
}

/* get_score_position */
bool get_score_position(GAME_STAGE* stage, char* str, uint32_t pos){
    if( pos < stage->topLength ){
        /* Guardo en formato el resultado */
        sprintf(str, "%d. %s %d", pos, stage->topScore[pos].name, stage->topScore[pos].score);
        return true;
    }else{
        /* Pide una posicion que no hay */
        return false;
    }
}

/* stage_init */
GAME_STAGE stage_init(char* username){
    /* Inicializacion de instancia */
    GAME_STAGE stage = {
        .value = DEFAULT_GAME_STAGE,
        .hasChanged = false
    };
    
    /* Cargo el top score */
    stage.topLength = get_score_array(stage.topScore, SCORE_FILE);

    /* Cargo nombre del usuario */
    if( username == NULL ){
        strcpy(stage.username, DEFAULT_USERNAME);
    }else{
        strcpy(stage.username, username);
    }

    /* Devuelvo */
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

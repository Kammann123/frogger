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

/* load_score */
void load_score(GAME_STAGE* stage){
    LENGTH i = 0;
    
    /* Cargo el top score */
    get_score_array(stage->topScore, SCORE_FILE);
    
    /* Calculo cantidad de elementos */
    while( stage->topScore[i++].name[0] );
    stage->topLength = i-1;
}

/* save_score */
void save_score(GAME_STAGE* stage, uint32_t score){
    /* Agrego el puntaje */
    add_score(stage->username, score, stage->topScore);
    
    /* Vuelvo a cargar */
    load_score(stage);
}

/* get_score_position */
bool get_score_position(GAME_STAGE* stage, char* str, uint32_t pos){
    if( pos < stage->topLength ){
        /* Guardo en formato el resultado */
        sprintf(str, "%d. %s %d", pos + 1, stage->topScore[pos].name, stage->topScore[pos].score);
        return true;
    }else{
        /* Pide una posicion que no hay */
        return false;
    }
}

/* stage_init */
GAME_STAGE stage_init(char* username, uint16_t characterId, uint16_t backgroundId, uint16_t vehiclesId){
    
    /* Inicializacion de instancia */
    GAME_STAGE stage = {
        .value = DEFAULT_GAME_STAGE,
        .hasChanged = false,
        .characterId = characterId,
        .backgroundId = backgroundId,
        .vehiclesId = vehiclesId
        
    };
    
    /* Cargo el score */
    load_score(&stage);
    
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

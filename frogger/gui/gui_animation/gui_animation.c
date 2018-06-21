#include "gui_animation.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/***********************************/
/* Prototipo de funciones privadas */
/***********************************/

/************************************/
/* Definicion de funciones privadas */
/************************************/

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* gui_animation_create_framelist */
FRAME* gui_animation_create_framelist(uint16_t frameQty){
    FRAME* framelist;
    uint16_t i;
    
    /* Reservo memoria */
    framelist = malloc( sizeof(FRAME) * frameQty );
    
    /* Verifico que haya podido */
    if( framelist == NULL ){
        return NULL;
    }
    
    /* Inicializo su contenido */
    for(i = 0;i < frameQty;i++){
        framelist[i] = NULL;
    }
    
    return framelist;
}

/* gui_animation_destroy_framelist */
void gui_animation_destroy_framelist(FRAME* framelist, uint16_t framesQty){
    uint16_t i;
    
    /* Libero memoria de los frames de la lista */
    for(i = 0;i < framesQty;i++){
        if( framelist[i] != NULL ){
            gui_animation_destroy_frame(framelist[i]);
        }
    }
    
    /* Libero la memoria reservada */
    free(framelist);
}

/* gui_animation_create_frame */
FRAME gui_animation_create_frame(char* str){
    FRAME frame;
    
    /* Asigno memoria */
    frame = malloc( sizeof(char) * strlen(str) );
    if( frame == NULL ){
        return NULL;
    }
    
    /* Copio el contenido */
    strcpy(frame, str);
    
    /* Devuelvo resultado */
    return frame;
}

/* gui_animation_destroy_frame */
void gui_animation_destroy_frame(FRAME frame){
    /* Libero memoria */
    free(frame);
}
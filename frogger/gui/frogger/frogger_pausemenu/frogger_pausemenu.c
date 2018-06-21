#include "frogger_pausemenu.h"

#include <stdlib.h>

/**************************/
/* Objetos de la libreria */
/**************************/

/* Mensajes en memoria */
const char* PAUSEMENU_OPTIONS[] = {
    RESUME_MSG,
    RESTART_MSG,
    EXIT_MSG,
    NULL
};

/* Opcion seleccionada */
static uint16_t optionSelected = DEFAULT_SELECTION;

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* frogger_pausemenu_close */
void frogger_pausemenu_close(void){
#if PLATFORM_MODE == PC_ALLEGRO
    
#elif PLATFORM_MODE == RPI
    
#endif    
}

/* frogger_pausemenu_update */
void frogger_pausemenu_update(void){
#if PLATFORM_MODE == PC_ALLEGRO
    
#elif PLATFORM_MODE == RPI
    
#endif
}

/* frogger_pausemenu_move */
void frogger_pausemenu_move(uint16_t input){
    switch(input){
        case MOVE_UP:
            if( optionSelected != RESUME_OPTION ){
                optionSelected--;
            }
            break;
        case MOVE_DOWN:
            if( optionSelected != EXIT_OPTION ){
                optionSelected++;
            }
            break;
    }
}

/* frogger_pausemenu_selected */
uint16_t frogger_pausemenu_selected(void){
    return optionSelected;
}
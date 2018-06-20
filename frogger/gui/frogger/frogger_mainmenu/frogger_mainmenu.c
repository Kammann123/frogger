#include "frogger_mainmenu.h"

#include <stdlib.h>

/**************************/
/* Objetos de la libreria */
/**************************/

/* Mensajes en memoria */
const char* MAINMENU_OPTIONS[] = {
    PLAY_MSG,
    RANK_MSG,
    HOWTO_MSG,
    EXIT_MSG,
    NULL
};

/* Opcion seleccionada */
static uint16_t optionSelected = DEFAULT_SELECTION;

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* frogger_mainmenu_close */
void frogger_mainmenu_close(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_mainmenu_close();
#elif PLATFORM_MODE == RPI
    
#endif    
}

/* frogger_mainmenu_update */
void frogger_mainmenu_update(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_mainmenu(NULL, optionSelected);
#elif PLATFORM_MODE == RPI
    
#endif
}

/* frogger_mainmenu_move */
void frogger_mainmenu_move(uint16_t input){
    switch(input){
        case MOVE_UP:
            if( optionSelected != PLAY_OPTION ){
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

/* frogger_mainmenu_selected */
uint16_t frogger_mainmenu_selected(void){
    return optionSelected;
}
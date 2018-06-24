#include "frogger_kernel.h"

#include "../../gui/frogger/frogger_game/frogger_game.h"

/**************************/
/* Objetos de la libreria */
/**************************/

/* Datos de la partida */
FROGGER_GAME_DATA froggerGame = {
    .level = DEFAULT_LEVEL,
    .lifes = DEFAULT_LIFES,
    .time = DEFAULT_TIME,
    .score = DEFAULT_SCORE
};

/***************************/
/* Definicion de funciones */
/***************************/

/* frogger_flow */
void frogger_flow(void){
    
    /* Controlo que se muevan los carriles */
    frogger_game_move_lanes();
    
}
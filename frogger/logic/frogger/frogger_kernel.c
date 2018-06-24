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
    
    /* Me fijo que no haya colisiones */
    if( frogger_game_collisions() ){
        /* Le quito una vida */
        froggerGame.lifes--;
        
        /* Me fijo que tenga vidas aun */
        if( froggerGame.lifes ){
            frogger_game_reset_frog_position();
        }else{
            
        }
        
    }
}
#include "frogger_kernel.h"

#include "../../gui/frogger/frogger_game/frogger_game.h"
#include "../../gui/gui_animation/gui_animation.h"

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
    
    /* Si esta en tierra */
    if( frogger_game_is_street_region() ){
    
        /* Me fijo que no haya colisiones */
        if( frogger_game_is_frog_static() ){   
            if( frogger_game_collisions() ){
                /* Le quito una vida */
                froggerGame.lifes--;

                /* Si hubo colision */
                frogger_reset();
            }
        }
    }else if( frogger_game_is_water_region() ){
        
        /* Me fijo si no se subio a transporte */
        frogger_game_is_transport();
        
        /* Me fijo si no se cayo al agua */
        if( frogger_game_drown() ){
            /* Le quito una vida */
            froggerGame.lifes--;
            
            /* Si hubo colision */
            frogger_reset();
        }else{
            /* Manejo el transporte */
            frogger_game_transport_frog();
        }
    }
}

/* frogger_reset */
void frogger_reset(void){
        
    /* Me fijo que tenga vidas aun */
    if( froggerGame.lifes ){
        frogger_game_reset_frog_position();
    }
}
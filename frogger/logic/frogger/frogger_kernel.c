#include "frogger_kernel.h"

#include "../../gui/frogger/frogger_game/frogger_game.h"
#include "../../gui/gui_animation/gui_animation.h"
#include <math.h>

/**************************/
/* Objetos de la libreria */
/**************************/

/* Datos de la partida */
FROGGER_GAME_DATA froggerGame = {
    .stage = DEFAULT_STAGE,
    .level = DEFAULT_LEVEL,
    .lifes = DEFAULT_LIFES,
    .time = DEFAULT_TIME,
    .score = DEFAULT_SCORE
};

/***************************/
/* Definicion de funciones */
/***************************/

/* frogger_calculate_score */
uint32_t frogger_calculate_score(uint32_t level, uint32_t stage, uint32_t time){
    uint32_t score, timePlus;
    
    /* Calculo el puntaje por el nivel y el stage */
    score = LEVEL_SCORE_BASE * level * (stage + 1);
    
    /* Calculo el plus por tiempo */
    timePlus = (TIME_SCORE_BASE + (double)level / 10) * (TIME_MAX_VALUE - time);
    
    /* score final */
    score += timePlus;
    
    return score;
}

/* frogger_flow */
uint32_t frogger_flow(void){
    
    /* Me fijo si perdio */
    if( !froggerGame.lifes ){
        return FROGGER_HAS_LOST;
    }
    
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
                return FROGGER_HAS_DIED;
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
            return FROGGER_HAS_DROWN;
        }else{
            /* Manejo el transporte */
            frogger_game_transport_frog();
        }
    }
    
    /* Limite de tiempo */
    if( froggerGame.time > TIME_MAX_VALUE ){
        return FROGGER_HAS_LOST;
    }
    
    /* Me fijo si gano */
    if( frogger_game_has_won() ){
        /* Calculo y agrego puntaje */
        froggerGame.score += frogger_calculate_score(froggerGame.level, froggerGame.stage, froggerGame.time);

        /* Incremento stage */
        froggerGame.stage++;

        /* Reinicio timer */
        froggerGame.time = 0;

        /* Me fijo si termino el nivel */
        if( froggerGame.stage == STAGE_MAX_VALUE ){
            /* Pauso el juego */
            frogger_game_pause();

            /* Subo el nivel */
            if( !frogger_level_up() ){
                return FROGGER_ERROR;
            }

            /* Continuo el juego */
            frogger_game_continue();
        }

        /* Reseteo posicion */
        frogger_reset();

        return FROGGER_HAS_WON;
    }
    
    return FROGGER_NOTHING;
}

/* frogger_level_up */
bool frogger_level_up(void){
    /* New values */
    froggerGame.level++;
    froggerGame.stage = DEFAULT_STAGE;
    froggerGame.lifes = DEFAULT_LIFES;
    return frogger_game_new_level( froggerGame.level );
}

/* frogger_reset */
void frogger_reset(void){
        
    /* Me fijo que tenga vidas aun */
    if( froggerGame.lifes ){
        while( !frogger_game_is_frog_static() );
        frogger_game_reset_frog_position();
    }
}

/* frogger_time_count */
void frogger_time_count(void){
    froggerGame.time++;
}

/* frogger_restart */
void frogger_restart(void){
    froggerGame.level = DEFAULT_LEVEL;
    froggerGame.stage = DEFAULT_STAGE;
    froggerGame.lifes = DEFAULT_LIFES;
    froggerGame.score = DEFAULT_SCORE;
    froggerGame.time = DEFAULT_TIME;
}

/* frogger_get_score */
uint32_t frogger_get_score(void){
    return froggerGame.score;
}

/* frogger_get_lifes */
uint32_t frogger_get_lifes(void){
    return froggerGame.lifes;
}

/* frogger_get_level */
uint32_t frogger_get_level(void){
    return froggerGame.level;
}

/* frogger_get_stage */
uint32_t frogger_get_stage(void){
    return froggerGame.stage;
}
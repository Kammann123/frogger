/* RPI_PHYSICS
*/

#include "../frogger_physics.h"
#include "../../gui_input.h"

/**********************/
/* Funciones publicas */
/**********************/

/* frogger_game_get_step */
uint32_t frogger_game_get_step(void){
    return 0;
}

/* frogger_game_get_resolution */
uint32_t frogger_game_get_resolution(void){
    return 0;
}

/* frogger_game_frog_init */
bool frogger_game_frog_init(FROG* frog){
    return false;
}

/* frogger_game_movement_valid */
bool frogger_game_movement_valid(FROG frog, uint16_t input){
    return false;
}

/* frogger_game_create_object */
ANIMATED_OBJECT* frogger_game_create_object(POSITION pos, SPEED speed, GUI_ANIMATION_ORIENTATION orientation, uint32_t type){
    return NULL;
}

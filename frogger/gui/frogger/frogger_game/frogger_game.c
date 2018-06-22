#include "frogger_game.h"

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* frogger_game_screen_update */
void frogger_game_screen_update(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_screen_update();
#elif PLATFORM_MODE == RPI
#endif
}

/* frogger_game_screen_close */
void frogger_game_screen_close(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_screen_close();
#elif PLATFORM_MODE == RPI
#endif
}
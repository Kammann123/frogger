#include "frogger_game.h"

/*********************/
/* Objetos del juego */
/*********************/

/* Frog del jugador */
FROG frog = { .object=NULL };

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* frogger_game_continue */
void frogger_game_continue(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_continue();
#elif PLATFORM_MODE == RPI
#endif    
}

/* frogger_game_pause */
void frogger_game_pause(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_pause();
#elif PLATFORM_MODE == RPI
#endif    
}

/* frogger_game_close */
void frogger_game_close(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_close();
#elif PLATFORM_MODE == RPI
#endif    
}
/* frogger_game_init */
bool frogger_game_init(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_init();
#elif PLATFORM_MODE == RPI
#endif    
}

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
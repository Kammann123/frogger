#include "frogger_lostscreen.h"
#include "../../../settings.h"
#include "../../allegro/frogger/lostscreen/allegro_frogger_lostscreen.h"

/****************/
/* Definiciones */
/****************/

/* frogger_lostscreen_update */
void frogger_lostscreen_update(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_lostscreen();
#elif PLATFORM_MODE == RPI
#endif    
}

/* frogger_lostscreen_close */
void frogger_lostscreen_close(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_lostscreen_close();
#elif PLATFORM_MODE == RPI
#endif    
}
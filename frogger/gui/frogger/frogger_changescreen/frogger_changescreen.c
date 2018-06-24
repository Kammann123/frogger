#include "frogger_changescreen.h"
#include "../../../settings.h"
#include "../../allegro/frogger/changescreen/allegro_frogger_changescreen.h"

/****************/
/* Definiciones */
/****************/

/* frogger_changescreen_update */
void frogger_changescreen_update(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_changescreen();
#elif PLATFORM_MODE == RPI
#endif
}

/* frogger_changescreen_close */
void frogger_changescreen_close(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_changescreen_close();
#elif PLATFORM_MODE == RPI
#endif
}
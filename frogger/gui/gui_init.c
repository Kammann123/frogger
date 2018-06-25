#include "gui_init.h"

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* gui_init */
bool gui_init(void){
#if PLATFORM_MODE == PC_ALLEGRO
    return allegro_wrapped_init();
#elif PLATFORM_MODE == RPI
#endif
}
#include "gui_init.h"

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* gui_init */
bool gui_init(void){
#if PLATFORM_MODE == PC_ALLEGRO
    return allegro_wrapped_init();
#elif PLATFORM_MODE == RPI
<<<<<<< HEAD
=======
	return rpi_init();
    
>>>>>>> f5d5fc0fb1a57cb6e21ff043148672adea3fb906
#endif
}
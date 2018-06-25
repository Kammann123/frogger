#include "gui_input.h"

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* gui_input_init */
bool gui_input_init(void){
#if PLATFORM_MODE == PC_ALLEGRO
    return allegro_input_init();
#elif PLATFORM_MODE == RPI
<<<<<<< HEAD
=======
    return joystick_input_init();
>>>>>>> f5d5fc0fb1a57cb6e21ff043148672adea3fb906
#endif
}

/* gui_input_close */
void gui_input_close(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_input_close();
#elif PLATFORM_MODE == RPI
    
#endif
}

/* gui_input_source */
bool gui_input_source(EVENT* event, void* none){
#if PLATFORM_MODE == PC_ALLEGRO
    return allegro_input_source(event, none);
#elif PLATFORM_MODE == RPI
<<<<<<< HEAD
=======
	return joystick_state(event, none);
    
>>>>>>> f5d5fc0fb1a57cb6e21ff043148672adea3fb906
#endif    
}
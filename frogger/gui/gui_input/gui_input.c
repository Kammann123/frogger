#include "gui_input.h"

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* gui_input_init */
bool gui_input_init(void){
#if PLATFORM_MODE == PC_ALLEGRO
    return allegro_input_init();
#elif PLATFORM_MODE == RPI
    
#endif
}

/* gui_input_close */
void gui_input_close(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_input_close();
#elif PLATFORM_MODE == RPI
    
#endif
}

/* gui_input_event */
bool gui_input_event(EVENT* event, void* none){
#if PLATFORM_MODE == PC_ALLEGRO
    return allegro_input_source(event, none);
#elif PLATFORM_MODE == RPI
    
#endif    
}
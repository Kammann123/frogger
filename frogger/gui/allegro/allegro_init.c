#include "allegro_init.h"

#include <allegro5/allegro_image.h>

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* allegro_init */
bool allegro_wrapped_init(void){
    
    /* Inicializo el modulo general */
    if( !al_init() ){
        return false;
    }
    
    if( !al_init_image_addon() ){
        return false;
    }
    
    /* Instalo el teclado */
    if( !al_install_keyboard() ){
        return false;
    }
    
    /* Inicializacion exitosa */
    return true;
}
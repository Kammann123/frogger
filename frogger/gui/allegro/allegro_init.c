#include "allegro_init.h"

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>

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
    
    al_init_font_addon();
    
    if( !al_init_ttf_addon() ){
        return false;
    }
    
    /* Instalo el teclado */
    if( !al_install_keyboard() ){
        return false;
    }
    
    /* Inicializacion exitosa */
    return true;
}
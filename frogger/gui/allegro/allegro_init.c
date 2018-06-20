#include "allegro_init.h"

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* allegro_init */
bool allegro_init(void){
    
    /* Inicializo el modulo general */
    if( !al_init() ){
        return false;
    }
    
    /* Instalo el teclado */
    if( !al_install_keyboard() ){
        return false;
    }
    
    /* Inicializacion exitosa */
    return true;
}
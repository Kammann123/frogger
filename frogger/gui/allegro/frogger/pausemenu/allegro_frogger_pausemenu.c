#include "allegro_frogger_pausemenu.h"

#include <allegro5/allegro5.h>

/**************************/
/* Objetos de la libreria */
/**************************/
static ALLEGRO_DISPLAY* display = NULL;

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* allegro_frogger_pausemenu_close  */
void allegro_frogger_pausemenu_close(void){
    /* Cierro el display */
    al_destroy_display(display);
    display = NULL;
}

/* allegro_frogger_pausemenu */
void allegro_frogger_pausemenu(const char** options, uint16_t optionSelected){
    ALLEGRO_BITMAP* bitmap = NULL;
    
    /* Inicializo el display */
    if( display == NULL ){
        display = al_create_display(PAUSEMENU_WIDTH, PAUSEMENU_HEIGHT);
        if( display == NULL ){
            return;
        }
    }
    
    /* Cargo el bitmap */
    switch( optionSelected ){
        case 0:
            bitmap = al_load_bitmap(PAUSEMENU_OPTION_0);
            break;
        case 1:
            bitmap = al_load_bitmap(PAUSEMENU_OPTION_1);
            break;
        case 2:
            bitmap = al_load_bitmap(PAUSEMENU_OPTION_2);
            break;
    }
    
    /* Verifico que no hay problema con la carga */
    if( bitmap == NULL ){
        return;
    }
    
    /* Pongo el fondo */
    al_draw_bitmap(bitmap, 0, 0, 0);
    al_flip_display();
}
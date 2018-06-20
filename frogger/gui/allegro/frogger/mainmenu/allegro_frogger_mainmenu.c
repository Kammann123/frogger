#include "allegro_frogger_mainmenu.h"

#include <allegro5/allegro5.h>

/**************************/
/* Objetos de la libreria */
/**************************/
static ALLEGRO_DISPLAY* display = NULL;

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* allegro_frogger_mainmenu_update */
void allegro_frogger_mainmenu_close(void){
    /* Cierro el display */
    al_destroy_display(display);
    display = NULL;
}

/* allegro_frogger_mainmenu */
void allegro_frogger_mainmenu(char** options, uint16_t optionSelected){
    ALLEGRO_BITMAP* bitmap = NULL;
    
    /* Inicializo el display */
    if( display == NULL ){
        display = al_create_display(MAINMENU_WIDTH, MAINMENU_HEIGHT);
        if( display == NULL ){
            return;
        }
    }
    
    /* Cargo el bitmap */
    switch( optionSelected ){
        case 0:
            bitmap = al_load_bitmap(MAINMENU_OPTION_0);
            break;
        case 1:
            bitmap = al_load_bitmap(MAINMENU_OPTION_1);
            break;
        case 2:
            bitmap = al_load_bitmap(MAINMENU_OPTION_2);
            break;
        case 3:
            bitmap = al_load_bitmap(MAINMENU_OPTION_3);
            break;
    }
    
    if( bitmap == NULL ){
        return;
    }
    
    /* Pongo el fondo */
    al_clear_to_color( al_map_rgb(255, 255, 255) );
    al_draw_bitmap(bitmap, 0, 0, 0);
    al_flip_display();
}
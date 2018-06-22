#include "allegro_frogger_game.h"
#include "../../../frogger/frogger_game/frogger_game.h"

#include <allegro5/allegro.h>

/**************************/
/* Objetos de la libreria */
/**************************/

/* Display del juego */
static ALLEGRO_DISPLAY* display = NULL;

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* allegro_frogger_screen_update */
void allegro_frogger_screen_update(void){
    ALLEGRO_BITMAP* fieldBitmap;
    
    /* Abro la ventana */
    if( display == NULL ){
        display = al_create_display(ALLEGRO_FROGGER_GAME_WIDTH, ALLEGRO_FROGGER_GAME_HEIGHT);
        if( display == NULL ){
            return;
        }
    }
    
    /* Limpio la ventana */
    al_clear_to_color( CLEAR_COLOR );
    
    /* Pongo el campo de juego */
    fieldBitmap = al_load_bitmap(FROGGER_IMAGE_FIELD);
    if( fieldBitmap == NULL ){
        return;
    }
    al_draw_bitmap(fieldBitmap, 0, 0, 0);    
    
    /* Mando el buffer al display */
    al_flip_display();
}

/* allegro_frogger_screen_close */
void allegro_frogger_screen_close(void){
    if( display != NULL ){
        al_destroy_display(display);
        display = NULL;
    }
}

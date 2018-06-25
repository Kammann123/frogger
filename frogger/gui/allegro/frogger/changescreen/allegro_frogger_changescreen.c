#include "allegro_frogger_changescreen.h"
#include "../../../../logic/frogger/frogger_kernel.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>

#include <stdio.h>

/**************************/
/* Objetos de la libreria */
/**************************/

/* Creo la instancia de display */
static ALLEGRO_DISPLAY* display = NULL;

/****************/
/* Definiciones */
/****************/

/* allegro_frogger_changescreen */
void allegro_frogger_changescreen(void){
    ALLEGRO_BITMAP* bitmap;
    ALLEGRO_FONT* font;
    char text[10];
    
    /* Abro la ventana */
    if( display == NULL ){
        display = al_create_display(ALLEGRO_CHANGESCREEN_WIDTH, ALLEGRO_CHANGESCREEN_HEIGHT);
        if( display == NULL ){
            return;
        }
        al_set_window_position(display, 300, 100);
    }

    /* Limpio el fondo */
    al_clear_to_color( CLEAR_COLOR );
    
    /* Cargo el bitmap */
    bitmap = al_load_bitmap( ALLEGRO_FROGGER_CHANGESCREEN );
    if( bitmap == NULL ){
        return;
    }
    al_draw_bitmap(bitmap, 0, 0, 0);
    
    /* Fuente */
    font = al_load_font(ALLEGRO_FROGGER_CHANGE_FONT, ALLEGRO_FROGGER_CHANGE_SIZE, 0);
    if( font == NULL ){
        return;
    }
    
    /* Nivel */
    sprintf(text, "%d", frogger_get_level());
    al_draw_text(font, LEVEL_COLOR, LEVEL_X, LEVEL_Y, 0, text);
    
    /* Stage */
    sprintf(text, "%d", frogger_get_stage());
    al_draw_text(font, STAGE_COLOR, STAGE_X, STAGE_Y, 0, text);
    
    /* Mando el buffer al display */
    al_flip_display();
}

/* allegro_frogger_changescreen_close */
void allegro_frogger_changescreen_close(void){
    
    /* Elimino el display */
    if( display != NULL ){
        
        al_destroy_display(display);
        display = NULL;
    }
}
#include "allegro_frogger_lostscreen.h"
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

/* allegro_frogger_lostscreen */
void allegro_frogger_lostscreen(void){
    ALLEGRO_BITMAP* bitmap;
    ALLEGRO_FONT* font;
    char score[10];
    
    /* Abro la ventana */
    if( display == NULL ){
        display = al_create_display(ALLEGRO_LOSTSCREEN_WIDTH, ALLEGRO_LOSTSCREEN_HEIGHT);
        if( display == NULL ){
            return;
        }
    }

    /* Limpio el fondo */
    al_clear_to_color( CLEAR_COLOR );
    
    /* Cargo el bitmap */
    bitmap = al_load_bitmap( ALLEGRO_FROGGER_LOSTSCREEN );
    if( bitmap == NULL ){
        return;
    }
    al_draw_bitmap(bitmap, 0, 0, 0);
    
    /* Puntuacion */
    font = al_load_ttf_font(ALLEGRO_FROGGER_SCORE_FONT, ALLEGRO_FROGGER_SCORE_SIZE, 0);
    if( font == NULL ){
        return;
    }
    sprintf(score, "%d", frogger_get_score());
    al_draw_text(font, SCORE_COLOR, SCORE_X, SCORE_Y, 0, score);
    
    /* Mando el buffer al display */
    al_flip_display();
}

/* allegro_frogger_lostscreen_close */
void allegro_frogger_lostscreen_close(void){
    
    /* Elimino el display */
    if( display != NULL ){
        
        al_destroy_display(display);
        display = NULL;
    }
}

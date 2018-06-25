
#include "allegro_frogger_rankmenu.h"
#include <allegro5/allegro5.h>

/**************************/
/* Objetos de la libreria */
/**************************/
static ALLEGRO_DISPLAY* display = NULL;

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* allegro_frogger_pausemenu_close  */
void allegro_frogger_rankmenu_close(void){
    /* Cierro el display */
    al_destroy_display(display);
    display = NULL;
}

/* allegro_frogger_pausemenu */
void allegro_frogger_rankmenu(const char** options){
    ALLEGRO_BITMAP* bitmap = NULL;
    ALLEGRO_BITMAP* bitmap2 = NULL;
    uint8_t lastpos, i;
    uint8_t yBase = Y_RANK_BASE;
    
    /* Inicializo el display */
    if( display == NULL ){
        display = al_create_display(RANKMENU_WIDTH, RANKMENU_HEIGHT);
        if( display == NULL ){
            return;
        }
    }
    
    bitmap = al_load_bitmap(RANKMENU_BACKGROUND);
    bitmap2 = al_load_bitmap(FROG_GIF);
    
    /* FALTA UNIR EL ARCHIVO SCORE_BOARD
    scores_t * scoreArray =  (scores_t * ) calloc (MAX_SCORES+1, sizeof(scores_t));
    get_score_array(scoreArray, SCORE_FILE);
    lastpos = get_last_pos (scoreArray);
     
    
     */

    /* Verifico que no hay problema con la carga */
    if( bitmap == NULL || bitmap2 == NULL){
        return;
    }
    
    /* Pongo el fondo */
    al_draw_bitmap(bitmap, 0, 0, 0);
    al_draw_bitmap(bitmap2, 20, 20, 0);
    
    
    /* Pongo el ranking*/
    for (i=0; i<lastpos; i++){
        /* FALTA UNIR SCORE_BOARD Y WINDOW_MANAGER
         add_label(WINDOW *window, char *idName, scoreBoard[i].name, X_RANK, yBase, ALLEGRO_COLOR colorFont, uint8_t fontSize, char *fontFile);
         add_label(WINDOW *window, char *idName, itoa (scoreBoard[lastPos].score), X_RANK + 30, yBase, ALLEGRO_COLOR colorFont, uint8_t fontSize, char *fontFile);
         yBase += Y_RANK_DIFF;
         
         */
    }
    
    al_flip_display();
}


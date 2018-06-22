#include "allegro_frogger_game.h"
#include "../../../frogger/frogger_game/frogger_game.h"
#include "../../../gui_animation/gui_animation.h"
#include "../../../gui_files/gui_files.h"

#include <allegro5/allegro.h>

/**************************/
/* Objetos de la libreria */
/**************************/

/* Objetos de acceso externo */
extern FROG frog;

/* Display del juego */
static ALLEGRO_DISPLAY* display = NULL;

/* Motor de animaciones */
static ANIMATION_ENGINE* engine = NULL;

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* allegro_frogger_continue */
void allegro_frogger_continue(void){
    gui_animation_continue_engine(engine);
}

/* allegro_frogger_pause */
void allegro_frogger_pause(void){
    gui_animation_pause_engine(engine);
}

/* allegro_frogger_close */
void allegro_frogger_close(void){
    /* Libero el objeto frog */
    if( frog.object != NULL ){
        gui_animation_destroy_object(frog.object);
    }
    
    /* Libero motor de animaciones */
    if( engine != NULL ){
        gui_animation_destroy_engine(engine);
    }
}

/* allegro_frogger_init */
bool allegro_frogger_init(void){
    
    /* Creo el objeto frog */
    frog.object = gui_animation_create_object(DEFAULT_FROG_X * ALLEGRO_DISPLAY_STEP, DEFAULT_FROG_Y * ALLEGRO_DISPLAY_STEP, DEFAULT_FROG_ORIENTATION);
    if( frog.object == NULL ){
        return false;
    }
    
    /* Creo el motor de animaciones */
    engine = gui_animation_create_engine();
    if( engine == NULL ){
        gui_animation_destroy_object(frog.object);
        return false;
    }
    
    /* Cargo animaciones de frog */
    if( !gui_files_read_objfile(FROGGER_PATH_FROG_UP_OBJFILE, frog.object) ){
        gui_animation_destroy_object(frog.object);
        gui_animation_destroy_engine(engine);
        return false;
    }
    if( !gui_files_read_objfile(FROGGER_PATH_FROG_DOWN_OBJFILE, frog.object) ){
        gui_animation_destroy_object(frog.object);
        gui_animation_destroy_engine(engine);
        return false;
    }
    if( !gui_files_read_objfile(FROGGER_PATH_FROG_LEFT_OBJFILE, frog.object) ){
        gui_animation_destroy_object(frog.object);
        gui_animation_destroy_engine(engine);
        return false;
    }
    if( !gui_files_read_objfile(FROGGER_PATH_FROG_RIGHT_OBJFILE, frog.object) ){
        gui_animation_destroy_object(frog.object);
        gui_animation_destroy_engine(engine);
        return false;
    }
    
    /* Vinculo objetos con el motor */
    if( !gui_animation_attach_object(engine, frog.object) ){
        return false;
    }
    
    /* Inicio el motor */
    gui_animation_start_engine(engine);
    
    gui_animation_start_loop(frog.object, GUI_ANIMATION_VERTICAL_DOWN);
}

/* allegro_frogger_screen_update */
void allegro_frogger_screen_update(void){
    ALLEGRO_BITMAP* bitmap;
    
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
    bitmap = al_load_bitmap(FROGGER_IMAGE_FIELD);
    if( bitmap == NULL ){
        return;
    }
    al_draw_bitmap(bitmap, 0, 0, 0);
    
    /* Pongo la ranita :) */
    bitmap = al_load_bitmap( gui_animation_get_frame(frog.object) );
    if( bitmap == NULL ){
        return;
    }
    al_draw_bitmap(bitmap, frog.object->currentPos.x, frog.object->currentPos.y, 0);
    
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

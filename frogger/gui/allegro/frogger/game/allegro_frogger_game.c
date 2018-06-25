#include "allegro_frogger_game.h"
#include "../../../gui_animation/gui_animation.h"
#include "../../../gui_files/gui_files.h"
#include "../../../gui_input/gui_input.h"
#include "../../../../logic/frogger/frogger_kernel.h"
#include "../../frogger/game/allegro_frogger_game.h"
#include "../../../frogger/frogger_game/frogger_game.h"

#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

/**************************/
/* Objetos de la libreria */
/**************************/

/* Objetos de acceso externo */
extern FROG frog;
extern FIELD field;
extern FROGGER_GAME_DATA froggerGame;

/* Display del juego */
static ALLEGRO_DISPLAY* display = NULL;

/***********************************/
/* Prototipo de funciones privadas */
/***********************************/

/* map_collision
 * Devuelve true si el objeto frog se va
 * a chocar contra el objeto en (x,y)
 * 
 * input: Orientacion de movimiento
 * x: Posicion x del obstaculo
 * y: Posicion y del obstaculo
 */
static bool map_collision(uint16_t input, int32_t x, int32_t y);

/************************************/
/* Definicion de funciones privadas */
/************************************/

/* map_collision */
static bool map_collision(uint16_t input, int32_t x, int32_t y){
    switch(input){
        case MOVE_UP:
            if( x == frog.object->pos.x ){
                if( y == (frog.object->pos.y - ALLEGRO_DISPLAY_STEP) ){
                    return true;
                }
            }
            break;
        case MOVE_DOWN:
            if( x == frog.object->pos.x ){
                if( y == (frog.object->pos.y + ALLEGRO_DISPLAY_STEP) ){
                    return true;
                }
            }
            break;
        case MOVE_LEFT:
            if( y == frog.object->pos.y ){
                if( x == (frog.object->pos.x - ALLEGRO_DISPLAY_STEP) ){
                    return true;
                }
            }
            break;
        case MOVE_RIGHT:
            if( y == frog.object->pos.y ){
                if( x == (frog.object->pos.x + ALLEGRO_DISPLAY_STEP) ){
                    return true;
                }
            }
            break;
    }
    
    /* No hubo colision */
    return false;
}

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* frog_init */
bool allegro_frogger_frog_init(void){
    
    /* Cargo la ranita :D */
    frog.object = gui_animation_load_object(ALLEGRO_FROG_OBJFILE, map_position(DEFAULT_FROG_X, DEFAULT_FROG_Y), DEFAULT_FROG_ANIMATION);
    if( frog.object == NULL ){
        return false;
    }
    
    /* Frog inicializada correctamente */
    return true;
}

/* allegro_frogger_create_object */
ANIMATED_OBJECT* allegro_frogger_create_object(POSITION pos, SPEED speed, GUI_ANIMATION_ORIENTATION orientation, uint32_t type){
    ANIMATED_OBJECT* object;
    
    /* Cargo el object file */
    switch( type ){
        case FROGGER_MOTORBIKE:
            object = gui_animation_load_object( ALLEGRO_MOTORBIKE_OBJFILE, pos, "" );
            break;
        case FROGGER_CAR:
            object = gui_animation_load_object( ALLEGRO_CAR_OBJFILE, pos, "" );
            break;
        case FROGGER_TRUCK:
            object = gui_animation_load_object( ALLEGRO_TRUCK_OBJFILE, pos, "" );
            break;
        case FROGGER_BOAT:
            object = gui_animation_load_object( ALLEGRO_BOAT_OBJFILE, pos, "" );
            break;
        case FROGGER_YACHT:
            object = gui_animation_load_object( ALLEGRO_YACHT_OBJFILE, pos, "" );
            break;
    }
    
    /* Valido */
    if( object == NULL ){
        return NULL;
    }
    
    /* Cargo la velocidad */
    object->speed = speed;
    
    /* Cargo orientacion */
    gui_animation_set_animation(object, gui_animation_seek_animation(object, orientation));
    
    /* Devuelvo la creacion */
    return object;
}

/* allegro_frogger_movement_valid */
bool allegro_frogger_movement_valid(uint16_t input){
    
    /* Me fijo que no haya colision con obstaculos */
    if( map_collision(input, TRASH_0_X, TRASH_0_Y) ){
        return false;
    }
    if( map_collision(input, TRASH_1_X, TRASH_1_Y) ){
        return false;
    }
    if( map_collision(input, PHONE_X, PHONE_Y) ){
        return false;
    }
    
    /* Me fijo que este en los limites */
    switch(input){
        case MOVE_UP:
            if( frog.object->pos.y == ALLEGRO_DISPLAY_BORDER_UP ){
                return false;
            }
            break;
        case MOVE_DOWN:
            if( frog.object->pos.y == ALLEGRO_DISPLAY_BORDER_DOWN ){
                return false;
            }
            break;
        case MOVE_LEFT:
            if( frog.object->pos.x == ALLEGRO_DISPLAY_BORDER_LEFT ){
                return false;
            }
            break;
        case MOVE_RIGHT:
            if( frog.object->pos.x == ALLEGRO_DISPLAY_BORDER_RIGHT ){
                return false;
            }
            break;
    }
    
    /* No hubo caso invalido */
    return true;
}

/* allegro_frogger_screen_update */
void allegro_frogger_screen_update(void){
    ALLEGRO_FONT* font;
    ALLEGRO_BITMAP* bitmap;
    LANE lane;
    FROGGER_OBJECT object;
    char text[MAX_LINE];
    
    uint32_t i, ii;
    
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
    bitmap = al_load_bitmap(ALLEGRO_IMAGE_FIELD);
    if( bitmap == NULL ){
        return;
    }
    al_draw_bitmap(bitmap, 0, 0, 0);
    
    /* Pongo las vidas */
    bitmap = NULL;
    switch( froggerGame.lifes ){
        case 1:
            bitmap = al_load_bitmap(ALLEGRO_IMAGE_LIFES_1);
            break;
        case 2:
            bitmap = al_load_bitmap(ALLEGRO_IMAGE_LIFES_2);
            break;
        case 3:
            bitmap = al_load_bitmap(ALLEGRO_IMAGE_LIFES_3);
            break;
    }
    if( bitmap != NULL ){
        al_draw_bitmap(bitmap, ALLEGRO_DISPLAY_LIFES_X, ALLEGRO_DISPLAY_LIFES_Y, 0);
    }else{
        return;
    }
    
    /* Pongo la info de score */
    font = al_load_ttf_font(ALLEGRO_FONT_0, ALLEGRO_SIZE_0, 0);
    if( font != NULL ){
         sprintf(text, "%s %d", ALLEGRO_SCORE_MSG, froggerGame.score);
        al_draw_text(font, SCORE_COLOR, ALLEGRO_DISPLAY_SCORE_X, ALLEGRO_DISPLAY_SCORE_Y, 0, text);
    }else{
        return;
    }
    
    /* Pongo la info de time */
    font = al_load_ttf_font(ALLEGRO_FONT_0, ALLEGRO_SIZE_0, 0);
    if( font != NULL ){
        sprintf(text, "%s %d", ALLEGRO_TIME_MSG, froggerGame.time);
        al_draw_text(font, TIME_COLOR, ALLEGRO_DISPLAY_TIME_X, ALLEGRO_DISPLAY_TIME_Y, 0, text);
    }else{
        return;
    }
    
    /* Pongo los objetos */
    for(i = 0;i < field.lanesQty;i++){
        lane = field.lanes[i];
        
        for(ii = 0;ii < lane.objectsQty;ii++){
            object = lane.objects[ii];
            
            if( object->pos.x < ALLEGRO_FROGGER_GAME_WIDTH ){
                bitmap = al_load_bitmap( gui_animation_get_frame(object) );
                if( bitmap == NULL ){
                  return;
                }
                al_draw_bitmap(bitmap, object->pos.x, object->pos.y, 0);  
            }
            
        }
    }
    
    /* Pongo la ranita :) */
    bitmap = al_load_bitmap( gui_animation_get_frame(frog.object) );
    if( bitmap == NULL ){
        return;
    }
    al_draw_bitmap(bitmap, frog.object->pos.x, frog.object->pos.y, 0);
    
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

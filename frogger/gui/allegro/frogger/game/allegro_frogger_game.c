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

/* frog_init
 * Inicializa el contenido del objeto animado de la rana
 *
 * frog: Objeto a inicializar
 */
static bool frog_init(FROG* frog);

/************************************/
/* Definicion de funciones privadas */
/************************************/

/* frog_init */
static bool frog_init(FROG* frog){    
    
    /* Creo el objeto frog */
    frog->object = gui_animation_create_object(DEFAULT_FROG_X * ALLEGRO_DISPLAY_STEP, DEFAULT_FROG_Y * ALLEGRO_DISPLAY_STEP, DEFAULT_FROG_ORIENTATION);
    if( frog->object == NULL ){
        return false;
    }    
    
    if( !gui_animation_load_objfile(FROGGER_FROG_UP_OBJFILE, frog->object) ){
        gui_animation_destroy_object(frog->object);
        return false;
    }
    if( !gui_animation_load_objfile(FROGGER_FROG_DOWN_OBJFILE, frog->object) ){
        gui_animation_destroy_object(frog->object);
        return false;
    }
    if( !gui_animation_load_objfile(FROGGER_FROG_LEFT_OBJFILE, frog->object) ){
        gui_animation_destroy_object(frog->object);
        return false;
    }
    if( !gui_animation_load_objfile(FROGGER_FROG_RIGHT_OBJFILE, frog->object) ){
        gui_animation_destroy_object(frog->object);
        return false;
    }
    
    /* Frog inicializada correctamente */
    return true;
}

/* map_collision */
static bool map_collision(uint16_t input, int32_t x, int32_t y){
    switch(input){
        case MOVE_UP:
            if( x == frog.object->currentPos.x ){
                if( y == (frog.object->currentPos.y - ALLEGRO_DISPLAY_STEP) ){
                    return true;
                }
            }
            break;
        case MOVE_DOWN:
            if( x == frog.object->currentPos.x ){
                if( y == (frog.object->currentPos.y + ALLEGRO_DISPLAY_STEP) ){
                    return true;
                }
            }
            break;
        case MOVE_LEFT:
            if( y == frog.object->currentPos.y ){
                if( x == (frog.object->currentPos.x - ALLEGRO_DISPLAY_STEP) ){
                    return true;
                }
            }
            break;
        case MOVE_RIGHT:
            if( y == frog.object->currentPos.y ){
                if( x == (frog.object->currentPos.x + ALLEGRO_DISPLAY_STEP) ){
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

/* allegro_frogger_create_object */
ANIMATED_OBJECT* allegro_frogger_create_object(POSITION pos, SPEED speed, uint32_t orientation, uint32_t type){
    ANIMATED_OBJECT* object;
    uint32_t i;
    
    /* Creo el objeto */
    object = gui_animation_create_object(pos.x * ALLEGRO_DISPLAY_STEP, pos.y * ALLEGRO_DISPLAY_STEP, orientation);
    if( object == NULL ){
        return NULL;
    }
    
    /* Cargo el object file */
    switch( type ){
        case FROGGER_MOTORBIKE:
            if( !gui_animation_load_objfile(FROGGER_MOTORBIKE_LEFT_OBJFILE, object) ){
                gui_animation_destroy_object( object );
                return NULL;
            }
            if( !gui_animation_load_objfile(FROGGER_MOTORBIKE_RIGHT_OBJFILE, object) ){
                gui_animation_destroy_object( object );
                return NULL;
            }
            break;
        case FROGGER_CAR:
            if( !gui_animation_load_objfile(FROGGER_CAR_LEFT_OBJFILE, object) ){
                gui_animation_destroy_object( object );
                return NULL;
            }
            if( !gui_animation_load_objfile(FROGGER_CAR_RIGHT_OBJFILE, object) ){
                gui_animation_destroy_object( object );
                return NULL;
            }
            break;
        case FROGGER_TRUCK:
            if( !gui_animation_load_objfile(FROGGER_TRUCK_LEFT_OBJFILE, object) ){
                gui_animation_destroy_object( object );
                return NULL;
            }
            if( !gui_animation_load_objfile(FROGGER_TRUCK_RIGHT_OBJFILE, object) ){
                gui_animation_destroy_object( object );
                return NULL;
            }
            break;
        case FROGGER_BOAT:
            if( !gui_animation_load_objfile(FROGGER_BOAT_LEFT_OBJFILE, object) ){
                gui_animation_destroy_object( object );
                return NULL;
            }
            if( !gui_animation_load_objfile(FROGGER_BOAT_RIGHT_OBJFILE, object) ){
                gui_animation_destroy_object( object );
                return NULL;
            }
            break;
        case FROGGER_YACHT:
            if( !gui_animation_load_objfile(FROGGER_YACHT_LEFT_OBJFILE, object) ){
                gui_animation_destroy_object( object );
                return NULL;
            }
            if( !gui_animation_load_objfile(FROGGER_YACHT_RIGHT_OBJFILE, object) ){
                gui_animation_destroy_object( object );
                return NULL;
            }
            break;
    }
    
    /* Le fuerzo algunos parametros */
    object->orientation = orientation;
    
    for(i = 0;i < NUMBER_OF_ORIENTATIONS;i++){
        if( object->animations[i].orientation == orientation ){
            object->animations[i].speed = speed;
            break;
        }
    }
    
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
            if( frog.object->currentPos.y == ALLEGRO_DISPLAY_BORDER_UP ){
                return false;
            }
            break;
        case MOVE_DOWN:
            if( frog.object->currentPos.y == ALLEGRO_DISPLAY_BORDER_DOWN ){
                return false;
            }
            break;
        case MOVE_LEFT:
            if( frog.object->currentPos.x == ALLEGRO_DISPLAY_BORDER_LEFT ){
                return false;
            }
            break;
        case MOVE_RIGHT:
            if( frog.object->currentPos.x == ALLEGRO_DISPLAY_BORDER_RIGHT ){
                return false;
            }
            break;
    }
    
    /* No hubo caso invalido */
    return true;
}

/* allegro_frogger_init */
bool allegro_frogger_init(void){
    /* Inicializo la rana */
    if( !frog_init(&frog) ){
        return false;
    }    
}

/* allegro_frogger_screen_update */
void allegro_frogger_screen_update(void){
    ALLEGRO_FONT* font;
    ALLEGRO_BITMAP* bitmap;
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
    bitmap = al_load_bitmap(FROGGER_IMAGE_FIELD);
    if( bitmap == NULL ){
        return;
    }
    al_draw_bitmap(bitmap, 0, 0, 0);
    
    /* Pongo las vidas */
    bitmap = NULL;
    switch( froggerGame.lifes ){
        case 1:
            bitmap = al_load_bitmap(FROGGER_IMAGE_LIFES_1);
            break;
        case 2:
            bitmap = al_load_bitmap(FROGGER_IMAGE_LIFES_2);
            break;
        case 3:
            bitmap = al_load_bitmap(FROGGER_IMAGE_LIFES_3);
            break;
    }
    if( bitmap != NULL ){
        al_draw_bitmap(bitmap, ALLEGRO_DISPLAY_LIFES_X, ALLEGRO_DISPLAY_LIFES_Y, 0);
    }else{
        return;
    }
    
    /* Pongo la info de score */
    font = al_load_ttf_font(FROGGER_FONT_0, FROGGER_SIZE_0, 0);
    if( font != NULL ){
         sprintf(text, "%s %d", ALLEGRO_SCORE_MSG, froggerGame.score);
        al_draw_text(font, SCORE_COLOR, ALLEGRO_DISPLAY_SCORE_X, ALLEGRO_DISPLAY_SCORE_Y, 0, text);
    }else{
        return;
    }
    
    /* Pongo la info de time */
    font = al_load_ttf_font(FROGGER_FONT_0, FROGGER_SIZE_0, 0);
    if( font != NULL ){
         sprintf(text, "%s %d", ALLEGRO_TIME_MSG, froggerGame.time);
        al_draw_text(font, TIME_COLOR, ALLEGRO_DISPLAY_TIME_X, ALLEGRO_DISPLAY_TIME_Y, 0, text);
    }else{
        return;
    }
    
    /* Pongo los objetos */
    for(i = 0;i < field.lanesQty;i++){
        for(ii = 0;ii < field.lanes[i].objectsQty;ii++){
            bitmap = al_load_bitmap( gui_animation_get_frame(field.lanes[i].objects[ii]) );
            if( bitmap == NULL ){
                return;
            }
            al_draw_bitmap(bitmap, field.lanes[i].objects[ii]->currentPos.x, field.lanes[i].objects[ii]->currentPos.y, 0);
        }
    }
    
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

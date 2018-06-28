/* ALLEGRO_PHYSICS
 */

#include "../../gui_input.h"
#include "../frogger_physics.h"

/* Configuracion */
#define ALLEGRO_DISPLAY_BORDER_UP       0
#define ALLEGRO_DISPLAY_BORDER_DOWN     400
#define ALLEGRO_DISPLAY_BORDER_LEFT     0
#define ALLEGRO_DISPLAY_BORDER_RIGHT    600

#define ALLEGRO_DISPLAY_STEP            40
#define ALLEGRO_STEP_RESOLUTION         10

/* Obstaculos del juego */
#define TRASH_0_X 0
#define TRASH_0_Y 400
#define TRASH_1_X 40
#define TRASH_1_Y 400
#define PHONE_X 600
#define PHONE_Y 400

/* Paths para acceder a animaciones */
#define ALLEGRO_PATH_OBJECTS                "gui/frogger/allegro/objects/"

#define ALLEGRO_FROG_OBJFILE                ALLEGRO_PATH_OBJECTS "frog/frog"
#define ALLEGRO_MOTORBIKE_OBJFILE           ALLEGRO_PATH_OBJECTS "motorbike/motorbike"
#define ALLEGRO_CAR_OBJFILE                 ALLEGRO_PATH_OBJECTS "car/car"
#define ALLEGRO_TRUCK_OBJFILE               ALLEGRO_PATH_OBJECTS "truck/truck"
#define ALLEGRO_BOAT_OBJFILE                ALLEGRO_PATH_OBJECTS "boat/boat"
#define ALLEGRO_YACHT_OBJFILE               ALLEGRO_PATH_OBJECTS "yacht/yacht"

/* map_collision */
static bool map_collision(uint16_t input, int32_t x, int32_t y){
    FROG frog;

    /* Obtengo el objeto */
    frog = frogger_get_frog();

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
        default:
            break;
    }

    /* No hubo colision */
    return false;
}


/**********************/
/* Funciones publicas */
/**********************/

/* frogger_game_get_step */
uint32_t frogger_game_get_step(void){
    return ALLEGRO_DISPLAY_STEP;
}

/* frogger_game_get_resolution de tablero
 */
uint32_t frogger_game_get_resolution(void){
    return ALLEGRO_STEP_RESOLUTION;
}

/* frogger_game_frog_init */
bool frogger_game_frog_init(FROG* frog){
    /* Cargo la ranita :D */
    frog->object = gui_animation_load_object(ALLEGRO_FROG_OBJFILE, map_position(DEFAULT_FROG_X * ALLEGRO_DISPLAY_STEP, DEFAULT_FROG_Y * ALLEGRO_DISPLAY_STEP), DEFAULT_FROG_ANIMATION);
    if( frog->object == NULL ){
        return false;
    }

    /* Frog inicializada correctamente */
    return true;
}

/* frogger_game_movement_valid */
bool frogger_game_movement_valid(FROG frog, INPUT_VALUES input){
    int32_t step = frogger_game_get_step();
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

    printf("%d %d\n", frog.object->pos.x, ALLEGRO_DISPLAY_BORDER_LEFT);
    /* Me fijo que este en los limites */
    switch(input){
        case MOVE_UP:
            if( (frog.object->pos.y - step) < ALLEGRO_DISPLAY_BORDER_UP ){
                return false;
            }
            break;
        case MOVE_DOWN:
            if( (frog.object->pos.y + step) > ALLEGRO_DISPLAY_BORDER_DOWN ){
                return false;
            }
            break;
        case MOVE_LEFT:
            if( ((frog.object->pos.x) - step) < ALLEGRO_DISPLAY_BORDER_LEFT ){
                return false;
            }
            break;
        case MOVE_RIGHT:
            if( (frog.object->pos.x + step) > ALLEGRO_DISPLAY_BORDER_RIGHT ){
                return false;
            }
            break;
        default:
            break;
    }

    /* No hubo caso invalido */
    return true;
}

/* frogger_game_create_object */
ANIMATED_OBJECT* frogger_game_create_object(POSITION pos, SPEED speed, GUI_ANIMATION_ORIENTATION orientation, uint32_t type){
    ANIMATED_OBJECT* object;

    pos.x *= ALLEGRO_DISPLAY_STEP;
    pos.y *= ALLEGRO_DISPLAY_STEP;

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
        default:
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

/* RPI_PHYSICS
*/

#include "../frogger_physics.h"
#include "../../gui_input.h"

/* Configuraciones generales */

#define RPI_DISPLAY_STEP    1
#define RPI_STEP_RESOLUTION 1

/* Paths para acceder a animaciones */
#define RPI_PATH_OBJECTS                "gui/frogger/rpi/objects/"

/* Configuracion */
#define RPI_DISPLAY_BORDER_UP       0
#define RPI_DISPLAY_BORDER_DOWN     10
#define RPI_DISPLAY_BORDER_LEFT     0
#define RPI_DISPLAY_BORDER_RIGHT    15

#define RPI_FROG_OBJFILE                RPI_PATH_OBJECTS "frog/frog"
#define RPI_MOTORBIKE_OBJFILE           RPI_PATH_OBJECTS "motorbike/motorbike"
#define RPI_CAR_OBJFILE                 RPI_PATH_OBJECTS "car/car"
#define RPI_TRUCK_OBJFILE               RPI_PATH_OBJECTS "truck/truck"
#define RPI_BOAT_OBJFILE                RPI_PATH_OBJECTS "boat/boat"
#define RPI_YACHT_OBJFILE               RPI_PATH_OBJECTS "yacht/yacht"

/**********************/
/* Funciones publicas */
/**********************/

/* frogger_frog_selector */
char* frogger_frog_selector(uint16_t index){
    return RPI_FROG_OBJFILE;
}

/* frogger_game_get_step */
uint32_t frogger_game_get_step(void){
    return RPI_DISPLAY_STEP;
}

/* frogger_game_get_resolution */
uint32_t frogger_game_get_resolution(void){
    return RPI_STEP_RESOLUTION;
}

/* frogger_game_frog_init */
bool frogger_game_frog_init(FROG* frog, uint16_t characterId){
    /* Cargo la ranita :D */
    frog->object = gui_animation_load_object(frogger_frog_selector(characterId), map_position(DEFAULT_FROG_X * RPI_DISPLAY_STEP, DEFAULT_FROG_Y * RPI_DISPLAY_STEP), DEFAULT_FROG_ANIMATION);
    if( frog->object == NULL ){
        return false;
    }

    /* Frog inicializada correctamente */
    return true;
}

/* frogger_game_movement_valid */
bool frogger_game_movement_valid(FROG frog, INPUT_VALUES input){
/* Me fijo que este en los limites */
    switch(input){
        case MOVE_UP:
            if( frog.object->pos.y == RPI_DISPLAY_BORDER_UP ){
                return false;
            }
            break;
        case MOVE_DOWN:
            if( frog.object->pos.y == RPI_DISPLAY_BORDER_DOWN ){
                return false;
            }
            break;
        case MOVE_LEFT:
            if( frog.object->pos.x == RPI_DISPLAY_BORDER_LEFT ){
                return false;
            }
            break;
        case MOVE_RIGHT:
            if( frog.object->pos.x == RPI_DISPLAY_BORDER_RIGHT ){
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
ANIMATED_OBJECT* frogger_game_create_object(POSITION pos, SPEED speed, GUI_ANIMATION_ORIENTATION orientation, uint32_t type, uint16_t vehiclesId){
    ANIMATED_OBJECT* object;

    pos.x *= RPI_DISPLAY_STEP;
    pos.y *= RPI_DISPLAY_STEP;

    /* Cargo el object file */
    switch( type ){
        case FROGGER_MOTORBIKE:
            object = gui_animation_load_object( RPI_MOTORBIKE_OBJFILE, pos, "" );
            break;
        case FROGGER_CAR:
            object = gui_animation_load_object( RPI_CAR_OBJFILE, pos, "" );
            break;
        case FROGGER_TRUCK:
            object = gui_animation_load_object( RPI_TRUCK_OBJFILE, pos, "" );
            break;
        case FROGGER_BOAT:
            object = gui_animation_load_object( RPI_BOAT_OBJFILE, pos, "" );
            break;
        case FROGGER_YACHT:
            object = gui_animation_load_object( RPI_YACHT_OBJFILE, pos, "" );
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

/* ALLEGRO_PHYSICS
 */

#include "../../gui_input.h"
#include "../frogger_physics.h"

/* Configuracion */
#define ALLEGRO_DISPLAY_BORDER_UP       0
#define ALLEGRO_DISPLAY_BORDER_DOWN     440
#define ALLEGRO_DISPLAY_BORDER_LEFT     0
#define ALLEGRO_DISPLAY_BORDER_RIGHT    600

#define ALLEGRO_DISPLAY_STEP            40
#define ALLEGRO_STEP_RESOLUTION         10

/* Obstaculos del juego */ 
#define TRASH           map_region( map_position(0, 400), map_position(80, 440) )
#define PHONE           map_region( map_position(600, 400), map_position(640, 440) )
#define BORDER_LEFT     map_region( map_position(-40, 0), map_position(0, 480) )
#define BORDER_RIGHT    map_region( map_position(640, 0), map_position(680, 480) )
#define BORDER_UP       map_region( map_position(0, -40), map_position(0, 480) )
#define BORDER_DOWN     map_region( map_position(0, 440), map_position(640, 480) )

/* Paths para acceder a animaciones */
#define ALLEGRO_PATH_OBJECTS                "gui/frogger/allegro/objects/"
    
#define ALLEGRO_FROG_OBJFILE                ALLEGRO_PATH_OBJECTS "frog/frog"
#define ALLEGRO_FROGGY_OBJFILE              ALLEGRO_PATH_OBJECTS "frog/froggy"
#define ALLEGRO_BIRD_OBJFILE                ALLEGRO_PATH_OBJECTS "frog/bird"
#define ALLEGRO_MOTORBIKE_OBJFILE           ALLEGRO_PATH_OBJECTS "motorbike/motorbike"
#define ALLEGRO_CAR_OBJFILE                 ALLEGRO_PATH_OBJECTS "car/car"
#define ALLEGRO_TRUCK_OBJFILE               ALLEGRO_PATH_OBJECTS "truck/truck"
#define ALLEGRO_BOAT_OBJFILE                ALLEGRO_PATH_OBJECTS "boat/boat"
#define ALLEGRO_SMALL_CLOUD_OBJFILE         ALLEGRO_PATH_OBJECTS "boat/small_cloud"
#define ALLEGRO_YACHT_OBJFILE               ALLEGRO_PATH_OBJECTS "yacht/yacht"
#define ALLEGRO_CLOUD_OBJFILE               ALLEGRO_PATH_OBJECTS "yacht/cloud"

/**********************/
/* Funciones publicas */
/**********************/


/* frogger_frog_selector */
char* frogger_frog_selector(uint16_t index){
    switch(index){
        case 1:
            return ALLEGRO_FROGGY_OBJFILE;
            break;
        case 2:
            return ALLEGRO_BIRD_OBJFILE;
            break;
        default:
            return ALLEGRO_FROG_OBJFILE;
            break;
    }
}

/* frogger_yatch_selector */
char* frogger_yatch_selector(uint16_t index){
    switch(index){
        case 1:
            return ALLEGRO_CLOUD_OBJFILE;
            break;
        default:
            return ALLEGRO_YACHT_OBJFILE;
            break;
    }
}

/* frogger_boat_selector */
char* frogger_boat_selector(uint16_t index){
    switch(index){
        case 1:
            return ALLEGRO_SMALL_CLOUD_OBJFILE;
            break;
        default:
            return ALLEGRO_BOAT_OBJFILE;
            break;
    }
}

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
bool frogger_game_frog_init(FROG* frog, uint16_t characterId){
    int32_t dx, dy;
    
    /* Cargo la ranita :D */
    frog->object = gui_animation_load_object(frogger_frog_selector(characterId), map_position(DEFAULT_FROG_X * ALLEGRO_DISPLAY_STEP, DEFAULT_FROG_Y * ALLEGRO_DISPLAY_STEP), DEFAULT_FROG_ANIMATION);

    /* Verifico error de carga */
    if( frog->object == NULL ){
        return false;
    }    
    
    /* Calculo desfasaje para posicion */
    dx = (ALLEGRO_DISPLAY_STEP - frog->object->width) / 2;
    dy = (ALLEGRO_DISPLAY_STEP - frog->object->height) / 2;

    /* Recalibro posicion para centrar en division */
    frog->object->pos = map_position(frog->object->pos.x + dx, frog->object->pos.y + dy); 

    /* Frog inicializada correctamente */
    return true;
}

/* frogger_game_movement_valid */
bool frogger_game_movement_valid(FROG frog, INPUT_VALUES input){
    POSITION frogPos = frog.object->pos;
    REGION frogRegion;
    int32_t step = frogger_game_get_step();
    
    switch(input){
        case MOVE_UP:
            frogPos.y -= step;
            break;
        case MOVE_DOWN:
            frogPos.y += step;
            break;
        case MOVE_LEFT:
            frogPos.x -= step;
            break;
        case MOVE_RIGHT:
            frogPos.x += step;
            break;
    }
    
    frogRegion = map_region(frogPos, map_position(frogPos.x + frog.object->width, frogPos.y + frog.object->height));
    
    /* Me fijo que no haya colision con obstaculos */
    if( gui_animation_region_collision(TRASH, frogRegion) ){
        return false;
    }
    
    /* Me fijo que no haya colision con obstaculos */
    if( gui_animation_region_collision(PHONE, frogRegion) ){
        return false;
    }
    
    
    /* Me fijo que este en los limites */
    if( gui_animation_region_collision(BORDER_LEFT, frogRegion) ){
        return false;
    }
    if( gui_animation_region_collision(BORDER_RIGHT, frogRegion) ){
        return false;
    }
    if( gui_animation_region_collision(BORDER_UP, frogRegion) ){
        return false;
    }
    if( gui_animation_region_collision(BORDER_DOWN, frogRegion) ){
        return false;
    }

    /* No hubo caso invalido */
    return true;
}

/* frogger_game_create_object */
ANIMATED_OBJECT* frogger_game_create_object(POSITION pos, SPEED speed, GUI_ANIMATION_ORIENTATION orientation, uint32_t type, uint16_t vehiclesId){
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
            object = gui_animation_load_object( frogger_boat_selector(vehiclesId), pos, "" );
            break;
        case FROGGER_YACHT:
            object = gui_animation_load_object( frogger_yatch_selector(vehiclesId), pos, "" );
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

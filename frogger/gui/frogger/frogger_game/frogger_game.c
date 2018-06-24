#include "frogger_game.h"
#include "../../gui_input/gui_input.h"
#include "../../allegro/frogger/game/allegro_frogger_game.h"
#include "../../gui_files/gui_files.h"
#include "../../gui_types.h"

#include <string.h>
#include <stdio.h>

/*********************/
/* Objetos del juego */
/*********************/

/* Motor de animaciones */
static ANIMATION_ENGINE* engine = NULL;

/* Frog del jugador */
FROG frog = { .object=NULL, .transport=NULL };

/* Carriles del juego */
FIELD field;

/************************************/
/* Prototipos de funciones privadas */
/************************************/

/* frogger_game_field_init
 * Inicializa el campo de juego
 */
static bool frogger_game_field_init(void);

/* frogger_game_destroy_lane
 * Libera memoria del carril
 *
 * lane: Carril
 */
static void frogger_game_destroy_lane(LANE* lane);

/* frogger_game_create_lane
 * Crea un objeto carril
 * 
 * laneNumber: Numero de carril
 * type: Tipo de vehiculos
 * qty: Cantidad de objetos 
 * orientation: Orientacion de movimiento
 * speed: Velocidad de movimiento
 */
static LANE* frogger_game_create_lane(uint16_t laneNumber, uint16_t type, uint16_t qty, uint16_t orientation, SPEED speed);

/* create_lane_cfg
 * Crea una lista de archivos
 * 
 * amount: Cantidad
 */
static LANE_CFG* create_lane_cfg(uint32_t amount);

/* destroy_lane_cfg 
 * Libera las posiciones de un lane cfg
 *
 * amount: Cantidad 
 */
static void destroy_lane_cfg(LANE_CFG* lanesCfg, uint32_t amount);

/************************************/
/* Definicion de funciones privadas */
/************************************/

static LANE_CFG* create_lane_cfg(uint32_t amount){
    uint32_t i;
    LANE_CFG* laneCfg;
    
    /* Reservo memoria */
    laneCfg = malloc( sizeof(LANE_CFG) * amount );
    if( laneCfg == NULL ){
        return NULL;
    }
    
    /* Itero */
    for(i = 0;i < amount;i++){
        laneCfg[i] = NULL;
    }
    
    return laneCfg;
}

static void destroy_lane_cfg(LANE_CFG* lanesCfg, uint32_t amount){
    uint32_t i;
    
    /* Itero y libero elementos */
    for(i = 0;i < amount;i++){
        if( lanesCfg[i] != NULL ){
            free(lanesCfg[i]);
        }
    }
    
    /* Libero la lane cfg */
    free(lanesCfg);
}

/* frogger_game_create_field */
static bool frogger_game_field_init(void){
    SETTING* setting;
    uint32_t i;
    char fileIndex[2];
    char* auxStr;
    
    /* Cargo el archivo de configuraciones */
    setting = gui_files_load_setting(FROGGER_FIELD_CFG);
    if( setting == NULL ){
        return false;
    }
    
    /* Inicializo los parametros */
    if( !gui_files_get_int(setting, FIELD_ATTRIBUTES, FIELD_QUANTITY, &field.lanesQty) ){
        gui_files_destroy_setting(setting);
        return false;
    }
    
    /* Reservo memoria para los nombres de archivos */
    field.lanesCfg = create_lane_cfg(field.lanesQty);
    if( field.lanesCfg == NULL ){
        gui_files_destroy_setting(setting);
        return false;
    }
    
    /* Busco cada nombre de archivo */
    for(i = 0;i < field.lanesQty;i++){
        /* Armo la clave del parametro */
        sprintf(fileIndex, "%d", i);
        
        /* Busco el elemento */
        auxStr = gui_files_get_string(setting, FIELD_FILES, fileIndex);
        if( auxStr == NULL ){
            destroy_lane_cfg(field.lanesCfg, field.lanesQty);
            gui_files_destroy_setting(setting);
            return false;
        }
        
        /* Reservo memoria */
        field.lanesCfg[i] = malloc( sizeof(char) * (strlen(auxStr) + 1) );
        if( field.lanesCfg[i] == NULL ){
            destroy_lane_cfg(field.lanesCfg, field.lanesQty);
            gui_files_destroy_setting(setting);
            return false;
        }
        
        /* Guardo en memoria */
        strcpy(field.lanesCfg[i], auxStr);
    }
    
    /* Itero e inicializo cada lane */
    
    return true;
}

/* frogger_game_destroy_lane */
static void frogger_game_destroy_lane(LANE* lane){
    /* Libero memoria */
    free(lane->objects);
    
    /* Libero memoria */
    free(lane);
}

/* frogger_game_create_lane */
static LANE* frogger_game_create_lane(uint16_t laneNumber, uint16_t type, uint16_t qty, uint16_t orientation, SPEED speed){
    LANE* lane;
    
    /* Reservo memoria para el carril */
    lane = malloc( sizeof(LANE) );
    if( lane == NULL ){
        return NULL;
    }
    
    /* Reservo memoria para los objetos */
    lane->objects = malloc( sizeof(ANIMATED_OBJECT*) * qty );
    if( lane->objects == NULL ){
        free(lane);
        return NULL;
    }
    
    /* Inicializo atributos */
    lane->laneNumber = laneNumber;
    lane->type = type;
    lane->orientation = orientation;
    lane->objectsQty = qty;
    lane->speed = speed;
    
    return lane;
}

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* frogger_game_move_frog */
void frogger_game_move_frog(uint16_t input){
    uint32_t step;
    
    /* Verifico que este quieta */
    if( frog.object->status == GUI_ANIMATION_STATE_STATIC ){
        
        /* Me fijo si el desplazamiento es valido */
#if PLATFORM_MODE == PC_ALLEGRO
        if( !allegro_frogger_movement_valid(input) ){
            return;
        }
#elif PLATFORM_MODE == RPI
#endif   
        
        /* Cargo el desplazamiento */
#if PLATFORM_MODE == PC_ALLEGRO
        step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif   
        
        /* Muevo segun orientacion */
        switch(input){
            case MOVE_UP:
                gui_animation_start_movement(frog.object, GUI_ANIMATION_VERTICAL_UP, frog.object->currentPos.x, frog.object->currentPos.y - step);
                break;
            case MOVE_DOWN:
                gui_animation_start_movement(frog.object, GUI_ANIMATION_VERTICAL_DOWN, frog.object->currentPos.x, frog.object->currentPos.y + step);
                break;
            case MOVE_LEFT:
                gui_animation_start_movement(frog.object, GUI_ANIMATION_HORIZONTAL_LEFT, frog.object->currentPos.x - step, frog.object->currentPos.y);
                break;
            case MOVE_RIGHT:
                gui_animation_start_movement(frog.object, GUI_ANIMATION_HORIZONTAL_RIGHT, frog.object->currentPos.x + step, frog.object->currentPos.y);
                break;
        }
    }
}

/* frogger_game_continue */
void frogger_game_continue(void){
    gui_animation_continue_engine(engine);
}

/* frogger_game_pause */
void frogger_game_pause(void){
    gui_animation_pause_engine(engine);
}

/* frogger_game_close */
void frogger_game_close(void){
    if( frog.object != NULL ){
        gui_animation_destroy_object(frog.object);
    }
    
    /* Libero motor de animaciones */
    if( engine != NULL ){
        gui_animation_destroy_engine(engine);
    }
}

/* frogger_game_init */
bool frogger_game_init(void){
    
    /* Inicializo los carriles */
    if( !frogger_game_field_init() ){
        return false;
    }
    
    /* Creo el motor de animaciones */
    engine = gui_animation_create_engine();
    if( engine == NULL ){
        gui_animation_destroy_object(frog.object);
        return false;
    }
    
    /* Inicializo los objetos de cada interfaz */
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_init();
#elif PLATFORM_MODE == RPI
#endif
    
    /* Vinculo objetos con el motor */
    if( !gui_animation_attach_object(engine, frog.object) ){
        return false;
    }
    
    /* Inicio el motor */
    gui_animation_start_engine(engine);
}

/* frogger_game_screen_update */
void frogger_game_screen_update(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_screen_update();
#elif PLATFORM_MODE == RPI
#endif
}

/* frogger_game_screen_close */
void frogger_game_screen_close(void){
#if PLATFORM_MODE == PC_ALLEGRO
    allegro_frogger_screen_close();
#elif PLATFORM_MODE == RPI
#endif
}
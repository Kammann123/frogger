#include "frogger_game.h"
#include "../../gui_input/gui_input.h"
#include "../../gui_files/gui_files.h"
#include "../../gui_types.h"
#include "../../allegro/frogger/game/allegro_frogger_game.h"

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

/* frogger_game_lane_init 
 * Inicializa un carril con su archivo de configuracion
 * 
 * laneCfg: Archivo de configuracion
 * lane: Instancia de un carril
 */
static bool frogger_game_lane_init(LANE_CFG laneCfg, LANE* lane);

/* frogger_game_create_object
 * Crea un objeto de un carril con los parametros especificados
 *
 * pos: Posicion inicial
 * speed: Velocidad
 * orientation: Orientacion
 * type: Tipo de objeto
 */
static FROGGER_OBJECT frogger_game_create_object(POSITION pos, SPEED speed, uint32_t orientation, uint32_t type);

/* frogger_game_lane_sequence 
 * Crea una secuencia de posiciones para un carril, de forma aleatoria
 * 
 * amount: Cantidad de objetos
 * objectSize: Tamaño del objeto
 * y: Posicion Y
 * positions: Arreglo de posiciones resultados
 */
static bool frogger_game_lane_sequence(uint32_t amount, uint32_t objectSize, uint32_t y, POSITION* positions);

/* frogger_game_object_size }
 * Largo de un objeto
 * 
 * type: Tipo del objeto
 */
uint32_t frogger_game_object_size(uint32_t type);

/* frogger_game_create_list_lane
 * Crea un arreglo de LANE y inicializa 
 *
 * amount: Cantidad
 */
static LANE* frogger_game_create_list_lane(uint32_t amount);

/* frogger_game_create_list_objects
 * Creo arreglo de objetos e inicializo
 *
 * amount: Cantidad
 */
static FROGGER_OBJECT* frogger_game_create_list_objects(uint32_t amount);

/* frogger_game_create_list_lane_cfg 
 * Creo arreglo de archivos de los setting de lane
 * 
 * amount: Cantidad
 */
static LANE_CFG* frogger_game_create_list_lane_cfg(uint32_t amount);

/* frogger_game_destroy_field 
 * Libera memoria de un campo de juego
 *
 * field: Instancia
 */
static void frogger_game_destroy_field(FIELD field);

/* frogger_game_destroy_lane_cfg 
 * Libera memoria de los archivos de configuracion
 *
 * lanesCfg: Lista de archivos
 * length: Cantidad de elementos de lista 
 */
static void frogger_game_destroy_list_lane_cfg(LANE_CFG* lanesCfg, uint32_t length);

/* frogger_game_destroy_list_lane
 * Libera la memoria de los carriles
 *
 * lanes: Instancias de carriles
 * length: Largo de la lista
 */
static void frogger_game_destroy_list_lane(LANE* lanes, uint32_t length);

/* frogger_game_destroy_list_objects
 * Libera memoria de lista de objetos
 *
 * objects: Lista de objetos 
 * length: Largo de la lista
 */
static void frogger_game_destroy_list_objects(FROGGER_OBJECT* objects, uint32_t length);

/************************************/
/* Definicion de funciones privadas */
/************************************/

/* frogger_game_create_list_lane_cfg */
static LANE_CFG* frogger_game_create_list_lane_cfg(uint32_t amount){
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

/* frogger_game_create_list_objects */
static FROGGER_OBJECT* frogger_game_create_list_objects(uint32_t amount){
    uint32_t i;
    FROGGER_OBJECT* objects;
    
    /* Reservo memoria */
    objects = malloc( sizeof(FROGGER_OBJECT) * amount );
    if( objects == NULL ){
        return NULL;
    }
    
    /* Itero */
    for(i = 0;i < amount;i++){
        objects[i] = NULL;
    }
    
    return objects;
}

/* frogger_game_create_list_lane */
static LANE* frogger_game_create_list_lane(uint32_t amount){
    uint32_t i;
    LANE* lanes;
    
    /* Reservo memoria */
    lanes = malloc( sizeof(LANE) * amount );
    if( lanes == NULL ){
        return NULL;
    }
    
    /* Itero */
    for(i = 0;i < amount;i++){
        lanes[i].init = false;
    }
    
    return lanes;
}

/* frogger_game_destroy_field */
static void frogger_game_destroy_field(FIELD field){
    /* Libero lanes */
    frogger_game_destroy_list_lane(field.lanes, field.lanesQty);
    
    /* Libero lanes files */
    frogger_game_destroy_list_lane_cfg(field.lanesCfg, field.lanesQty);
    
}

/* frogger_game_destroy_lane_cfg */
static void frogger_game_destroy_list_lane_cfg(LANE_CFG* lanesCfg, uint32_t length){
    uint32_t i;
    
    /* Destruyo cada elemento */
    for(i = 0;i < length;i++){
        if( lanesCfg[i] != NULL ){
            free( lanesCfg[i] );
        }
    }
    
    /* Libero la lista */
    free(lanesCfg);
}

/* frogger_game_destroy_list_lane */
static void frogger_game_destroy_list_lane(LANE* lanes, uint32_t length){
    uint32_t i;
    
    /* Destruyo cada elemento */
    for(i = 0;i < length;i++){
        if( lanes[i].init ){
            frogger_game_destroy_list_objects(lanes[i].objects, lanes[i].objectsQty);
        }
    }
    
    /* Libero memoria de la lista */
    free(lanes);
}

/* frogger_game_destroy_list_objects */
static void frogger_game_destroy_list_objects(FROGGER_OBJECT* objects, uint32_t length){
    uint32_t i;
    
    /* Destruyo cada elemento */
    for(i = 0;i < length;i++){
        if( objects[i] != NULL ){
            gui_animation_destroy_object(objects[i]);  
        }
    }
    
    /* Libero memoria de la lista */
    free(objects);
}

/* frogger_game_lane_sequence */
static bool frogger_game_lane_sequence(uint32_t amount, uint32_t objectSize, uint32_t y, POSITION* positions){
    int32_t i, subdiv;
    
    /* Veo si es valido */
    subdiv = (MAP_X_MAX - MAP_X_MIN)/amount - (objectSize - 1);
    if( subdiv <= 1 ){
        return false;
    }
    
    /* Inicializo la semilla */
    srand( time(NULL) );
    
    /* Genero numeros aleatorios */
    for(i = 0;i < amount;i++){
        positions[i].y = y;
        positions[i].x = rand() % subdiv + i * (subdiv + (objectSize - 1));
    }
    
    return true;
}

/* frogger_game_create_object */
static FROGGER_OBJECT frogger_game_create_object(POSITION pos, SPEED speed, uint32_t orientation, uint32_t type){
    FROGGER_OBJECT object;
    
#if PLATFORM_MODE == PC_ALLEGRO
    object = allegro_frogger_create_object(pos, speed, orientation, type);
#elif PLATFORM_MODE == RPI
    
#endif
    
    return object;
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
    field.lanesCfg = frogger_game_create_list_lane_cfg(field.lanesQty);
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
            frogger_game_destroy_list_lane_cfg(field.lanesCfg, field.lanesQty);
            gui_files_destroy_setting(setting);
            return false;
        }
        
        /* Reservo memoria */
        field.lanesCfg[i] = malloc( sizeof(char) * (strlen(auxStr) + 1) );
        if( field.lanesCfg[i] == NULL ){
            frogger_game_destroy_list_lane_cfg(field.lanesCfg, field.lanesQty);
            gui_files_destroy_setting(setting);
            return false;
        }
        
        /* Guardo en memoria */
        strcpy(field.lanesCfg[i], auxStr);
    }
    
    /* Libero esta configuracion */
    gui_files_destroy_setting(setting);
    
    /* Reservo memoria para los carriles */
    field.lanes = frogger_game_create_list_lane(field.lanesQty);
    if( field.lanes == NULL ){
        frogger_game_destroy_list_lane_cfg(field.lanesCfg, field.lanesQty);
        return false;
    }
    
    /* Itero e inicializo cada lane */
    for(i = 0;i < field.lanesQty;i++){
        if( !frogger_game_lane_init(field.lanesCfg[i], field.lanes + i) ){
            frogger_game_destroy_list_lane_cfg(field.lanesCfg, field.lanesQty);
            frogger_game_destroy_list_lane(field.lanes, field.lanesQty);
            return false;
        }
    }
    
    return true;
}

/* frogger_game_object_size */
uint32_t frogger_game_object_size(uint32_t type){
    switch(type){
        case FROGGER_MOTORBIKE:
            return FROGGER_SIZE_MOTORBIKE;
            break;
        case FROGGER_CAR:
            return FROGGER_SIZE_CAR;
            break;
        case FROGGER_TRUCK:
            return FROGGER_SIZE_TRUCK;
            break;
        case FROGGER_BOAT:
            return FROGGER_SIZE_BOAT;
            break;
        case FROGGER_YACHT:
            return FROGGER_SIZE_YACHT;
            break;
    }
}

/* frogger_game_lane_init */
static bool frogger_game_lane_init(LANE_CFG laneCfg, LANE* lane){
    SETTING* setting;
    char* auxStr;
    uint32_t i, size;
    POSITION* positions;
    
    /* Cargo el archivo de configuracion */
    setting = gui_files_load_setting(laneCfg);
    if( setting == NULL ){
        return false;
    }
    
    /* Cargo los parametros enteros */
    if( !gui_files_get_int(setting, LANE_ATTRIBUTES, LANE_NUMBER, &lane->laneNumber) ){
        gui_files_destroy_setting(setting);
        return false;
    }
    if( !gui_files_get_int(setting, LANE_ATTRIBUTES, LANE_QUANTITY, &(lane->objectsQty)) ){
        gui_files_destroy_setting(setting);
        return false;
    }
    if( !gui_files_get_int(setting, LANE_ATTRIBUTES, LANE_TIMEDELTA, &(lane->speed.timeDelta)) ){
        gui_files_destroy_setting(setting);
        return false;
    }
    if( !gui_files_get_int(setting, LANE_ATTRIBUTES, LANE_SPACEDELTA, &(lane->speed.spaceDelta)) ){
        gui_files_destroy_setting(setting);
        return false;
    }
    
    /* Cargo los que son strings */
    auxStr = gui_files_get_string(setting, LANE_ATTRIBUTES, LANE_TYPE);
    if( auxStr == NULL ){
        gui_files_destroy_setting(setting);
        return false;
    }
    if( !(strcmp(auxStr, FROGGER_CFG_MOTORBIKE)) ){
        lane->type = FROGGER_MOTORBIKE;
    }else if( !(strcmp(auxStr, FROGGER_CFG_CAR)) ){
        lane->type = FROGGER_CAR;
    }else if( !(strcmp(auxStr, FROGGER_CFG_TRUCK)) ){
        lane->type = FROGGER_TRUCK;
    }else if( !(strcmp(auxStr, FROGGER_CFG_BOAT)) ){
        lane->type = FROGGER_BOAT;
    }else if( !(strcmp(auxStr, FROGGER_CFG_YACHT)) ){
        lane->type = FROGGER_YACHT;
    }
    
    auxStr = gui_files_get_string(setting, LANE_ATTRIBUTES, LANE_ORIENTATION);
    if( auxStr == NULL ){
        gui_files_destroy_setting(setting);
        return false;
    }
    if( !(strcmp(auxStr, OBJFILE_HORIZONTAL_LEFT)) ){
        lane->orientation = GUI_ANIMATION_HORIZONTAL_LEFT;
    }else if( !(strcmp(auxStr, OBJFILE_HORIZONTAL_RIGHT)) ){
        lane->orientation = GUI_ANIMATION_HORIZONTAL_RIGHT;
    }
    
    gui_files_destroy_setting(setting);
    
    /* Reservo memoria para los objetos */
    lane->objects = frogger_game_create_list_objects(lane->objectsQty);
    if( lane->objects == NULL ){
        return false;
    }
    
    /* Creo la secuencia */
    positions = malloc( sizeof(POSITION) * lane->objectsQty );
    if(positions == NULL){
        frogger_game_destroy_list_objects(lane->objects, lane->objectsQty);
        return false;
    }
    
    if( !frogger_game_lane_sequence(lane->objectsQty, frogger_game_object_size(lane->type), lane->laneNumber, positions)){
    
        frogger_game_destroy_list_objects(lane->objects, lane->objectsQty);
        return false;
    }
    
    /* Creo los objetos */
    for(i = 0;i < lane->objectsQty;i++){
        lane->objects[i] = frogger_game_create_object(positions[i], lane->speed, lane->orientation, lane->type);
        if( lane->objects[i] == NULL ){
            frogger_game_destroy_list_objects(lane->objects, lane->objectsQty);
            return false;
        }
    }
    
    /* Flag */
    lane->init = true;
    
    return true;
}

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* frogger_game_transport_off */
void frogger_game_transport_off(void);

/* frogger_game_transport_on */
void frogger_game_transport_on(void);

/* frogger_game_transport_frog */
void frogger_game_transport_frog(void){
    uint32_t i, ii, step;
    int32_t x;
    
    /* Cargo el desplazamiento */
#if PLATFORM_MODE == PC_ALLEGRO
        step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif 
    
    /* Me fijo que este en la region agua */
    if( gui_animation_in_region(frog.object->currentPos, REGION_WATER) ){
        
        /* Me fijo que este siendo transportado */
        if( frog.transport != NULL ){
            
            /* Me fijo si esta quieto */
            if( frog.object->status == GUI_ANIMATION_STATE_STATIC ){
                
                /* Configuro parametros de movimiento */
                frog.object->orientation = frog.transport->orientation;
                
                /* Configuro velocidad */
                for(i = 0;i < NUMBER_OF_ORIENTATIONS;i++){
                    
                    /* Busco la animacion de la rana */
                    if( frog.object->animations[i].orientation == frog.transport->orientation ){
                        
                        /* Busco animacion del barco */
                        for(ii = 0;ii < NUMBER_OF_ORIENTATIONS;ii++){
                            
                            if( frog.transport->animations[ii].orientation == frog.transport->orientation ){
                                frog.object->animations[i].speed.timeDelta = frog.transport->animations[ii].speed.timeDelta;
                                frog.object->animations[i].speed.spaceDelta = frog.transport->animations[ii].speed.spaceDelta; 
                            }                           
                        }
                    }
                }
                
                /* Calculo posicion final */
                switch( frog.transport->orientation ){
                    case GUI_ANIMATION_HORIZONTAL_LEFT:
                        x = frog.object->currentPos.x - step;
                        break;
                    case GUI_ANIMATION_HORIZONTAL_RIGHT:
                        x = frog.object->currentPos.x + step;
                        break;
                }
                
                /* Valido posicion */
                gui_animation_start_static_movement(frog.object, x, frog.object->currentPos.y);  
            }
        }
    }
}

/* frogger_game_drown */
bool frogger_game_drown(void){
    
    /* Me fijo que este en region de agua */
    if( gui_animation_in_region(frog.object->currentPos, REGION_WATER) ){
        
        /* Me fijo que este quieto */
        if( frog.object->status == GUI_ANIMATION_STATE_STATIC ){
            
            /* Me fijo que no este en transporte */
            if( frog.transport == NULL ){
                
                return true;
            }
        }
    }
    
    return false;
}

/* frogger_game_reset_frog_position */
void frogger_game_reset_frog_position(void){
    uint32_t step;
        
    /* Cargo el desplazamiento */
#if PLATFORM_MODE == PC_ALLEGRO
        step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif   

    /* Seteo posicion y orientacion de rana */
    frog.object->currentPos = map_position(DEFAULT_FROG_X, DEFAULT_FROG_Y);
    frog.object->orientation = DEFAULT_FROG_ORIENTATION;
    
}

/* frogger_game_collisions */
bool frogger_game_collisions(void){
    uint32_t i, ii;
    uint32_t step;
        
    /* Cargo el desplazamiento */
#if PLATFORM_MODE == PC_ALLEGRO
        step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif   

    /* Me muevo por los carriles */
    for(i = 0;i < field.lanesQty;i++){
        
        /* Me muevo por los objetos */
        for(ii = 0;ii < field.lanes[i].objectsQty;ii++){
            
            /* Me fijo si hay colision */
            if( field.lanes[i].type != FROGGER_YACHT && field.lanes[i].type != FROGGER_BOAT ){
                
                if( gui_animation_collision(frog.object, field.lanes[i].objects[ii], step)){
                    return true;
                }
            }
        }
    }
    
    return false;    
}

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
    
    /* Destruyo objetos de campo */
    frogger_game_destroy_field(field);
}

/* frogger_game_init */
bool frogger_game_init(void){
    uint32_t i, ii;
    
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
        gui_animation_destroy_object(frog.object);
        gui_animation_destroy_engine(engine);
        return false;
    }
    
    /* Vinculos los objetos */
    for(i = 0;i < field.lanesQty;i++){
        for(ii = 0;ii < field.lanes[i].objectsQty;ii++){
            if( !gui_animation_attach_object(engine, field.lanes[i].objects[ii]) ){
                gui_animation_destroy_object(frog.object);
                frogger_game_destroy_field(field);
                return false;
            }
        }
    }
    
    
    /* Inicio el motor */
    gui_animation_start_engine(engine);
    
    return true;
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

/* frogger_game_move_lanes */
void frogger_game_move_lanes(void){
    uint32_t i, ii;
    int32_t step;
    int32_t x;
    
#if PLATFORM_MODE == PC_ALLEGRO
    step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif
    
    /* Itero los carriles */
    for(i = 0;i < field.lanesQty;i++){
        
        /* Itero los objetos de cada carril */
        for(ii = 0;ii < field.lanes[i].objectsQty;ii++){
            
            /* Me fijo su estado de animacion */
            if( field.lanes[i].objects[ii]->status == GUI_ANIMATION_STATE_STATIC ){
                
                /* Nueva posicion x */
                switch( field.lanes[i].orientation ){
                    case GUI_ANIMATION_HORIZONTAL_LEFT:
                        x = (field.lanes[i].objects[ii]->currentPos.x / step) - 1;
                        break;
                    case GUI_ANIMATION_HORIZONTAL_RIGHT:
                        x = (field.lanes[i].objects[ii]->currentPos.x / step) + 1;
                        break;
                }
                
                /* Verifico los limites */
                if( x < MAP_X_MIN ){
                    field.lanes[i].objects[ii]->currentPos.x = MAP_X_MAX * step;
                    x = MAP_X_MAX - 1;
                }else if( x > MAP_X_MAX ){
                    field.lanes[i].objects[ii]->currentPos.x = MAP_X_MIN * step;
                    x = MAP_X_MIN + 1;
                }
                
                /* Si esta quieto, lo desplazo */
                gui_animation_start_movement(field.lanes[i].objects[ii], field.lanes[i].orientation, x * step, field.lanes[i].laneNumber * step);
            }
        }
    }
}
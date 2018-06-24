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

/* frogger_game_destroy_lane
 * Libera memoria del carril
 *
 * lane: Carril
 */
static void frogger_game_destroy_lane(LANE* lane);

/* destroy_lane_cfg 
 * Libera las posiciones de un lane cfg
 *
 * amount: Cantidad 
 */
static void destroy_lane_cfg(LANE_CFG* lanesCfg, uint32_t amount);

/* destroy_lanes
 * Libera el arreglo y destruye sus elementos
 *
 * lanes: Arreglo
 * amount: Cantidad de elementos
 */
static void destroy_lanes(LANE* lanes, uint32_t amount);

/* create_lanes
 * Crea un arreglo de LANE y inicializa 
 *
 * amount: Cantidad
 */
static LANE* create_lanes(uint32_t amount);

/* create_objects
 * Creo arreglo de objetos e inicializo
 *
 * amount: Cantidad
 */
static FROGGER_OBJECT* create_objects(uint32_t amount);

/* destroy_objects
 * Destruyo los objetos y libero memoria
 *
 * lane: Carril
 */
static void destroy_objects(LANE* lane);

/* frogger_game_lane_init 
 * Inicializa un carril con su archivo de configuracion
 * 
 * laneCfg: Archivo de configuracion
 * lane: Instancia de un carril
 */
static bool frogger_game_lane_init(LANE_CFG laneCfg, LANE* lane);

/* frogger_game_lane_object
 * Crea un objeto de un carril con los parametros especificados
 *
 * pos: Posicion inicial
 * speed: Velocidad
 * orientation: Orientacion
 * type: Tipo de objeto
 */
static FROGGER_OBJECT frogger_game_lane_object(POSITION pos, SPEED speed, uint32_t orientation, uint32_t type);

/* frogger_game_lane_sequence 
 * Crea una secuencia de posiciones para un carril, de forma aleatoria
 * 
 * amount: Cantidad de objetos
 * objectSize: Tamaño del objeto
 * y: Posicion Y
 * positions: Arreglo de posiciones resultados
 */
static bool frogger_game_lane_sequence(uint32_t amount, uint32_t objectSize, uint32_t y, POSITION* positions);

/* frogger_game_object_size 
 * Devuelve el tamaño en divisiones del objeto
 * segun su id de tipo
 *
 * type: Tipo de objeto
 */
static uint32_t frogger_game_object_size(uint32_t type);

/************************************/
/* Definicion de funciones privadas */
/************************************/

/* frogger_game_object_size */
static uint32_t frogger_game_object_size(uint32_t type){
    switch( type ){
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
        default:
            return 0;
            break;
    }
}

/* frogger_game_lane_sequence */
static bool frogger_game_lane_sequence(uint32_t amount, uint32_t objectSize, uint32_t y, POSITION* positions){
    uint32_t i, subdiv;
    
    /* Veo si es valido */
    subdiv = (MAP_X_MAX - MAP_X_MIN)/amount - (objectSize - 1);
    if( subdiv <= objectSize ){
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

/* frogger_game_lane_object */
static FROGGER_OBJECT frogger_game_lane_object(POSITION pos, SPEED speed, uint32_t orientation, uint32_t type){
    FROGGER_OBJECT object;
    
#if PLATFORM_MODE == PC_ALLEGRO
    object = allegro_frogger_lane_object(pos, speed, orientation, type);
#elif PLATFORM_MODE == RPI
    
#endif
    
    return object;
}

/* create_lane_cfg */
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

/* destroy_lane_cfg */
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
    
    /* Libero esta configuracion */
    gui_files_destroy_setting(setting);
    
    /* Reservo memoria para los carriles */
    field.lanes = create_lanes(field.lanesQty);
    if( field.lanes == NULL ){
        destroy_lane_cfg(field.lanesCfg, field.lanesQty);
        return false;
    }
    
    /* Itero e inicializo cada lane */
    for(i = 0;i < field.lanesQty;i++){
        if( !frogger_game_lane_init(field.lanesCfg[i], field.lanes + i) ){
            destroy_lane_cfg(field.lanesCfg, field.lanesQty);
            destroy_lanes(field.lanes, field.lanesQty);
            return false;
        }
    }
    
    return true;
}

/* frogger_game_lane_init */
static bool frogger_game_lane_init(LANE_CFG laneCfg, LANE* lane){
    SETTING* setting;
    char* auxStr;
    uint32_t i;
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
    lane->objects = create_objects(lane->objectsQty);
    if( lane->objects == NULL ){
        return false;
    }
    
    /* Creo la secuencia */
    positions = malloc( sizeof(POSITION) * lane->objectsQty );
    if(positions == NULL){
        destroy_objects(lane);
        return false;
    }
    if( !frogger_game_lane_sequence(lane->objectsQty, frogger_game_object_size(lane->type), lane->laneNumber, positions)){
        destroy_objects(lane);
        return false;
    }
    
    /* Creo los objetos */
    for(i = 0;i < lane->objectsQty;i++){
        lane->objects[i] = frogger_game_lane_object(map_position(0, lane->laneNumber), lane->speed, lane->orientation, lane->type);
        if( lane->objects[i] == NULL ){
            destroy_objects(lane);
            return false;
        }
    }
    
    return true;
}

static FROGGER_OBJECT* create_objects(uint32_t amount){
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

static void destroy_objects(LANE* lane){
    uint32_t i;
    
    /* Itero */
    for(i = 0;i < lane->objectsQty;i++){
        if( lane->objects[i] != NULL ){
            gui_animation_destroy_object( lane->objects[i] );
        }
    }
    
    /* Libero memoria de la lista */
    free(lane->objects);
}

/* create_lanes */
static LANE* create_lanes(uint32_t amount){
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

/* destroy_lanes */
static void destroy_lanes(LANE* lanes, uint32_t amount){
    uint32_t i;
    
    /* Itero */
    for(i = 0;i < amount;i++){
        if( lanes[i].init ){
            
        }
    }
    
    /* Libero memoria */
    free(lanes);
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
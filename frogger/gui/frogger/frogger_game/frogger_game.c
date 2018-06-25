#include "frogger_game.h"
#include "../../gui_input/gui_input.h"
#include "../../gui_files/gui_files.h"
#include "../../gui_types.h"
#include "../../allegro/frogger/game/allegro_frogger_game.h"
#include "../../allegro/frogger/lostscreen/allegro_frogger_lostscreen.h"
#include "../../gui_animation/gui_animation.h"

/*********************/
/* Objetos del juego */
/*********************/

/* Motor de animaciones */
static ANIMATION_ENGINE* engine = NULL;

/* Frog del jugador */
FROG frog = { .object=NULL, .transport=NULL };

/* Carriles del juego */
FIELD field;

/******************/
/* FIELD handlers */
/******************/

/* frogger_game_create_field 
 * Crea una instancia del campo de juego 
 */
static FIELD frogger_game_create_field(void);

/* frogger_game_init_field 
 * Inicializa memoria para el field 
 *
 * field: Instancia 
 * qty: Cantidad de carriles
 */
static bool frogger_game_init_field(FIELD* field, LENGTH qty);

/* frogger_game_destroy_field
 * Destruye y libera instancia field
 *
 * field: Instancia*/
static void frogger_game_destroy_field(FIELD* field);

/* frogger_game_load_field 
 * Cargo en memoria el mapa
 *
 * field: Instancia
 * filename: Nombre del archivo
 */
static bool frogger_game_load_field(FIELD* field, char* filename);

/*****************/
/* LANE handlers */
/*****************/

/* frogger_game_create_lane 
 * Crea una instancia del objeto LANE
 */
static LANE frogger_game_create_lane(void);

/* frogger_game_init_lane 
 * Inicializa memoria para el objeto LANE
 * 
 * lane: Instancia
 * qty: Cantidad de objetos del carril
 */
static bool frogger_game_init_lane(LANE* lane, LENGTH qty);

/* frogger_game_destroy_lane 
 * Destruye y libera memoria del carril
 *
 * lane: Instancia 
 */
static void frogger_game_destroy_lane(LANE* lane);

/* frogger_game_load_lane 
 * Carga en memoria una instancia de LANE
 * 
 * lane: Instancia del carril
 * filename: Archivo configuracion del carril
 */
static bool frogger_game_load_lane(LANE* lane, char* filename);

/**********************/
/* LANE_FILE handlers */
/**********************/

/* frogger_game_create_cfg 
 * Crea una instancia de nombre de configuracion
 */
static CFG frogger_game_create_cfg(void);

/* frogger_game_init_cfg
 * Inicializa memoria para instancia
 *
 * cfg: Instancia
 * file: Nombre de archivo
 */
static bool frogger_game_init_cfg(CFG* cfg, char* file);

/* frogger_game_destroy_cfg 
 * Destruye y libera memoria del cfg
 *
 * cfg: Instancia
 */
static void frogger_game_destroy_cfg(CFG* cfg);

/************************************/
/* Prototipos de funciones privadas */
/************************************/

/* frogger_game_type_conv 
 * Convierte el tipo string a tipo numerico
 *
 * str: String con el tipo de objeto 
 */
static OBJECT_TYPES frogger_game_type_conv(char* str);

/* frogger_game_create_object
 * Crea un objeto de un carril con los parametros especificados
 *
 * pos: Posicion inicial
 * speed: Velocidad
 * orientation: Orientacion
 * type: Tipo de objeto
 */
static FROGGER_OBJECT frogger_game_create_object(POSITION pos, SPEED speed, GUI_ANIMATION_ORIENTATION orientation, OBJECT_TYPES type);

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
static uint32_t frogger_game_object_size(uint32_t type);

/* frogger_game_speed_resolution 
 * Recalcula velocidad con la resolucion elegida por la interfaz
 *
 * speed: Velocidad
 */
static SPEED frogger_game_speed_resolution(SPEED speed);

/* frogger_game_new_speed
 * Calcula nueva velocidad
 *
 * speed: Velocidad base
 * factor: Factor de reduccion del timedelta
 * var: Punto variable
 */
static SPEED frogger_game_new_speed(SPEED speed, uint32_t factor, uint32_t var);

/* frogger_game_get_norm_pos
 * Devuelve una posicion normalizada del objeto desde la referencia marcada 
 * 
 * reference: Referencia 
 * obj: Objeto
 */
static POSITION frogger_game_get_norm_pos(POSITION reference, POSITION obj);

/******************/
/* FIELD handlers */
/******************/

/* frogger_game_load_field */
static bool frogger_game_load_field(FIELD* field, char* filename){
    SETTING* settings;
    LENGTH i;
    char fi[5];
    char* str;
    
    /* Creo la instancia field */
    *field = frogger_game_create_field();
    
    /* Abro el archivo de configuracion */
    settings = gui_files_load_setting(filename);
    if( settings == NULL ){
        return false;
    }
    
    /* Busco el tamaño */
    if( !gui_files_get_int(settings, FIELD_ATTRIBUTES, FIELD_QUANTITY, &field->lanesQty) ){
        gui_files_destroy_setting(settings);
        return false;
    }
    if( !field->lanesQty ){
        gui_files_destroy_setting(settings);
        return false;
    }
    
    /* Inicializo la memoria */
    if( !frogger_game_init_field(field, field->lanesQty) ){
        gui_files_destroy_setting(settings);
        return false;
    }
    
    /* Creo los archivos y carriles */
    for(i = 0;i < field->lanesQty;i++){
        field->files[i] = frogger_game_create_cfg();
        field->lanes[i] = frogger_game_create_lane();
    }
    
    /* Cargo el contenido */
    for(i = 0;i < field->lanesQty;i++){
        /* Armo el string index */
        sprintf(fi, "%d", i);
        
        /* Busco el contenido */
        str = gui_files_get_string(settings, FIELD_FILES, fi);
        if( str == NULL ){
            frogger_game_destroy_field(field);
            gui_files_destroy_setting(settings);
            return false;
        }
        
        /* Inicializo config */
        if( !frogger_game_init_cfg(&field->files[i], str) ){
            frogger_game_destroy_field(field);
            gui_files_destroy_setting(settings);
            return false;
        }
        
        /* Cargo el carril */
        if( !frogger_game_load_lane(&field->lanes[i], str) ){
            frogger_game_destroy_field(field);
            gui_files_destroy_setting(settings);
            return false;
        }
    }
    
    /* Exito! */
    gui_files_destroy_setting(settings);
    return true;    
}

/* frogger_game_create_field */
static FIELD frogger_game_create_field(void){
    FIELD field;
    
    /* Parametros default */
    field.lanes = NULL;
    field.files = NULL;
    field.init = false;
    
    /* Devuelvo */
    return field;
}

/* frogger_game_init_field */
static bool frogger_game_init_field(FIELD* field, LENGTH qty){
    
    if( field->init ){
        return false;
    }
    
    /* Reserva memoria para arreglo de lanes */
    field->lanes = malloc( sizeof(LANE) * qty );
    if( field->lanes == NULL ){
        return false;
    }
    
    /* Reserva memoria para arreglo de files */
    field->files = malloc( sizeof(CFG) * qty );
    if( field->files == NULL ){
        free( field->lanes );
        return false;
    }
    
    /* Inicializado */
    field->lanesQty = qty;
    field->init = true;
    
    /* Exito! */
    return true;
}

/* frogger_game_destroy_field */
static void frogger_game_destroy_field(FIELD* field){
    LENGTH i;
    /* Verifico inicializacion */
    if( field->init ){
        
        /* Muevo por cada elemento */
        for(i = 0;i < field->lanesQty;i++){
            
            /* Destruyo el carril */
            frogger_game_destroy_lane(&field->lanes[i]);
            
            /* Destruyo su archivo */
            frogger_game_destroy_cfg(&field->files[i]);
        }
        
        /* Libero memoria de arreglos */
        free( field->lanes );
        free( field->files );
    }
    
    field->init = false;
}

/*****************/
/* LANE handlers */
/*****************/

/* frogger_game_create_lane */
static LANE frogger_game_create_lane(void){
    LANE lane;
    
    /* Parametros default */
    lane.objects = NULL;
    lane.init = false;
    
    /* Devuelvo el objeto */
    return lane;
}

/* frogger_game_init_lane */
static bool frogger_game_init_lane(LANE* lane, LENGTH qty){
    
    if(lane->init){
        return false;
    }
    
    /* Reservo memoria para objetos */
    lane->objects = malloc( sizeof(FROGGER_OBJECT) * qty );
    if( lane->objects == NULL ){
        return false;
    }
    
    /* Inicializado!! */
    lane->objectsQty = qty;
    lane->init = true;
    
    /* Exito! */
    return true;
    
}

/* frogger_game_destroy_lane */
static void frogger_game_destroy_lane(LANE* lane){
    LENGTH i;
    /* Verifico inicializacion */
    if( lane->init ){
        
        /* Recorro y elimino objetos */
        for(i = 0;i < lane->objectsQty;i++){
            gui_animation_destroy_object(lane->objects[i]);
        }
        
        /* Libero memoria del arreglo */
        free(lane->objects);
    }
    lane->init = false;
}

/* frogger_game_load_lane */
static bool frogger_game_load_lane(LANE* lane, char* filename){
    SETTING* settings;
    POSITION* positions;
    LENGTH i;
    char* str;
    
    /* Abro archivo de configuracion */
    settings = gui_files_load_setting(filename);
    if( settings == NULL ){
        return false;
    }
    
    /* Busco la cantidad de objetos */
    if( !gui_files_get_int(settings, LANE_ATTRIBUTES, LANE_ATT_QUANTITY, &lane->objectsQty) ){
        gui_files_destroy_setting(settings);
        return false;
    }
    if( !lane->objectsQty ){
        gui_files_destroy_setting(settings);
        return false;
    }
    
    /* Inicializo la instancia */
    if( !frogger_game_init_lane(lane, lane->objectsQty) ){
        gui_files_destroy_setting(settings);
        return false;
    }
    
    /* Cargo parametros numericos */
    if( !gui_files_get_int(settings, LANE_ATTRIBUTES, LANE_ATT_NUMBER, &lane->laneNumber) ){
        frogger_game_destroy_lane(lane);
        gui_files_destroy_setting(settings);
        return false;
    }
    if( !gui_files_get_int(settings, LANE_ATTRIBUTES, LANE_ATT_TIME, &lane->speed.timeDelta) ){
        frogger_game_destroy_lane(lane);
        gui_files_destroy_setting(settings);
        return false;
    }
    if( !gui_files_get_int(settings, LANE_ATTRIBUTES, LANE_ATT_SPACE, &lane->speed.spaceDelta) ){
        frogger_game_destroy_lane(lane);
        gui_files_destroy_setting(settings);
        return false;
    }
    if( !gui_files_get_int(settings, LANE_ATTRIBUTES, LANE_ATT_ACCEL, &lane->aFactor) ){
        frogger_game_destroy_lane(lane);
        gui_files_destroy_setting(settings);
        return false;
    }
    
    /* Cargo parametros string */
    str = gui_files_get_string(settings, LANE_ATTRIBUTES, LANE_ATT_TYPE);
    if( str == NULL ){
        frogger_game_destroy_lane(lane);
        gui_files_destroy_setting(settings);
        return false;
    }
    lane->type = frogger_game_type_conv(str);
    
    str = gui_files_get_string(settings, LANE_ATTRIBUTES, LANE_ATT_ORIENTATION);
    if( str == NULL ){
        frogger_game_destroy_lane(lane);
        gui_files_destroy_setting(settings);
        return false;
    }
    lane->orientation = gui_animation_orientation_conv(str);
    
    /* Cierro el setting */
    gui_files_destroy_setting(settings);
    
    /* Creo la secuencia */
    positions = malloc( sizeof(POSITION) * lane->objectsQty );
    if(positions == NULL){
        frogger_game_destroy_lane(lane);
        return false;
    }
    if( !frogger_game_lane_sequence(lane->objectsQty, frogger_game_object_size(lane->type), lane->laneNumber, positions)){
        free(positions);
        frogger_game_destroy_lane(lane);
        return false;
    }
    
    /* Cargo objetos */
    for(i = 0;i < lane->objectsQty;i++){
            lane->objects[i] = frogger_game_create_object(positions[i], lane->speed, lane->orientation, lane->type);
        if( lane->objects[i] == NULL ){
            frogger_game_destroy_lane(lane);
            return false;
        }
    }
    
    /* Exito! */
    return true;    
}

/**********************/
/* LANE_FILE handlers */
/**********************/

/* frogger_game_create_cfg */
static CFG frogger_game_create_cfg(void){
    CFG cfg;
    
    /* Parametros default */
    cfg.init = false;
    cfg.file = NULL;
    
    /* Devuelvo */
    return cfg;
}

/* frogger_game_init_cfg */
static bool frogger_game_init_cfg(CFG* cfg, char* file){
    LENGTH length;
    
    if( cfg->init ){
        return false;
    }
    
    if( file == NULL ){
        return false;
    }
    
    /* Calculo cantidad de bytes */
    length = strlen(file) + 1;
    
    /* Reservo memoria */
    cfg->file = malloc( sizeof(char) * length );
    if( cfg->file == NULL ){
        return false;
    }
    
    /* Guardo contenido */
    strcpy( cfg->file, file );
    
    /* Inicializado! */
    cfg->init = true;
    
    /* Devuelvo exito */
    return true;
}

/* frogger_game_destroy_cfg */
static void frogger_game_destroy_cfg(CFG* cfg){
    /* Verifico inicializacion */
    if( cfg->init ){
        
        /* Libero la memoria */
        free(cfg->file);
    }
    cfg->init = false;
}

/************************************/
/* Definicion de funciones privadas */
/************************************/

/* frogger_game_type_conv */
static OBJECT_TYPES frogger_game_type_conv(char* str){
    if( !( strcmp(str, FROGGER_CFG_MOTORBIKE) ) ){
        return FROGGER_MOTORBIKE;
    }else if( !( strcmp(str, FROGGER_CFG_CAR) ) ){
        return FROGGER_CAR;
    }else if( !( strcmp(str, FROGGER_CFG_TRUCK) ) ){
        return FROGGER_TRUCK;
    }else if( !( strcmp(str, FROGGER_CFG_BOAT) ) ){
        return FROGGER_BOAT;
    }else if( !( strcmp(str, FROGGER_CFG_YACHT) ) ){
        return FROGGER_YACHT;
    }
    return FROGGER_NONE;
}

/* frogger_game_get_norm_pos */
static POSITION frogger_game_get_norm_pos(POSITION reference, POSITION obj){
    uint32_t step;
    POSITION newPos = {
        .y = reference.y
    };
    
    /* Cargo el desplazamiento */
#if PLATFORM_MODE == PC_ALLEGRO
    step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif
    
    /* Busco la pos */
    for(newPos.x = reference.x;newPos.x <= (obj.x + step);newPos.x += step){
        
        /* Verifico que es valida */
        if( obj.x < newPos.x ){
            newPos.x -= step;
            return newPos;
        }
    }
    
    newPos.x = obj.x;
    
    return newPos;
}

/* frogger_game_new_speed */
static SPEED frogger_game_new_speed(SPEED speed, uint32_t factor, uint32_t var){
    SPEED newSpeed;
    
    /* Valido caso critico */
    if( (speed.timeDelta - (factor*var)) < 0){
        return speed;
    }
    
    /* Fijo la distancia a recorrer */
    newSpeed.spaceDelta = speed.spaceDelta;
    
    /* Recalculo el tiempo */
    newSpeed.timeDelta = speed.timeDelta - (factor * var);
    
    return newSpeed;
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
static FROGGER_OBJECT frogger_game_create_object(POSITION pos, SPEED speed, GUI_ANIMATION_ORIENTATION orientation, OBJECT_TYPES type){
    FROGGER_OBJECT object;
    
#if PLATFORM_MODE == PC_ALLEGRO
    object = allegro_frogger_create_object(pos, frogger_game_speed_resolution(speed), orientation, type);
#elif PLATFORM_MODE == RPI
    
#endif
    
    return object;
}

/* frogger_game_object_size */
static uint32_t frogger_game_object_size(uint32_t type){
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

/* frogger_game_speed_resolution 
 * Recalcula velocidad con la resolucion elegida por la interfaz
 *
 * speed: Velocidad
 */
static SPEED frogger_game_speed_resolution(SPEED speed){
    SPEED newSpeed;
    uint32_t resolution;
    uint32_t step;
    
#if PLATFORM_MODE == PC_ALLEGRO
    step = ALLEGRO_DISPLAY_STEP;
    resolution = ALLEGRO_STEP_RESOLUTION;
#elif PLATFORM_MODE == RPI
#endif
    
    /* Redefino velocidad */
    speed.spaceDelta *= step;
    
    /* Redefino velocidad para mayor claridad en pantalla */
    newSpeed.spaceDelta = speed.spaceDelta / resolution;
    newSpeed.timeDelta = speed.timeDelta / resolution;
    
    return newSpeed;
}

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* frogger_game_new_level */
bool frogger_game_new_level(uint32_t level){
    uint32_t i, ii;
    POSITION* positions;
    uint32_t step;
    SPEED speed;
    
    /* Cargo el desplazamiento */
#if PLATFORM_MODE == PC_ALLEGRO
    step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif
    
    /* Me muevo entre los carriles */
    for(i = 0;i < field.lanesQty;i++){
        
        /* Creo en memoria lista con nuevas posicione */
        positions = malloc( sizeof(POSITION) * field.lanes[i].objectsQty);
        if( positions == NULL ){
            return false;
        }
        
        /* Cargo nuevas posiciones */
        if( !frogger_game_lane_sequence(field.lanes[i].objectsQty, frogger_game_object_size(field.lanes[i].type), field.lanes[i].laneNumber, positions)){
            free(positions);
            return false;
        }
        
        /* Nueva velocidad */
        speed = frogger_game_new_speed(field.lanes[i].speed, field.lanes[i].aFactor, level-1);
        
        /* Me muevo entre los objetos */
        for(ii = 0;ii < field.lanes[i].objectsQty;ii++){
            
            /* Cargo nueva velocidad */
            field.lanes[i].objects[ii]->speed = frogger_game_speed_resolution(speed);
                        
            /* Cargo nueva posicion */
            field.lanes[i].objects[ii]->pos = map_position(positions[ii].x * step, positions[ii].y * step);
        }
    }
    
    free(positions);
    
    return true;
}

/* frogger_game_has_won */
bool frogger_game_has_won(void){
    uint32_t step;
    POSITION pos;
    
    /* Cargo el desplazamiento */
#if PLATFORM_MODE == PC_ALLEGRO
    step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif
    
    /* Objeto posicion */
    pos = map_position(frog.object->pos.x / step, frog.object->pos.y / step);
    if( pos.y == 0 ){
        return true;
    }
    return false;    
}

/* frogger_game_is_frog_static */
bool frogger_game_is_frog_static(void){
    if( frog.object->status == GUI_ANIMATION_STATE_STATIC ){
        return true;
    }else if( frog.object->status == GUI_ANIMATION_STATE_STATIC_MOVE ){
        return true;
    }
    return false;
}

/* frogger_game_is_water_region */
bool frogger_game_is_water_region(void){
    uint32_t step;
    
    /* Cargo el desplazamiento */
#if PLATFORM_MODE == PC_ALLEGRO
    step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif
        
   /* Me fijo si esta en region */
    if( gui_animation_in_region( map_position(frog.object->pos.x / step, frog.object->pos.y / step) , REGION_WATER) ){
        return true;
    }
    return false;    
}

/* frogger_game_is_street_region */
bool frogger_game_is_street_region(void){
    uint32_t step;
    
    /* Cargo el desplazamiento */
#if PLATFORM_MODE == PC_ALLEGRO
    step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif
        
   /* Me fijo si esta en region */
    if( gui_animation_in_region( map_position(frog.object->pos.x / step, frog.object->pos.y / step) , REGION_STREET) ){
        return true;
    }
    return false;
}

/* frogger_game_transport_on */
void frogger_game_is_transport(void){
    LANE lane;
    FROGGER_OBJECT object;
    
    uint32_t i, ii, step;
    bool found = false;
    
    /* Cargo el desplazamiento */
#if PLATFORM_MODE == PC_ALLEGRO
    step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif 
    
    /* Me fijo que este en region de agua */
    if( frogger_game_is_water_region() ){
        
        /* Que este quieto... */
        if( frog.object->status == GUI_ANIMATION_STATE_STATIC ){
                
            /* Veo cada carril en busca de transportes */
            for(i = 0;i < field.lanesQty && !found;i++){
                lane = field.lanes[i];
                
                /* Que sea transporte el carril...  */
                if( lane.type == FROGGER_BOAT || lane.type == FROGGER_YACHT ){

                    /* Busco entre sus transportes */
                    for(ii = 0;ii < lane.objectsQty && !found;ii++){/* Calculo esquina */
                        object = lane.objects[ii];
                        
                        /* Me fijo si esta en el */
                        if( gui_animation_collision(frog.object, object) ){
                            if( frog.transport == NULL ){
                                /* Modo transporte */
                                frog.transport = object;
                                found = true;
                                /* Normalizo la posicion */
                            }
                        }else{
                            if( frog.transport != NULL ){
                                /* Salgo de modo transporte? */
                                if( frog.transport == object ){
                                    frog.transport = NULL;
                                    found = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

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
    if( frogger_game_is_water_region() ){
        
        /* Me fijo que este siendo transportado */
        if( frog.transport != NULL ){
            
            /* Me fijo si esta quieto */
            if( frog.object->status == GUI_ANIMATION_STATE_STATIC ){
                
                /* Configuro parametros de movimiento */
                if( gui_animation_get_orientation(frog.transport) == GUI_HORIZONTAL_LEFT ){
                    x = frog.object->pos.x - step;
                }
                if( gui_animation_get_orientation(frog.transport) == GUI_HORIZONTAL_RIGHT ){
                    x = frog.object->pos.x + step;
                }
                
                /* Valido posicion */
                if( x >= 0  && x <= (DISPLAY_DIVISIONS_X - 1)*step ){
                
                    /* Configuro velocidad */
                    frog.object->speed = frog.transport->speed;

                    /* Configuro parametros de movimiento */
                    if( gui_animation_get_orientation(frog.transport) == GUI_HORIZONTAL_LEFT ){
                        gui_animation_set_animation(frog.object, FROG_LEFT);
                    }
                    if( gui_animation_get_orientation(frog.transport) == GUI_HORIZONTAL_RIGHT ){
                        gui_animation_set_animation(frog.object, FROG_RIGHT);
                    }
                    
                    /* Inicio el movimiento */
                    gui_animation_start_static_movement(frog.object, step);     
                } 
            }
        }
    }
}

/* frogger_game_drown */
bool frogger_game_drown(void){
    
    /* Me fijo que este en region de agua */
    if( frogger_game_is_water_region() ){
        
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
    frog.object->pos = map_position(DEFAULT_FROG_X * step, DEFAULT_FROG_Y * step);
    gui_animation_set_animation(frog.object, DEFAULT_FROG_ANIMATION);
    
}

/* frogger_game_collisions */
bool frogger_game_collisions(void){
    uint32_t i, ii;
    uint32_t step;
    
    LANE lane;
    FROGGER_OBJECT object;
        
    /* Cargo el desplazamiento */
#if PLATFORM_MODE == PC_ALLEGRO
        step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif   

    /* Me muevo por los carriles */
    for(i = 0;i < field.lanesQty;i++){
        lane = field.lanes[i];
        
        /* Me muevo por los objetos */
        for(ii = 0;ii < field.lanes[i].objectsQty;ii++){
            object = lane.objects[ii];
            
            /* Me fijo si hay colision */
            if( lane.type != FROGGER_YACHT && lane.type != FROGGER_BOAT ){
                
                if( gui_animation_collision(frog.object, object)){
                    return true;
                }
            }
        }
    }
    
    return false;    
}

/* frogger_game_move_frog */
bool frogger_game_move_frog(uint16_t input){
    uint32_t step;
    
    /* Verifico que este quieta */
    if( frog.object->status == GUI_ANIMATION_STATE_STATIC || frog.object->status == GUI_ANIMATION_STATE_STATIC_MOVE ){
        
    /* Me fijo si el desplazamiento es valido */
#if PLATFORM_MODE == PC_ALLEGRO
        if( !allegro_frogger_movement_valid(input) ){
            return false;
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
                gui_animation_start_movement(frog.object, FROG_UP, step);
                break;
            case MOVE_DOWN:
                gui_animation_start_movement(frog.object, FROG_DOWN, step);
                break;
            case MOVE_LEFT:
                gui_animation_start_movement(frog.object, FROG_LEFT, step);
                break;
            case MOVE_RIGHT:
                gui_animation_start_movement(frog.object, FROG_RIGHT, step);
                break;
        }
        
        /* Finalizo tranporte */
        if( frog.transport != NULL ){
            frog.transport = NULL;
            if( !gui_animation_reload_object(frog.object) ){
                return false;
            }
        }
    }
        
    return true;
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
    /* Libero motor de animaciones */
    if( engine != NULL ){
        gui_animation_destroy_engine(engine);
    }
    
    if( frog.object != NULL ){
        gui_animation_destroy_object(frog.object);
    }
    
    /* Destruyo objetos de campo */
    frogger_game_destroy_field(&field);
}

/* frogger_game_frog_init */
bool frogger_game_frog_init(void){  
    /* Inicializo la rana de cada interfaz */
#if PLATFORM_MODE == PC_ALLEGRO
    if( !allegro_frogger_frog_init() ){
        return false;
    }
#elif PLATFORM_MODE == RPI
#endif
    
    return true;
}

/* frogger_game_init */
bool frogger_game_init(void){
    uint32_t i, ii;
    uint32_t step;
        
    /* Cargo el desplazamiento */
#if PLATFORM_MODE == PC_ALLEGRO
        step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif   
    
    /* Inicializo los carriles */
    if( !frogger_game_load_field(&field, FROGGER_FIELD_CFG) ){
        return false;
    }
    
    /* Inicializo la rana */
    if( !frogger_game_frog_init() ){
        return false;
    }
    
    /* Creo el motor de animaciones */
    engine = gui_animation_create_engine();
    if( engine == NULL ){
        gui_animation_destroy_object(frog.object);
        return false;
    }
    
    /* Vinculo objetos con el motor */
    if( !gui_animation_attach_engine(engine, frog.object) ){
        gui_animation_destroy_object(frog.object);
        gui_animation_destroy_engine(engine);
        return false;
    }
    
    /* Vinculos los objetos */
    for(i = 0;i < field.lanesQty;i++){
        for(ii = 0;ii < field.lanes[i].objectsQty;ii++){
            if( !gui_animation_attach_engine(engine, field.lanes[i].objects[ii]) ){
                gui_animation_destroy_object(frog.object);
                frogger_game_destroy_field(&field);
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
    
    LANE lane;
    FROGGER_OBJECT object;
    
#if PLATFORM_MODE == PC_ALLEGRO
    step = ALLEGRO_DISPLAY_STEP;
#elif PLATFORM_MODE == RPI
#endif
    
    /* Itero los carriles */
    for(i = 0;i < field.lanesQty;i++){
        lane = field.lanes[i];
        
        /* Itero los objetos de cada carril */
        for(ii = 0;ii < lane.objectsQty;ii++){
            object = lane.objects[ii];
                    
            /* Me fijo su estado de animacion */
            if( object->status == GUI_ANIMATION_STATE_STATIC ){
                
                /* Nueva posicion x */
                if( lane.orientation == GUI_HORIZONTAL_LEFT ){
                    x = (object->pos.x / step) - 1;
                }else if( lane.orientation == GUI_HORIZONTAL_RIGHT ){
                    x = (object->pos.x / step) + 1;
                }
                
                /* Verifico los limites */
                if( x < MAP_X_MIN ){
                    object->pos.x = MAP_X_MAX * step;
                }else if( x > MAP_X_MAX ){
                    object->pos.x = MAP_X_MIN * step;
                }
                
                /* Si esta quieto, lo desplazo */
                gui_animation_start_static_movement(object, step);
            }
        }
    }
}
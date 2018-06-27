#include "gui_animation.h"
#include "../gui_files/gui_files.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

/*****************************/
/* ANIMATION_ENGINE handlers */
/*****************************/

/* gui_animation_engine_thread
 * Thread handler del motor de animaciones
 *
 * thisEngine: Motor para manejar
 */
static void* gui_animation_engine_thread(void* thisEngine);

/******************/
/* FRAME handlers */
/******************/

/* gui_animation_create_frame
 * Crea un frame default
 */
static FRAME gui_animation_create_frame(void);

/* gui_animation_init_frame
 * Inicializa la memoria que ocupa el frame
 *
 * frame: Instancia a inicializar
 * filename: Nombre del archivo
 */
static bool gui_animation_init_frame(FRAME* frame, char* filename);

/* gui_animation_destroy_frame
 * Destruye la instancia frame
 *
 * frame: Instancia
 */
static void gui_animation_destroy_frame(FRAME* frame);

/**********************/
/* ANIMATION handlers */
/**********************/

/* gui_animation_create_animation
 * Crea la animacion con valores default
 */
static ANIMATION gui_animation_create_animation(void);

/* gui_animation_init_animation
 * Incializa la memoria a ocupar por la animacion
 *
 * animation: Instancia para inicializar
 * id: Identificador
 * qty: Cantidad de frames
 */
static bool gui_animation_init_animation(ANIMATION* animation, ANIMATION_ID id, LENGTH qty);

/* gui_animation_destroy_animation
 * Destruye la instancia
 *
 * animation: Instancia
 */
static void gui_animation_destroy_animation(ANIMATION* animation);

/* gui_animation_load_animation
 * Carga en memoria una animacion
 *
 * animation: Animacion a cargar
 * filename: Archivo con la config. de animacion
 */
static bool gui_animation_load_animation(ANIMATION* animation, char* filename);

/****************************/
/* ANIMATED_OBJECT handlers */
/****************************/

/* gui_animation_start_one_time */
void gui_animation_start_one_time(ANIMATED_OBJECT* object, ANIMATION_ID id){

    /* Seteo la animacion */
    gui_animation_set_animation(object, id);

    /* Configuro el modo */
    object->status = GUI_ANIMATION_STATE_ONE_TIME;
}

/* gui_animation_create_object
 * Crea un objeto con valores default
 *
 * pos: Posicion inicial
 */
static OBJECT_POINTER gui_animation_create_object(POSITION pos);

/* gui_animation_init_object
 * Inicializa la memoria de la instancia
 *
 * object: Instancia
 * qty: Cantidad de animaciones
 * id: Animacion inicial
 */
static bool gui_animation_init_object(OBJECT_POINTER object, LENGTH qty, ANIMATION_ID id);

/* gui_animation_move_object
 * Mueve la posicion del objeto
 *
 * object: Instancia del objeto
 */
static void gui_animation_move_object(OBJECT_POINTER object);

/* gui_animation_nextframe_object
 * Mueve el frame del objeto
 *
 * object: Instancia del objeto
 */
static uint32_t gui_animation_nextframe_object(OBJECT_POINTER object);

/****************************/
/* ANIMATED_OBJECT handlers */
/****************************/

/* gui_animation_reload_object */
bool gui_animation_reload_object(ANIMATED_OBJECT* object){
    SETTING* settings;

    /* Valido */
    if( object->file == NULL ){
        return false;
    }

    /* Cargo la configuracion del objeto */
    settings = gui_files_load_setting(object->file);
    if( settings == NULL ){
        return false;
    }

    /* Cargo los parametros basicos */
    if( !gui_files_get_int(settings, OBJFILE_ATTRIBUTES, OBJFILE_ATT_WIDTH, &object->width) ){
        gui_files_destroy_setting(settings);
        return false;
    }
    if( !gui_files_get_int(settings, OBJFILE_ATTRIBUTES, OBJFILE_ATT_HEIGHT, &object->height) ){
        gui_files_destroy_setting(settings);
        return false;
    }
    if( !gui_files_get_int(settings, OBJFILE_ATTRIBUTES, OBJFILE_ATT_TIME, &object->speed.timeDelta) ){
        gui_files_destroy_setting(settings);
        return false;
    }
    if( !gui_files_get_int(settings, OBJFILE_ATTRIBUTES, OBJFILE_ATT_SPACE, &object->speed.spaceDelta) ){
        gui_files_destroy_setting(settings);
        return false;
    }

    /* Exito */
    gui_files_destroy_setting(settings);
    return true;
}

/* gui_animation_get_frame_length */
LENGTH gui_animation_get_frame_length(ANIMATED_OBJECT* object, ANIMATION_ID id){
    ANIMATION animation;
    LENGTH i;

    /* Busco la animacion */
    for(i = 0;i < object->animQty;i++){
        animation = object->animations[i];

        /* La encuentro por el id */
        if( !( strcmp(id, animation.id) ) ){

            return animation.framesQty;
        }
    }

    return 0;

}

/* gui_animation_get_orientation */
GUI_ANIMATION_ORIENTATION gui_animation_get_orientation(ANIMATED_OBJECT* object){
    LENGTH i;
    ANIMATION animation;

    /* Busco entre las animaciones */
    for(i = 0;i < object->animQty;i++){
        animation = object->animations[i];

        /* Encuentro la animacion */
        if( gui_animation_is_animation(object, animation.id) ){

            /* Devuelvo orientacion */
            return animation.orientation;
        }
    }
}

/* gui_animation_seek_animation */
ANIMATION_ID gui_animation_seek_animation(ANIMATED_OBJECT* object, GUI_ANIMATION_ORIENTATION or){
    LENGTH i;
    ANIMATION animation;

    /* Busco entre las animaciones */
    for(i = 0;i < object->animQty;i++){
        animation = object->animations[i];

        /* Encuentro la orientacion */
        if( animation.orientation == or ){
            return animation.id;
        }
    }
    return NULL;
}

/* gui_animation_set_animation */
void gui_animation_set_animation(ANIMATED_OBJECT* object, ANIMATION_ID id){
    strcpy(object->currentAnimation, id);
}


/* gui_animation_is_animation */
bool gui_animation_is_animation(ANIMATED_OBJECT* object, ANIMATION_ID id){
    if( !strcmp(object->currentAnimation, id) ){
        return true;
    }
    return false;
}

/* gui_animation_get_frame */
char* gui_animation_get_frame(ANIMATED_OBJECT* object){
    uint16_t i;
    char* file = NULL;

    /* Busco animacion correspondiente */
    for(i = 0;i < object->animQty;i++){

        if( !( strcmp(object->currentAnimation, object->animations[i].id) ) ){
            file = object->animations[i].frames[object->frameIndex].file;
        }
    }

    return file;
}

/* gui_animation_move_object */
static void gui_animation_move_object(OBJECT_POINTER object){
    LENGTH i;
    uint32_t dist;

    /* Calculo desplazamiento */
    if( object->status == GUI_ANIMATION_STATE_MOVE ){
        dist = (object->speed.spaceDelta / gui_animation_get_frame_length(object, object->currentAnimation));
    }else{
        dist = object->speed.spaceDelta;
    }

    /* Busco la animacion */
    for(i = 0;i < object->animQty;i++){

        if( !( strcmp(object->currentAnimation, object->animations[i].id) ) ){

            switch( object->animations[i].orientation ){
                case GUI_VERTICAL_UP:
                    object->pos.y -= dist;
                    break;
                case GUI_VERTICAL_DOWN:
                    object->pos.y += dist;
                    break;
                case GUI_HORIZONTAL_LEFT:
                    object->pos.x -= dist;
                    break;
                case GUI_HORIZONTAL_RIGHT:
                    object->pos.x += dist;
                    break;
                case GUI_ORIENTATION_NULL:
                    return;
                    break;
            }
        }
    }
}

/* gui_animation_nextframe_object */
static uint32_t gui_animation_nextframe_object(OBJECT_POINTER object){
    LENGTH i;

    /* Busco la animacion */
    for(i = 0;i < object->animQty;i++){

        if( !( strcmp(object->currentAnimation, object->animations[i].id) ) ){
            object->frameIndex++;
            if( object->frameIndex >= object->animations[i].framesQty ){
                object->frameIndex = 0;
            }
            return object->frameIndex;
        }
    }
}

/* gui_animation_create_object */
static OBJECT_POINTER gui_animation_create_object(POSITION pos){
    OBJECT_POINTER object;

    /* Reservo memoria para el object */
    object = malloc( sizeof(ANIMATED_OBJECT) );
    if( object == NULL ){
        return NULL;
    }

    /* Inicializacion */
    pthread_mutex_init(&object->objectMutex, NULL);

    object->pos = pos;
    object->status = GUI_ANIMATION_STATE_STATIC;
    object->frameIndex = 0;
    object->distance = 0;
    object->timeCounter = 0;
    object->currentAnimation = NULL;
    object->animations = NULL;
    object->file = NULL;
    object->animQty = 0;
    object->init = false;

    /* Devuelvo */
    return object;
}

/* gui_animation_init_object */
static bool gui_animation_init_object(OBJECT_POINTER object, LENGTH qty, ANIMATION_ID id){

    /* Reservo memoria */
    object->animations = malloc( sizeof(ANIMATION) * qty );
    if( object->animations == NULL ){
        return false;
    }

    /* Reservo memoria para id */
    object->currentAnimation = malloc( sizeof(char) * ANIMATION_ID_MAX_LENGTH );
    if( object->currentAnimation == NULL ){
        free( object->animations );
        return false;
    }

    /* Reservo memoria para file name */
    object->file = malloc( sizeof(char) * ANIMATION_ID_MAX_LENGTH );
    if( object->file == NULL ){
        free( object->currentAnimation );
        free( object->animations );
        return false;
    }

    /* Inicializado!! */
    strcpy(object->currentAnimation, id);
    object->animQty = qty;
    object->init = true;

    /* Exito!! */
    return true;
}

/* gui_animation_destroy_object * object: Instancia */
void gui_animation_destroy_object(OBJECT_POINTER object){
    LENGTH i;
    if( object ){
        /* Verifico inicializacion */
        if( object->init ){

            /* Destruyo cada instancia animacion */
            for(i = 0;i < object->animQty;i++){
                gui_animation_destroy_animation(&object->animations[i]);
            }

            /* Libero memoria de arreglo */
            free( object->animations );
        }

        /* Destruyo mutex */
        pthread_mutex_destroy(&object->objectMutex);

        /* Libero id */
        free( object->currentAnimation );

        /* Libero filename */
        free( object->file );

        /* Libero memoria del objeto */
        free( object );
    }
}

/* gui_animation_load_object */
OBJECT_POINTER gui_animation_load_object(char* filename, POSITION pos, ANIMATION_ID id){
    OBJECT_POINTER object;
    SETTING* settings;
    LENGTH length;
    char* str, i;
    char ai[10];

    /* Creo el objeto por primera vez */
    object = gui_animation_create_object(pos);
    if( object == NULL ){
        return NULL;
    }

    /* Cargo la configuracion del objeto */
    settings = gui_files_load_setting(filename);
    if( settings == NULL ){
        gui_animation_destroy_object(object);
        return NULL;
    }

    /* Busco cantidad de animaciones */
    length = gui_files_get_section_length(settings, OBJFILE_ANIMATIONS);
    if( !length ){
        gui_animation_destroy_object(object);
        gui_files_destroy_setting(settings);
        return NULL;
    }

    /* Inicializo el objeto */
    if( !gui_animation_init_object(object, length, id) ){
        gui_animation_destroy_object(object);
        gui_files_destroy_setting(settings);
        return NULL;
    }

    /* Cargo el filename */
    strcpy(object->file, filename);

    /* Cargo los parametros basicos */
    if( !gui_files_get_int(settings, OBJFILE_ATTRIBUTES, OBJFILE_ATT_WIDTH, &object->width) ){
        gui_animation_destroy_object(object);
        gui_files_destroy_setting(settings);
        return NULL;
    }
    if( !gui_files_get_int(settings, OBJFILE_ATTRIBUTES, OBJFILE_ATT_HEIGHT, &object->height) ){
        gui_animation_destroy_object(object);
        gui_files_destroy_setting(settings);
        return NULL;
    }
    if( !gui_files_get_int(settings, OBJFILE_ATTRIBUTES, OBJFILE_ATT_TIME, &object->speed.timeDelta) ){
        gui_animation_destroy_object(object);
        gui_files_destroy_setting(settings);
        return NULL;
    }
    if( !gui_files_get_int(settings, OBJFILE_ATTRIBUTES, OBJFILE_ATT_SPACE, &object->speed.spaceDelta) ){
        gui_animation_destroy_object(object);
        gui_files_destroy_setting(settings);
        return NULL;
    }

    /* Cargo cada animacion */
    for(i = 0;i < length;i++){
        /* Creo el indice string */
        sprintf(ai, "%d", i);

        /* Busco el nombre de archivo */
        str = gui_files_get_string(settings, OBJFILE_ANIMATIONS, ai);
        if( str == NULL ){
            gui_animation_destroy_object(object);
            gui_files_destroy_setting(settings);
            return NULL;
        }

        /* Cargo la animacion */
        if( !gui_animation_load_animation(&object->animations[i], str) ){
            gui_animation_destroy_object(object);
            gui_files_destroy_setting(settings);
            return NULL;
        }
    }

    /* Exito !! */
    gui_files_destroy_setting(settings);
    return object;
}

/**********************/
/* ANIMATION handlers */
/**********************/

/* gui_animation_load_animation */
static bool gui_animation_load_animation(ANIMATION* animation, char* filename){
    SETTING* settings;
    LENGTH length, i;
    char* str;
    char fi[5];

    /* Creo la instancia */
    *animation = gui_animation_create_animation();

    /* Cargo las configuraciones */
    settings = gui_files_load_setting(filename);
    if( settings == NULL ){
        return false;
    }

    /* Cargo la orientacion */
    str = gui_files_get_string(settings, ANIMATION_ATTRIBUTES, ANIMATION_ATT_ORIENTATION);
    if( str == NULL ){
        gui_files_destroy_setting(settings);
        return false;
    }

    /* Guardo la orientacion */
    animation->orientation = gui_animation_orientation_conv(str);

    /* Calculo cantidad de frames */
    length = gui_files_get_section_length(settings, ANIMATION_FRAMES);
    if( !length ){
        gui_files_destroy_setting(settings);
        return false;
    }

    /* Busco el animation id */
    str = gui_files_get_string(settings, ANIMATION_ATTRIBUTES, ANIMATION_ATT_ID);
    if( str == NULL ){
        gui_files_destroy_setting(settings);
        return false;
    }

    /* Inicializo la animacion */
    if( !gui_animation_init_animation(animation, str, length) ){
        gui_files_destroy_setting(settings);
        return false;
    }

    /* Inicializo cada frame */
    for(i = 0;i < length;i++){
        /* Creo el index string */
        sprintf(fi, "%d", i);

        /* Busco el frame */
        str = gui_files_get_string(settings, ANIMATION_FRAMES, fi);
        if( str == NULL ){
            gui_animation_destroy_animation(animation);
            gui_files_destroy_setting(settings);
            return false;
        }

        /* Inicializo */
        if( !gui_animation_init_frame(&animation->frames[i], str) ){
            gui_animation_destroy_animation(animation);
            gui_files_destroy_setting(settings);
            return false;
        }
    }

    /* Exito! */
    gui_files_destroy_setting(settings);
    return true;
}

/* gui_animation_create_animation */
static ANIMATION gui_animation_create_animation(void){
    ANIMATION animation;

    /* Inicializo por defecto */
    animation.init = false;
    animation.framesQty = 0;
    animation.orientation = GUI_ORIENTATION_NULL;
    animation.id = NULL;
    animation.frames = NULL;

    /* Devuelvo */
    return animation;
}

/* gui_animation_init_animation */
static bool gui_animation_init_animation(ANIMATION* animation, ANIMATION_ID id, LENGTH qty){
    LENGTH length, i;

    if( animation->init ){
        return false;
    }

    /* Calculo bytes para id */
    length = strlen(id) + 1;

    /* Reservo memoria para id */
    animation->id = malloc( sizeof(char) * length );
    if( animation->id == NULL ){
        return false;
    }

    /* Guardo el id */
    strcpy(animation->id, id);

    /* Reservo memoria para frames */
    animation->frames = malloc( sizeof(FRAME) * qty );
    if( animation-> frames == NULL ){
        free( animation->id );
        return false;
    }

    /* Inicializado!! */
    animation->framesQty = qty;
    animation->init = true;

    for(i = 0;i < animation->framesQty;i++){
        animation->frames[i] = gui_animation_create_frame();
    }

    /* Exito! */
    return true;
}

/* gui_animation_destroy_animation */
static void gui_animation_destroy_animation(ANIMATION* animation){
    LENGTH i;
    if( animation ){
        /* Verifico inicializacion */
        if( animation->init ){

            /* Libero memoria del id */
            free( animation->id );

            /* Destruyo las intancias de frames */
            for(i = 0;i < animation->framesQty;i++){
                gui_animation_destroy_frame(&animation->frames[i]);
            }

            /* Libero memoria de arreglo frame */
            free( animation->frames );
        }
        animation->init = false;
    }
}



/******************/
/* FRAME handlers */
/******************/

/* gui_animation_create_frame */
static FRAME gui_animation_create_frame(void){
    FRAME frame;

    /* Inicializo por defecto */
    frame.init = false;
    frame.file = NULL;

    /* La devuelvo */
    return frame;
}

/* gui_animation_init_frame */
static bool gui_animation_init_frame(FRAME* frame, char* filename){
    LENGTH length;

    if( frame->init ){
        return false;
    }

    if( filename == NULL ){
        return false;
    }

    /* Calculo cantidad de bytes */
    length = strlen(filename) + 1;

    /* Reservo memoria */
    frame->file = malloc( sizeof(char) * length );
    if( frame->file == NULL ){
        return false;
    }

    /* Copio contenido */
    strcpy(frame->file, filename);

    /* Inicializado!! */
    frame->init = true;

    /* Exito! */
    return true;
}

/* gui_animation_destroy_frame */
static void gui_animation_destroy_frame(FRAME* frame){
    if( frame ){
        /* Verifico inicializacion */
        if( frame->init ){

            /* Libero memoria */
            if( frame->file != NULL ){
                free( frame->file );
            }
        }
        frame->init = false;
    }
}

/*****************************/
/* ANIMATION_ENGINE handlers */
/*****************************/

/* gui_animation_engine_thread */
static void* gui_animation_engine_thread(void* thisEngine){
    ANIMATION_ENGINE* engine = thisEngine;
    ANIMATED_OBJECT* object;
    uint32_t timeMax;

    uint32_t i;

    /* Me fijo si esta habilitado el motor de animaciones */
    while( !engine->shutdown ){

        if( !engine->pause ){
            /* Retardo temporal de 1mS */
            usleep(1000);

            /* Controlo los objetos del motor */
            for(i = 0;i < engine->length;i++){
                /* Cargo el objeto */
                object = engine->objects[i];

                /* Me fijo el estado del objeto */
                if( object->status != GUI_ANIMATION_STATE_STATIC ){
                    /* Incremento contador de tiempo */
                    object->timeCounter++;

                    /* Get time max */
                    if( object->status == GUI_ANIMATION_STATE_MOVE ){
                        timeMax = object->speed.timeDelta / gui_animation_get_frame_length(object, object->currentAnimation);
                    }else{
                        timeMax = object->speed.timeDelta;
                    }
                    /* Me fijo si paso el tiempo */
                    if( object->timeCounter >= timeMax ){

                        /* Reinicio el tiempo */
                        object->timeCounter = 0;

                        /* Manejo animacion */
                        switch( object->status ){
                            case GUI_ANIMATION_STATE_STATIC_MOVE:
                                /* Me desplazo */
                                gui_animation_move_object(object);
                                break;
                            case GUI_ANIMATION_STATE_MOVE:
                                /* Cambio el frame */
                                gui_animation_nextframe_object(object);

                                /* Me desplazo */
                                gui_animation_move_object(object);
                                break;
                            case GUI_ANIMATION_STATE_LOOP:
                                /* Cambio el frame */
                                gui_animation_nextframe_object(object);
                                break;
                            case GUI_ANIMATION_STATE_ONE_TIME:
                                /* Cambio el frame */
                                if( !gui_animation_nextframe_object(object) ){
                                    object->status = GUI_ANIMATION_STATE_STATIC;
                                }
                                break;
                        }

                        /* Me fijo si ya llego */
                        if( object->status == GUI_ANIMATION_STATE_STATIC_MOVE || object->status == GUI_ANIMATION_STATE_MOVE ){
                            object->distance -= (object->speed.spaceDelta / gui_animation_get_frame_length(object, object->currentAnimation));
                            if( object->distance <= 0 ){
                                object->status = GUI_ANIMATION_STATE_STATIC;
                                object->frameIndex = 0;
                            }
                        }
                    }
                }
            }
        }
    }
}

/* gui_animation_create_engine */
ANIMATION_ENGINE* gui_animation_create_engine(void){
    ANIMATION_ENGINE* engine;

    /* Reservo memoria para el motor */
    engine = malloc( sizeof(ANIMATION_ENGINE) );
    if( engine == NULL ){
        return NULL;
    }

    /* Reservo memoria para la lista de objetos */
    engine->objects = malloc( sizeof(OBJECT_POINTER) );
    if( engine->objects == NULL ){
        free(engine);
        return NULL;
    }

    /* Inicializo atributos */
    engine->shutdown = false;
    engine->pause = false;
    engine->length = 0;

    /* Creo el mutex */
    pthread_mutex_init(&engine->engineMutex, NULL);

    return engine;

}

/* gui_animation_destroy_engine */
void gui_animation_destroy_engine(ANIMATION_ENGINE* engine){

    /* Apago el thread */
    engine->shutdown = true;
    pthread_join(engine->engineThread, NULL);

    /* Libero la memoria de la lista de objetos */
    if( engine->objects != NULL ){
        free(engine->objects);
    }

    /* Destruyo el mutex */
    pthread_mutex_destroy(&engine->engineMutex);

    /* Liboer la memoria del motor */
    free(engine);
}

/* gui_animation_pause_engine */
void gui_animation_pause_engine(ANIMATION_ENGINE* engine){
    pthread_mutex_lock(&engine->engineMutex);
    if( !engine->pause ){
        engine->pause = true;
    }
    pthread_mutex_unlock(&engine->engineMutex);
}

/* gui_animation_continue_engine */
void gui_animation_continue_engine(ANIMATION_ENGINE* engine){
    pthread_mutex_lock(&engine->engineMutex);
    if( engine->pause ){
        engine->pause = false;
    }
    pthread_mutex_unlock(&engine->engineMutex);
}

/* gui_animation_start_engine */
void gui_animation_start_engine(ANIMATION_ENGINE* engine){

    if( !engine->pause ){
        /* Habilito el thread */
        engine->shutdown = false;

        /* Inicio el thread */
        pthread_create(&(engine->engineThread), NULL, gui_animation_engine_thread, engine);
    }
}

/* gui_animation_attach_engine */
bool gui_animation_attach_engine(ANIMATION_ENGINE* engine, ANIMATED_OBJECT* object){

    /* Reservo memoria para el objeto */
    engine->objects = realloc(engine->objects, sizeof(OBJECT_POINTER) * (engine->length+1));
    if( engine->objects == NULL ){
        return false;
    }

    /* Vinculo el objeto con el motor */
    engine->objects[engine->length] = object;

    /* Incremento el largo de la lista de objetos */
    engine->length++;

    /* Proceso exitoso */
    return true;
}

/* gui_animation_start_static_movement */
void gui_animation_start_static_movement(ANIMATED_OBJECT* object, int32_t distance){
    /* Configuro posicion final */
    object->distance = distance;

    /* Configuro estado */
    object->status = GUI_ANIMATION_STATE_STATIC_MOVE;
}

/* gui_animation_start_movement */
void gui_animation_start_movement(ANIMATED_OBJECT* object, ANIMATION_ID id, int32_t distance){
    /* Configuro la posicion final de movimiento */
    object->distance = distance;

    /* Configuro la animacion del objeto */
    strcpy(object->currentAnimation, id);

    /* Configuro estado movimiento */
    object->status = GUI_ANIMATION_STATE_MOVE;
}

/* gui_animation_stop_movement */
void gui_animation_stop_movement(ANIMATED_OBJECT* object){
    /* Reseteo el frame */
    object->frameIndex = 0;

    /* Configuro */
    object->status = GUI_ANIMATION_STATE_STATIC;
}

/* gui_animation_stop_loop */
void gui_animation_stop_loop(ANIMATED_OBJECT* object){
    /* Reseteo el frame */
    object->frameIndex = 0;

    /* Desactivo estado loop */
    object->status = GUI_ANIMATION_STATE_STATIC;
}

/* gui_animation_start_loop */
void gui_animation_start_loop(ANIMATED_OBJECT* object, ANIMATION_ID id){
    /* Configuro la animacion del objeto */
    strcpy(object->currentAnimation, id);

    /* Pongo en modo bucle */
    object->status = GUI_ANIMATION_STATE_LOOP;
}

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* gui_animation_collision */
bool gui_animation_collision(ANIMATED_OBJECT* objA, ANIMATED_OBJECT* objB){
    REGION regionA, regionB;
    uint32_t x, y;
    bool collision;

    /* Creo la region A */
    x = objA->pos.x + objA->width - 1;
    y = objA->pos.y + objA->height - 1;
    regionA = map_region( objA->pos, map_position(x, y) );

    /* Creo la region B */
    x = objB->pos.x + objB->width - 1;
    y = objB->pos.y + objB->height - 1;
    regionB = map_region( objB->pos, map_position(x, y) );

    /* Me fijo colision */
    collision = gui_animation_region_collision(regionA, regionB);
    if( !collision ){
        collision = gui_animation_region_collision(regionB, regionA);
    }

    /* Me fijo si hubo colision */
    return collision;
}

/* gui_animation_map_collision */
bool gui_animation_map_collision(REGION a, ANIMATED_OBJECT* obj){
    REGION region = map_region(obj->pos, map_position(obj->pos.x+obj->width, obj->pos.y+obj->height));

    return gui_animation_region_collision(a, region);
}

/* gui_animation_region_collision */
bool gui_animation_region_collision(REGION a, REGION b){
    bool x_overlap, y_overlap, region_overlap;
    /* Veo si x se solapa */
    x_overlap = (a.iCorner.x < b.fCorner.x) && (a.fCorner.x > b.iCorner.x);

    /* Veo si x se solapa */
    y_overlap = (a.iCorner.y < b.fCorner.y) && (a.fCorner.y > b.iCorner.y);

    /* Ambas se solapan*/
    region_overlap = x_overlap && y_overlap;

    return region_overlap;
}

/* map_region */
REGION map_region(POSITION iCorner, POSITION fCorner){
    REGION region = {
        .iCorner = iCorner,
        .fCorner = fCorner
    };

    return region;
}

/* map_speed */
SPEED map_speed(uint32_t timeDelta, uint32_t spaceDelta){
    SPEED speed = {
        .timeDelta = timeDelta,
        .spaceDelta = spaceDelta
    };

    return speed;
}

/* gui_animation_in_region */
bool gui_animation_in_region(POSITION position, REGION region){
    if( position.x >= region.iCorner.x && position.x <= region.fCorner.x ){
        if( position.y >= region.iCorner.y && position.y <= region.fCorner.y ){
            return true;
        }
    }
    return false;
}

/* gui_animation_orientation_conv */
GUI_ANIMATION_ORIENTATION gui_animation_orientation_conv(char* str){
    if( !( strcmp(str, ANIMATION_HORIZONTAL_LEFT) ) ){
        return GUI_HORIZONTAL_LEFT;
    }else if( !( strcmp(str, ANIMATION_HORIZONTAL_RIGHT) ) ){
        return GUI_HORIZONTAL_RIGHT;
    }else if( !( strcmp(str, ANIMATION_VERTICAL_UP) ) ){
        return GUI_VERTICAL_UP;
    }else if( !( strcmp(str, ANIMATION_VERTICAL_DOWN) ) ){
        return GUI_VERTICAL_DOWN;
    }else{
        return GUI_ORIENTATION_NULL;
    }
}

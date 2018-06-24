#include "gui_animation.h"
#include "../gui_files/gui_files.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

/***********************************/
/* Prototipo de funciones privadas */
/***********************************/

/* gui_animation_engine_thread
 * Thread handler del motor de animaciones
 * 
 * thisEngine: Motor para manejar 
 */
static void* gui_animation_engine_thread(void* thisEngine);

/************************************/
/* Definicion de funciones privadas */
/************************************/

/* gui_animation_engine_thread */
static void* gui_animation_engine_thread(void* thisEngine){
    ANIMATION_ENGINE* engine = thisEngine;
    ANIMATED_OBJECT* object;
    uint32_t i, ii;
    
    /* Me fijo si esta habilitado el motor de animaciones */
    while( !engine->shutdown ){
        
        if( !engine->pause ){
            /* Retardo temporal de 1mS */
            usleep(1000);

            /* Veo objeto por objeto */
            for(i = 0;i < engine->length;i++){
                /* Cargo el objeto */
                object = engine->objects[i];

                /* Me fijo el estado del objeto */
                if( object->status != GUI_ANIMATION_STATE_STATIC ){
                    
                    pthread_mutex_lock(&(object->objectMutex));
                    /* Incremento contador de tiempo */
                    object->timeCounter++;

                    /* Me fijo en que animacion esta orientado el objeto */
                    for(ii = 0;ii < NUMBER_OF_ORIENTATIONS;ii++){

                        /* Encuentro la animacion en dicha orientacion */
                        if( object->animations[ii].orientation == object->orientation ){

                            /* Me fijo si paso el tiempo */
                            if( object->timeCounter >= object->animations[ii].speed.timeDelta ){

                                /* Reinicio el tiempo */
                                object->timeCounter = 0;

                                /* Cambio la animacion */
                                if( object->status == GUI_ANIMATION_STATE_LOOP ||  object->status == GUI_ANIMATION_STATE_MOVE ){
                                    object->frameIndex++;
                                    if( object->frameIndex >= object->animations[ii].framesQty ){
                                        object->frameIndex = 0;
                                    }
                                }

                                /* Me fijo si hay que cambiar la posicion */
                                if( object->status == GUI_ANIMATION_STATE_MOVE || object->status == GUI_ANIMATION_STATE_STATIC_MOVE ){
                                    /* Muevo la posicion */
                                    switch(object->orientation){
                                        case GUI_ANIMATION_HORIZONTAL_LEFT:
                                            object->currentPos.x -= object->animations[ii].speed.spaceDelta;
                                            break;
                                        case GUI_ANIMATION_HORIZONTAL_RIGHT:
                                            object->currentPos.x += object->animations[ii].speed.spaceDelta;
                                            break;
                                        case GUI_ANIMATION_VERTICAL_UP:
                                            object->currentPos.y -= object->animations[ii].speed.spaceDelta;
                                            break;
                                        case GUI_ANIMATION_VERTICAL_DOWN:
                                            object->currentPos.y += object->animations[ii].speed.spaceDelta;
                                            break;
                                    }

                                    /* Me fijo si ya llego */
                                    if( object->currentPos.x == object->finalPos.x ){
                                        if( object->currentPos.y == object->finalPos.y ){
                                            object->status = GUI_ANIMATION_STATE_STATIC;
                                            object->frameIndex = 0;
                                        }
                                    }
                                }
                            }

                            /* Cierro el loop */
                            break;
                        }
                    }
                    pthread_mutex_unlock(&(object->objectMutex));
                }
            }
        }
    }
}

/************************************/
/* Definicion de funciones publicas */
/************************************/

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

/* gui_animation_load_objfile */
bool gui_animation_load_objfile(char* objFile, ANIMATED_OBJECT* object){
    SETTING* setting;
    ANIMATION animation;
    
    uint16_t i, ii;
    char frameIndex[2];
    char* auxStr;
    
    bool error = false;
    
    /* Abro el archivo */
    setting = gui_files_load_setting(objFile);
    if( setting == NULL ){
        return false;
    }
    
    /* Busco espacio libre */
    for(i = 0;i < NUMBER_OF_ORIENTATIONS;i++){
        if( object->animations[i].orientation == GUI_ANIMATION_ORIENTATION_NULL ){
            
            /* Cargo los datos */
            if( !gui_files_get_int(setting, OBJFILE_ATTRIBUTES, OBJFILE_TIMEDELTA, &animation.speed.timeDelta) ){
                error = true;
                break;
            }
            if( !gui_files_get_int(setting, OBJFILE_ATTRIBUTES, OBJFILE_SPACEDELTA, &animation.speed.spaceDelta) ){
                error = true;
                break;
            }
            if( !gui_files_get_int(setting, OBJFILE_ATTRIBUTES, OBJFILE_QUANTITY, &animation.framesQty) ){
                error = true;
                break;
            }
            
            auxStr = gui_files_get_string(setting, OBJFILE_ATTRIBUTES, OBJFILE_ORIENTATION);
            if( auxStr == NULL ){
                error = true;
                break;
            }
            if( !(strcmp(auxStr, OBJFILE_VERTICAL_UP)) ){
                animation.orientation = GUI_ANIMATION_VERTICAL_UP;
            }else if( !(strcmp(auxStr, OBJFILE_VERTICAL_DOWN)) ){
                animation.orientation = GUI_ANIMATION_VERTICAL_DOWN;
            }else if( !(strcmp(auxStr, OBJFILE_HORIZONTAL_LEFT)) ){
                animation.orientation = GUI_ANIMATION_HORIZONTAL_LEFT;
            }else if( !(strcmp(auxStr, OBJFILE_HORIZONTAL_RIGHT)) ){
                animation.orientation = GUI_ANIMATION_HORIZONTAL_RIGHT;
            }
            
            /* Reservo memoria para los frames */
            animation.frames = malloc( sizeof(FRAME) * animation.framesQty );
            if( animation.frames == NULL ){
                error = true;
                break;
            }
            
            /* Reservo memoria para cada frame file name */
            for(ii = 0;ii < animation.framesQty;ii++){
                /* Creo la clave del parametro */
                sprintf(frameIndex, "%d", ii);
                
                /* Busco el string */
                auxStr = gui_files_get_string(setting, OBJFILE_FRAMES, frameIndex);
                if( auxStr == NULL ){
                    error = true;
                    break;
                }
                
                /* Reservo memoria para ese string */
                animation.frames[ii] = malloc( sizeof(char) * (strlen(auxStr)+1) );
                if( animation.frames[ii] == NULL ){
                    error = true;
                    break;
                }
                
                /* Copio contenido */
                strcpy(animation.frames[ii], auxStr);                
            }
            
            break;
        }
    }
    
    gui_files_destroy_setting(setting);
    
    if( error ){
        gui_animation_destroy_framelist(animation.frames, ii);
    }else{
        object->animations[i] = animation;
    }
    return !error;
}

/* gui_animation_in_region */
bool gui_animation_in_region(ANIMATED_OBJECT* object, REGION region){
    if( object->currentPos.x >= region.iCorner.x && object->currentPos.x <= region.fCorner.x ){
        if( object->currentPos.y >= region.iCorner.y && object->currentPos.y <= region.fCorner.y ){
            return true;
        }
    }
    return false;
}

/* gui_animation_pause_engine */
void gui_animation_pause_engine(ANIMATION_ENGINE* engine){
    if( !engine->pause ){
        engine->pause = true;
    }
}

/* gui_animation_continue_engine */
void gui_animation_continue_engine(ANIMATION_ENGINE* engine){
    if( engine->pause ){
        engine->pause = false;
    }
}

/* gui_animation_get_frame */
FRAME gui_animation_get_frame(ANIMATED_OBJECT* object){
    uint16_t i;
    FRAME frame = NULL;
    
    /* Busco animacion correspondiente */
    for(i = 0;i < NUMBER_OF_ORIENTATIONS;i++){
        
        if( object->orientation == object->animations[i].orientation ){
            pthread_mutex_lock(&(object->objectMutex));
            frame = object->animations[i].frames[object->frameIndex];
            pthread_mutex_unlock(&(object->objectMutex));
            break;
        }
    }
    
    return frame;
}

/* gui_animation_start_static_movement */
void gui_animation_start_static_movement(ANIMATED_OBJECT* object, int32_t x, int32_t y){
    /* Configuro posicion final */
    object->finalPos.x = x;
    object->finalPos.y = y;
    
    /* Configuro estado */
    object->status = GUI_ANIMATION_STATE_STATIC_MOVE;
}

/* gui_animation_start_movement */
void gui_animation_start_movement(ANIMATED_OBJECT* object, uint16_t orientation, int32_t x, int32_t y){
    
    /* Configuro la posicion final de movimiento */
    object->finalPos.x = x;
    object->finalPos.y = y;
    
    /* Configuro la orientacion del objeto */
    object->orientation = orientation;
    
    /* Configuro estado movimiento */
    object->status = GUI_ANIMATION_STATE_MOVE;
}

/* gui_animation_stop_loop */
void gui_animation_stop_loop(ANIMATED_OBJECT* object){
    
    /* Reseteo el frame */
    object->frameIndex = 0;
    
    /* Desactivo estado loop */
    object->status = GUI_ANIMATION_STATE_STATIC;
}

/* gui_animation_start_loop */
void gui_animation_start_loop(ANIMATED_OBJECT* object, uint16_t orientation){
     
    /* Configuro la animacion del objeto */
    object->orientation = orientation;
    
    /* Pongo en modo bucle */
    object->status = GUI_ANIMATION_STATE_LOOP;
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

/* gui_animation_attach_object */
bool gui_animation_attach_object(ANIMATION_ENGINE* engine, ANIMATED_OBJECT* object){
    
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

/* gui_animation_destroy_engine */
void gui_animation_destroy_engine(ANIMATION_ENGINE* engine){
    
    /* Apago el thread */
    engine->shutdown = true;
    
    /* Libero la memoria de la lista de objetos */
    if( engine->objects != NULL ){
        free(engine->objects);
    }
    
    /* Liboer la memoria del motor */
    free(engine);
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
    
}

/* gui_animation_destroy_object */
void gui_animation_destroy_object(ANIMATED_OBJECT* object){
    uint16_t i;
    
    /* Elimino el mutex */
    pthread_mutex_destroy(&(object->objectMutex));
    
    /* Libero memoria de framelists de animaciones */
    for(i = 0;i < NUMBER_OF_ORIENTATIONS;i++){
        if( object->animations[i].frames != NULL ){
            gui_animation_destroy_framelist(object->animations[i].frames, object->animations[i].framesQty);
        }
    }
    
    /* Libero memoria del objeto */
    free(object);
}

/* gui_animation_create_object */
ANIMATED_OBJECT* gui_animation_create_object(int32_t x, int32_t y, uint16_t orientation){
    ANIMATED_OBJECT* object;
    uint16_t i;
    
    /* Reservo memoria para el objeto */
    object = malloc( sizeof(ANIMATED_OBJECT) );
    if( object == NULL ){
        return NULL;
    }
    
    /* Inicializo parametros */
    object->currentPos.x = x;
    object->currentPos.y = y;
    object->status = GUI_ANIMATION_STATE_STATIC;
    object->frameIndex = 0;
    object->orientation = orientation;
    object->timeCounter = 0;
    
    /* Creo el mutex */
    pthread_mutex_init(&(object->objectMutex), NULL);
    
    /* Inicializo con null los frames */
    for(i = 0;i < NUMBER_OF_ORIENTATIONS;i++){
        object->animations[i].frames = NULL;
        object->animations[i].orientation = GUI_ANIMATION_ORIENTATION_NULL;
    }
    
    /* Devuelvo la creacion */
    return object;
}

/* gui_animation_create_framelist */
FRAME* gui_animation_create_framelist(uint16_t frameQty){
    FRAME* framelist;
    uint16_t i;
    
    /* Reservo memoria */
    framelist = malloc( sizeof(FRAME) * frameQty );
    
    /* Verifico que haya podido */
    if( framelist == NULL ){
        return NULL;
    }
    
    /* Inicializo su contenido */
    for(i = 0;i < frameQty;i++){
        framelist[i] = NULL;
    }
    
    return framelist;
}

/* gui_animation_destroy_framelist */
void gui_animation_destroy_framelist(FRAME* framelist, uint16_t framesQty){
    uint16_t i;
    
    /* Libero memoria de los frames de la lista */
    for(i = 0;i < framesQty;i++){
        if( framelist[i] != NULL ){
            gui_animation_destroy_frame(framelist[i]);
        }
    }
    
    /* Libero la memoria reservada */
    if( framelist != NULL ){
        free(framelist);
    }
}

/* gui_animation_create_frame */
FRAME gui_animation_create_frame(char* str){
    FRAME frame;
    
    /* Asigno memoria */
    frame = malloc( sizeof(char) * strlen(str) );
    if( frame == NULL ){
        return NULL;
    }
    
    /* Copio el contenido */
    strcpy(frame, str);
    
    /* Devuelvo resultado */
    return frame;
}

/* gui_animation_destroy_frame */
void gui_animation_destroy_frame(FRAME frame){
    /* Libero memoria */
    if( frame != NULL ){
        free(frame);
    }
}
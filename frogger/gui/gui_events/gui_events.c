#include "gui_events.h"
#include <stdlib.h>

/********************************/
/* Prototipo funciones privadas */
/********************************/

/* create_sources 
 * Crea el arreglo con fuentes de eventos para la 
 * cola 
 */
static QUEUE_SOURCES* create_sources(void);

/************************************/
/* Definicion de funciones privadas */
/************************************/

/* register_source */
bool register_source(EVENT_QUEUE* queue, EVENT* (*callback)(void*), void* args){
    EVENT_SOURCE newEventSource;
    
    /* Reservo mas memoria para agregar */
    queue->sources->array = realloc(queue->sources->array, sizeof(EVENT_SOURCE) * (queue->sources->length+1));
    if( queue->sources->array == NULL ){
        return false;
    }
    
    /* Inicializo la nueve fuente de eventos */
    newEventSource.callback = callback;
    newEventSource.args = args;
    
    /* La agrego al arreglo */
    queue->sources->array[queue->sources->length++] = newEventSource;
    
    /* Registro exitoso */
    return true;
}

/* create_sources */
static QUEUE_SOURCES* create_sources(void){
    QUEUE_SOURCES* sources;
    
    /* Reservo memoria para el handler */
    sources = malloc( sizeof(QUEUE_SOURCES) );
    if( sources == NULL ){
        return NULL;
    }
    
    /* Reservo memoria para el arreglo */
    sources->array = malloc( sizeof(EVENT_SOURCE) );
    if( sources->array == NULL ){
        return NULL;
    }
    
    /* Inicializo parametros */
    sources->length = 0;
    
    /* Devuelvo */
    return sources;
}

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* create_queue */
EVENT_QUEUE* create_queue(void){
    EVENT_QUEUE* queue;
    
    /* Reservo memoria para el handler */
    queue = malloc( sizeof(EVENT_QUEUE) );
    if( queue == NULL ){
        return NULL;
    }
    
    /* Reservo memoria para la cola de eventos */
    queue->events = malloc( sizeof(EVENT) );
    if( queue->events == NULL ){
        return NULL;
    }
    
    /* Creo la lista de fuentes de eventos */
    queue->sources = create_sources();
    if( queue->sources == NULL ){
        return NULL;
    }
    
    /* Inicializo parametros */
    queue->nextEvent = queue->events;
    queue->lastEvent = queue->events;
    
    /* Devuelvo cola creada */
    return queue;
}
#include "gui_events.h"
#include <stdlib.h>

/********************************/
/* Prototipo funciones privadas */
/********************************/

/************************************/
/* Definicion de funciones privadas */
/************************************/

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* create_sources */
QUEUE_SOURCES* create_sources(void){
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
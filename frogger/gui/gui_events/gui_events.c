#include "gui_events.h"
#include <stdlib.h>

/**********************/
/* Objetos del modulo */
/**********************/

/********************************/
/* Prototipo funciones privadas */
/********************************/

/************************************/
/* Definicion de funciones privadas */
/************************************/

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
    
    /* Inicializo parametros */
    queue->nextEvent = queue->events;
    queue->lastEvent = queue->events;
    
    /* Devuelvo cola creada */
    return queue;
}
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

/* raise_event
 * Agrega un evento a la cola de eventos
 *
 * queue: Cola de eventos
 * event: Evento
 */
static bool raise_event(EVENT_QUEUE* queue, EVENT* event);

/* queue_thread
 * Maneja la cola de eventos
 *
 * queue: La cola que tiene que manejar 
 */
static void* queue_thread(void* queue);

/* source_destroy
 * Libera la memoria de una lista de fuentes
 *
 * sources: Fuentes de eventos 
 */
static void source_destroy(QUEUE_SOURCES* sources);

/************************************/
/* Definicion de funciones privadas */
/************************************/

/* source_destroy */
static void source_destroy(QUEUE_SOURCES* sources){
    
    /* Libero memoria del arreglo */
    free(sources->array);
    
    /* Libero memoria del handler */
    free(sources);
}

/* queue_thread */
static void* queue_thread(void* queue){
    uint32_t i;
    EVENT_QUEUE* eventQueue = queue;
    EVENT* event;
    
    EVENT* (*callback)(void*);
    void* args;
    
    /* Habilito funcionamiento de la cola */
    while( !eventQueue->shutdown ){
        
        /* Reviso todas las fuentes de eventos */
        for(i = 0;i < eventQueue->sources->length;i++){
            /* Busco los datos de la fuente */
            callback = eventQueue->sources->array[i].callback;
            args = eventQueue->sources->array[i].args;
            
            /* Busco actualizacion del evento */
            event = callback(args);
            if( event ){
                raise_event(eventQueue, event);
            }
        }
    }
}

/* raise_event */
static bool raise_event(EVENT_QUEUE* queue, EVENT* event){
    uint32_t queueLength;
    
    /* Calculo largo de la cola */
    queueLength = queue->lastEvent - queue->events + 1;
    
    /* Compruebo el largo de memoria */
    if( queue->length == queueLength ){
        /* Reservo memoria para el nuevo evento */
        queue->events = realloc(queue->events, sizeof(EVENT) * (queueLength+1));
        if( queue->events == NULL ){
            return false;
        }
        queue->length++;
    }
    
    /* Guardo el nuevo elemento */
    *(queue->lastEvent) = *event;
    
    /* Muevo al terminador */
    queue->lastEvent++;
    
    /* Proceso exitoso */
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

/* queue_next_event */
bool queue_next_event(EVENT_QUEUE* queue, EVENT* event){
    
    /* Compruebo si hay un evento en la cola */
    if( queue->nextEvent != queue->lastEvent ){
        
    }
}

/* queue_close */
void queue_close(EVENT_QUEUE* queue){
    
    /* Apago el thread */
    queue->shutdown = true;
    
    /* Libero memoria de las fuentes */
    source_destroy(queue->sources);
    
    /* Libero memoria de la cola */
    free(queue->events);
}

/* queue_start */
void queue_start(EVENT_QUEUE* queue){
    
    /* Habilito */
    queue->shutdown = false;
    
    /* Inicio el thread */
    pthread_create(&(queue->queueThread), NULL, queue_thread, queue);
}

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
    queue->shutdown = false;
    queue->length = 1;
    
    /* Devuelvo cola creada */
    return queue;
}
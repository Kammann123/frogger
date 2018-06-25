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
    EVENT event;
    
    bool (*callback)(EVENT*, void*);
    void* args;
    
    /* Habilito funcionamiento de la cola */
    while( !eventQueue->shutdown ){
        
        pthread_mutex_lock(&eventQueue->queueMutex);
        /* Verifico error */
        if( eventQueue->shutdown ){
            pthread_mutex_unlock(&eventQueue->queueMutex);
            pthread_exit(NULL);
        }
        /* Reviso todas las fuentes de eventos */
        for(i = 0;i < eventQueue->sources->length;i++){
            /* Busco los datos de la fuente */
            callback = eventQueue->sources->array[i].callback;
            args = eventQueue->sources->array[i].args;
            
            /* Busco actualizacion del evento */
            if( callback(&event, args) ){
                raise_event(eventQueue, &event);
            }
        }
        pthread_mutex_unlock(&eventQueue->queueMutex);
    }
}

/* raise_event */
static bool raise_event(EVENT_QUEUE* queue, EVENT* event){
    uint32_t queueLength;
    
    /* Reutilizo la memoria */
    if( queue->nextEvent == queue->lastEvent ){
        queue->nextEvent = 0;
        queue->lastEvent = 0;
    }
    
    /* Calculo largo de la cola */
    queueLength = queue->lastEvent + 1;
    
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
    queue->events[queue->lastEvent] = *event;
    
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

/* queue_flush */
void queue_flush(EVENT_QUEUE* queue){
    pthread_mutex_lock(&(queue->queueMutex));
    queue->lastEvent = 0;
    queue->nextEvent = 0;
    pthread_mutex_unlock(&(queue->queueMutex));
}


/* queue_next_event */
bool queue_next_event(EVENT_QUEUE* queue, EVENT* event){
    
    /* Compruebo si hay un evento en la cola */
    if( queue->nextEvent != queue->lastEvent ){
        pthread_mutex_lock(&(queue->queueMutex));
        *event = queue->events[queue->nextEvent];
        queue->nextEvent++;
        pthread_mutex_unlock(&(queue->queueMutex));
        return true;
    }else{
        /* No habia eventos */
        return false;
    }
}

/* queue_close */
void queue_close(EVENT_QUEUE* queue){
    
    pthread_mutex_lock(&queue->queueMutex);
    /* Apago el thread */
    queue->shutdown = true;
    pthread_mutex_unlock(&queue->queueMutex);
    
    /* Elimino el mutex */
    pthread_mutex_destroy(&(queue->queueMutex));
    
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
bool register_source(EVENT_QUEUE* queue, bool (*callback)(EVENT*, void*), void* args){
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
    queue->events = malloc( sizeof(EVENT) * QUEUE_INITIAL_LENGTH );
    if( queue->events == NULL ){
        return NULL;
    }
    
    /* Creo la lista de fuentes de eventos */
    queue->sources = create_sources();
    if( queue->sources == NULL ){
        return NULL;
    }
    
    /* Inicializo el mutex */
    pthread_mutex_init(&(queue->queueMutex), NULL);
    
    /* Inicializo parametros */
    queue->nextEvent = 0;
    queue->lastEvent = 0;
    queue->shutdown = false;
    queue->length = QUEUE_INITIAL_LENGTH;
    
    /* Devuelvo cola creada */
    return queue;
}
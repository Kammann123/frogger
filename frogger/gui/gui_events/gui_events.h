/* GUI_EVENTS
 * Este modulo permite manejar colas de eventos de forma dinamica.
 */

/* Librerias utilizadas */
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#ifndef GUI_EVENTS_H
#define GUI_EVENTS_H

/******************/
/* Tipos de datos */
/******************/

typedef struct{
    /* Parametros de un evento */
    uint32_t type;
    uint32_t data;
    uint32_t source;
} EVENT;

typedef struct{
    /* Callback del generador de evento */
    EVENT* (*callback)(void*);
    
    /* Argumento opcional para el callback */
    void* args;
} EVENT_SOURCE;

typedef struct{
    /* Arreglo de fuentes de evento */
    EVENT_SOURCE* array;
    
    /* Cantidad de fuentes registradas */
    uint32_t length;
} QUEUE_SOURCES;

typedef struct{
    /* Puntero a la cola */
    EVENT* events;
    
    /* Puntero al proximo y ultimo elemento */
    EVENT* nextEvent;
    EVENT* lastEvent;
    
    /* Largo de la cola actual */
    uint32_t length;
    
    /* Fuentes de eventos para la cola */
    QUEUE_SOURCES* sources;
    
    /* Habilitadores */
    bool shutdown;
    
    /* Thread */
    pthread_t queueThread;
} EVENT_QUEUE;

/********************************/
/* Prototipo funciones publicas */
/********************************/

/* queue_next_event
 * Devuelve true si hay un evento en la cola y lo retorna
 * en el respectivo parametro
 *
 * queue: Cola de eventos 
 * event: Evento a retornar
 */
bool queue_next_event(EVENT_QUEUE* queue, EVENT* event);

/* queue_start
 * Inicia el funcionamiento de la cola
 *
 * queue: Cola de eventos
 */
void queue_start(EVENT_QUEUE* queue);

/* queue_close
 * Apaga la cola de eventos y la destruye
 * 
 * queue: Cola de eventos
 */
void queue_close(EVENT_QUEUE* queue);

/* register_source
 * Agrega un generador de eventos a una cola especifica
 * permite agregar un generador que se ejecuta en busca de eventos
 *
 * queue: Cola de eventos 
 * callback: Funcion para buscar esos eventos
 * args: Parametros opcionales necesarios 
 */
bool register_source(EVENT_QUEUE* queue, EVENT* (*callback)(void*), void* args);

/* create_queue 
 * Reserva memoria inicial para trabajar con
 * la cola de eventos y devuelve puntero 
 */
EVENT_QUEUE* create_queue(void);

#endif /* GUI_EVENTS_H */
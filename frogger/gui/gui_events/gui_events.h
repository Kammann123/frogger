/* GUI_EVENTS
 * Este modulo permite manejar colas de eventos de forma dinamica.
 */

/* Librerias utilizadas */
#include <stdint.h>
#include <stdbool.h>

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
    
    /* Fuentes de eventos para la cola */
    QUEUE_SOURCES sources; 
} EVENT_QUEUE;

/********************************/
/* Prototipo funciones publicas */
/********************************/

/* create_queue 
 * Reserva memoria inicial para trabajar con
 * la cola de eventos y devuelve puntero 
 */
EVENT_QUEUE* create_queue(void);

/* raise_event
 * Agrega un evento a la cola de eventos, informando el tipo y dato
 * asi como quien esta generandolo 
 *
 * type: Tipo del evento
 * data: Dato del evento
 * source: Fuente que genera el evento
 */
bool raise_event(uint32_t type, uint32_t data, uint32_t source);

#endif /* GUI_EVENTS_H */
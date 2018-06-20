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
    QUEUE_SOURCES* sources; 
} EVENT_QUEUE;

/********************************/
/* Prototipo funciones publicas */
/********************************/

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
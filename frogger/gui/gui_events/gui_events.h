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
    uint32_t type;
    uint32_t data;
    uint32_t source;
} EVENT;

typedef struct{
    /* Puntero a la cola */
    EVENT* events;
    
    /* Puntero al proximo y ultimo elemento */
    EVENT* nextEvent;
    EVENT* lastEvent;
} EVENT_QUEUE;

/********************************/
/* Prototipo funciones publicas */
/********************************/

/* create_queue 
 * Reserva memoria inicial para trabajar con
 * la cola de eventos y devuelve puntero 
 */
EVENT_QUEUE* create_queue(void);

#endif /* GUI_EVENTS_H */
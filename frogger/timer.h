#ifndef TIMER_H
#define TIMER_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>

/************************/
/* Estructuras de datos */
/************************/

typedef struct{
    uint16_t id;
    uint32_t timerCounter;
    uint32_t timerMax;
    void* (*callback)(void*);
    void* args;
} TIMER_EVENT;

typedef struct{
    TIMER_EVENT* events;
    uint16_t length;
    bool enable;
} TIMER_QUEUE;

/**********************/
/* Funciones publicas */
/**********************/

/* start_timer
 * Inicia el timer y su thread, ejecutar UNA vez
 */
void start_timer(void);

/* pause_timer 
 * Pausa el timer 
 */
void pause_timer(void);

/* resume_timer
 * Reanuda el timer que estaba pausado
 */
void resume_timer(void);

/* new_event
 * Crea e inicia un nuevo */
int16_t new_event(uint32_t time, void* (*callback)(void*), void* args); 

/* del_event_by_id
 * Elimina un evento segun su id
 *
 * id: Identificador del evento 
 */
void del_event_by_id(uint16_t id);

/* del_all_events
 * Elimina todos los eventos de timer
 */
void del_all_events(void);

/* wait_timer 
 * Hace que el hilo principal espere al timer
 * antes de terminar de funcionar 
 */
void wait_timer(void);

/* timer_destroy
 * Libera la memoria reservada
 */
void timer_destroy(void);

#endif /* TIMER_H */


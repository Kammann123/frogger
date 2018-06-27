/* GUI_TIMER
 * Este modulo permite generar eventos con timer
 * configurando los tiempos de los mismos
 */

#ifndef GUI_TIMER_H
#define GUI_TIMER_H

/* Librerias utilizadas */
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#include "../gui_events/gui_events.h"

/**************/
/* Constantes */
/**************/
#define TIMER_OVERFLOW  0

/******************/
/* Tipos de datos */
/******************/

typedef struct{
    /* Atributos del timer */
    uint32_t id;
    uint32_t timerCounter;
    uint32_t timerMax;
    
    /* Habilitadores */
    bool pause;
    bool enableAsEvent;
    
    /* Flag */
    bool timerOverflow;
} TIMER;

typedef struct{
    /* Array de timers */
    TIMER* timers;
    
    /* Largo de la cola */
    uint32_t length;
    
    /* Habilitadores */
    bool enable;
    bool shutdown;
    
    /* Thread */
    pthread_t timerThread;
    
    /* Mutex */
    pthread_mutex_t timerMutex;
} TIMER_QUEUE;

/**********************/
/* Funciones publicas */
/**********************/

/* gui_timer_overflow
 * Devuelve si el timer hizo overflow
 *
 * timerQueue: Cola de timers
 * id: Identificador del timer
 */
bool gui_timer_overflow(TIMER_QUEUE* timerQueue, uint32_t id);

/* gui_timer_global_get
 * Retorna el puntero a la cola de timers global
 */
TIMER_QUEUE* gui_timer_global_get(void);

/* gui_timer_global_close
 * Cierra y libera la cola de timers global
 */
void gui_timer_global_close(void);

/* gui_timer_global_init
 * Inicializa la cola de eventos global
 */
bool gui_timer_global_init(void);

/* gui_timer_pause
 * Pausa un timer individual en la cola de timers
 *
 * id: Identificador del timer
 */
void gui_timer_pause(TIMER_QUEUE* timerQueue, uint32_t id);

/* gui_timer_continue
 * Reanuda un timer individual pausado en la cola de timers
 *
 * id: Identificador del timer
 */
void gui_timer_continue(TIMER_QUEUE* timerQueue, uint32_t id);

/* gui_timer_clear_all 
 * Limpia todos los timers de la cola de timers
 *
 * timerQueue: Cola de timers
 */
void gui_timer_clear_all(TIMER_QUEUE* timerQueue);

/* gui_timer_clear 
 * Limpia el timer despues de un overflow, avisando
 * que se leyo y uso el evento
 *
 * timerQueue: Cola de timers
 * id: Identificador del timer
 */
void gui_timer_clear(TIMER_QUEUE* timerQueue, uint32_t id);

/* gui_timer_queue_pause
 * Pausa el funcionamiento del timer queue
 * 
 * timerQueue: Cola de timers 
 */
void gui_timer_queue_pause(TIMER_QUEUE* timerQueue);

/* gui_timer_queue_continue
 * Reanuda el funcionamiento de la cola de timers
 * 
 * timerQueue: Cola de timers
 */
void gui_timer_queue_continue(TIMER_QUEUE* timerQueue);

/* gui_timer_init 
 * Inicializa la queue del timer 
 */
TIMER_QUEUE* gui_timer_create(void);

/* gui_timer_destroy 
 * Libera la memoria de un timer 
 */
void gui_timer_destroy(TIMER_QUEUE* timerQueue);

/* gui_timer_new_event
 * Crea e inicia un nuevo evento de timer a la cola
 *
 * timerQueue: Cola de timers
 * time: Tiempo en milisegundos del evento
 * id: Id para reconocer el evento
 * enable: Habilita su uso para la cola de eventos
 */
bool gui_timer_new_event(TIMER_QUEUE* timerQueue, uint32_t time, uint32_t id, bool enable); 

/* gui_timer_start 
 * Inicia un timer queue
 *
 * timerQueue: Cola de timers
 */
void gui_timer_start(TIMER_QUEUE* timerQueue);

/* gui_timer_source
 * Genera eventos timer segun la cola
 */
bool gui_timer_source(EVENT* event, void* timerQueue);

#endif /* GUI_TIMER_H */


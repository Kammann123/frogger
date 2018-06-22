#include "gui_timer.h"

#include <unistd.h>
#include <stdlib.h>

/**********************/
/* Funciones privadas */
/**********************/

/* timer_thread
 * Thread de la cola de timer para manejar
 * los eventos por timer agregados
 * 
 * timerQueue: Cola de eventos
 */
static void* timer_thread(void* timerQueue);

/************************************/
/* Definicion de funciones privadas */
/************************************/

/* gui_timer_source */
bool gui_timer_source(EVENT* event, void* timerQueue){
    uint32_t i;
    TIMER_QUEUE* queue = timerQueue;
    
    /* Inicializo el evento */
    event->source = TIMER_SOURCE;
    
    /* Reviso todos los eventos */
    for(i = 0;i < queue->length;i++){
        
        /* Compruebo timer overflow */
        if( queue->timers[i].timerCounter >= queue->timers[i].timerMax && !(queue->timers[i].timerOverflow) ){
            
            pthread_mutex_lock(&(queue->timerMutex));
            
            /* Raise event */
            event->type = TIMER_OVERFLOW;
            event->data = queue->timers[i].id;
            /* Flag */
            queue->timers[i].timerOverflow = true;

            pthread_mutex_unlock(&(queue->timerMutex));
            
            return true;
        }
    }
    
    return false;
}

/* timer_thread */
static void* timer_thread(void* timerQueue){
    uint32_t i;
    TIMER_QUEUE* queue = timerQueue;
    
    while( !queue->shutdown ){
        /* Espera un milisegundo */
        usleep(1000);
        
        /* Actualizo estado de eventos */
        pthread_mutex_lock(&(queue->timerMutex));
        /* Verifico error de apagado */
        if( queue->shutdown ){
            pthread_exit(NULL);
        }
        if( queue->enable ){
            for(i = 0;i < queue->length;i++){
                if(queue->timers[i].timerCounter < queue->timers[i].timerMax){
                    queue->timers[i].timerCounter++;
                }
            }
        }
        pthread_mutex_unlock(&(queue->timerMutex));
    }
}

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* gui_timer_clear */
void gui_timer_clear(TIMER_QUEUE* timerQueue, uint32_t id){
    uint32_t i;
    
    /* Busco el timer */
    for(i = 0;i < timerQueue->length;i++){
        if( timerQueue->timers[i].id == id ){
            timerQueue->timers[i].timerOverflow = false;
            timerQueue->timers[i].timerCounter = 0;
            break;
        }
    }
}

/* gui_timer_pause */
void gui_timer_pause(TIMER_QUEUE* timerQueue){
    /* Deshabilito el thread */
    timerQueue->enable = false;
}

/* gui_timer_continue */
void gui_timer_continue(TIMER_QUEUE* timerQueue){
    /* Habilito el thread */
    timerQueue->enable = true;
}

/* gui_timer_start */
void gui_timer_start(TIMER_QUEUE* timerQueue){
    /* Habilito */
    timerQueue->enable = true;
    timerQueue->shutdown = false;
    
    /* Arranco el thread */
    pthread_create(&(timerQueue->timerThread), NULL, timer_thread, timerQueue);
}

/* gui_timer_destroy */
void gui_timer_destroy(TIMER_QUEUE* timerQueue ){
    
    pthread_mutex_lock(&(timerQueue->timerMutex));
    /* Apago el thread */
    timerQueue->shutdown = true;
    pthread_mutex_unlock(&(timerQueue->timerMutex));
    
    /* Destruyo el mutex */
    pthread_mutex_destroy(&(timerQueue->timerMutex));
    
    /* Libero memoria de los arreglos */
    free(timerQueue->timers);
    
    /* Libero memoria del handler */
    free(timerQueue);
}

/* gui_timer_create */
TIMER_QUEUE* gui_timer_create(void){
    TIMER_QUEUE* timerQueue;
    
    /* Reservo memoria para handler */
    timerQueue = malloc( sizeof(TIMER_QUEUE) );
    if( timerQueue == NULL ){
        return NULL;
    }
    
    /* Reservo memoria para arreglo de timers */
    timerQueue->timers = malloc(sizeof(TIMER));
    if( timerQueue->timers == NULL ){
        return NULL;
    }
    
    /* Parametros iniciales */
    timerQueue->enable = false;
    timerQueue->length = 0;
    timerQueue->shutdown = false;
    
    /* Inicializo el mutex */
    pthread_mutex_init(&(timerQueue->timerMutex), NULL);
    
    return timerQueue;
}

/* gui_timer_new_event */
bool gui_timer_new_event(TIMER_QUEUE* timerQueue, uint32_t time, uint32_t id){
    
    /* Reasigo mas memoria */
    timerQueue->timers = realloc(timerQueue->timers, sizeof(TIMER) * (timerQueue->length+1));
    if( timerQueue->timers == NULL ){
        return false;
    }
    
    /* Inicializo los parametros nuevos */
    timerQueue->timers[timerQueue->length].id = id;
    timerQueue->timers[timerQueue->length].timerCounter = 0;
    timerQueue->timers[timerQueue->length].timerMax = time;
    timerQueue->timers[timerQueue->length].timerOverflow = false;
    
    /* Incremento contador */
    timerQueue->length++;
    
    return true;
}
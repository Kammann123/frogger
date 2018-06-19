#include "timer.h"

/**************************/
/* Objetos de la libreria */
/**************************/

static TIMER_QUEUE queue;

static pthread_t timerThread;

/**********************/
/* Funciones privadas */
/**********************/

/* timer_thread
 * Thread para el timer
 */
static void* timer_thread(void*);

/* queue_init 
 * Inicializa queue 
 */
static bool queue_init(void);

/* timer_destroy
 * Libera la memoria reservada
 */
static void timer_destroy(void);

/***************************/
/* Definicion de funciones */
/***************************/

/* wait_timer */
void wait_timer(){
    pthread_join(timerThread, NULL);
}

/* start_timer */
void start_timer(){
    queue.enable = true;
    pthread_create(&timerThread, NULL, timer_thread, NULL);
}

/* pause_timer */
void pause_timer(){
    queue.enable = false;
}

/* resume_timer */
void resume_timer(){
    queue.enable = true;
}

/* timer_thread */
static void* timer_thread(){
    uint16_t i;
    
    while( !queue.shutdown ){
        /* Espera un milisegundo */
        usleep(1000);
        
        /* Actualizo estado de eventos */
        if( queue.enable ){
            for(i = 0;i < queue.length;i++){
                queue.events[i].timerCounter++;
                if(queue.events[i].timerCounter >= queue.events[i].timerMax){
                    queue.events[i].timerCounter = 0;
                    queue.events[i].callback(queue.events[i].args);
                }
            }
        }
    }
}

/* queue_init */
static bool queue_init(void){
    
    /* Parametros iniciales */
    queue.enable = false;
    queue.length = 0;
    queue.shutdown = false;
    
    /* Reservo memoria */
    queue.events = malloc( sizeof(TIMER_EVENT) );
    if( queue.events == NULL ){
        return false;
    }
    return true;
}

/* timer_destroy */
static void timer_destroy(void){
    free(queue.events);
}

/* timer_close */
void timer_close(void){
    timer_destroy();
    queue.shutdown = true;
}
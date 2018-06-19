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

/***************************/
/* Definicion de funciones */
/***************************/

/* queue_init */
static bool queue_init(void){
    
    /* Parametros iniciales */
    queue.enable = false;
    queue.length = 0;
    
    /* Reservo memoria */
    queue.events = malloc( sizeof(TIMER_EVENT) );
    if( queue.events == NULL ){
        return false;
    }
    return true;
}
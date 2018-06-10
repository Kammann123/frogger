#include "input.h"
#include "settings.h"

/**************************/
/* Objetos de la libreria */
/**************************/

/* Estado del evento de entrada */
static uint16_t eventStatus = NO_EVENT;

/**********************/
/* Funciones privadas */
/**********************/

/* update_input_status
 * Actualiza el estado del evento leyendo
 * las entradas del usuario
 */
static void update_input_status(void);

/* input_thread
 * Thread de actualizacion ciclica del estado
 * del objeto evento
 */
static void* input_thread(void*);

/***************************/
/* Definicion de funciones */
/***************************/

/* input_thread */
static void* input_thread(void* none){
    // El codigo va aqui xD
}

/* update_input_status */
static void update_input_status(void){
#if PLATFORM_MODE == RPI
	
#elif PLATFORM_MODE == PC_ALLEGRO
#else 
	//no mode set
#endif
    // El codigo va aqui xD
}

/* input_init */
bool input_init(void){
    // El codigo va aqui xD
}

/* input_close */
void input_close(void){
    // El codigo va aqui xD
}

/* get_input_status */
uint16_t get_input_status(void){
    return eventStatus;
}
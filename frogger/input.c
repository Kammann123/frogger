#include "input.h"

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
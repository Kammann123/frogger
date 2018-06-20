/* ALLEGRO_INPUT 
 * Este modulo permite manejar entradas mediante allegro por teclado
 */

/* Librerias utilizadas */
#include "../../gui_events/gui_events.h"

#include <stdbool.h>

#ifndef ALLEGRO_INPUT_H
#define ALLEGRO_INPUT_H

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* allegro_input_init 
 * Inicializa la cola de eventos de allegro
 * y la configura para revisar el teclado 
 */
bool allegro_input_init(void);

/* allegro_input_close
 * Cierra y libera la cola de eventos de allegro
 */
void allegro_input_close(void);

/* allegro_input_source
 * Generador de eventos por teclado, verifica si hay
 * algun evento en la cola de eventos de allegro
 * 
 * event: Evento donde se guarda
 * none: NULL
 */
bool allegro_input_source(EVENT* event, void* none);

#endif /* ALLEGRO_INPUT_H */


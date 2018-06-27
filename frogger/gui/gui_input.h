/* GUI_INPUT
 * Este modulo maneja las entradas del usuario al programa
 */

#ifndef GUI_INPUT_H
#define GUI_INPUT_H

/* Liberias utilizadas */
#include "gui_events/gui_events.h"

#include <stdbool.h>

/**************/
/* Constantes */
/**************/

/* Tipos de eventos */
typedef enum{
    MOVEMENT_EVENT,
    ACTION_EVENT
} INPUT_TYPES;

/* Valores de eventos */
typedef enum{
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    ENTER,
    MOVE_NONE
} INPUT_VALUES;

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* gui_input_init
 * Inicializa la interfaz grafica que corresponda
 */
bool gui_input_init(void);

/* gui_input_close
 * Cierra la interfaz grafica que corresponde
 */
void gui_input_close(void);

/* gui_input_source
 * Devuelve un evento de entrada del usuario
 *
 * event: Evento a guardar
 * none: NULL
 */
bool gui_input_source(EVENT* event, void* none);

#endif /* GUI_INPUT_H */

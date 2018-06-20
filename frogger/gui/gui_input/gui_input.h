/* GUI_INPUT
 * Este modulo hace de wrapper para acceder a 
 * las entradas del usuario por distintas plataformas
 */

/* Librerias utilizadas */
#include "../../settings.h"

#include "../allegro/allegro_input/allegro_input.h"

#ifndef GUI_INPUT_H
#define GUI_INPUT_H

/**************/
/* Constantes */
/**************/

/* Tipos de eventos */
#define MOVEMENT_EVENT  0
#define ACTION_EVENT    1

/* Valores de eventos */
#define MOVE_LEFT   0
#define MOVE_RIGHT  1
#define MOVE_UP     2
#define MOVE_DOWN   3
#define ENTER       4

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

/* gui_input_event
 * Devuelve un evento de entrada del usuario
 *
 * event: Evento a guardar
 */
bool gui_input_event(EVENT* event);

#endif /* GUI_INPUT_H */
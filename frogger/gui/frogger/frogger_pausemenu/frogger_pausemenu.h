/* FROGGER_PAUSEMENU
 * Este modulo maneja la ventana de menu de pausa
 * con la interfaz grafica que corresponda en la configuracion
 */

#ifndef FROGGER_PAUSEMENU_H
#define FROGGER_PAUSEMENU_H

/* Librerias utilizadas */
#include <stdint.h>

#include "../../gui_input/gui_input.h"
#include "../../../settings.h"

/**************/
/* Constantes */
/**************/

/* Opciones del menu */
#define RESUME_OPTION     0
#define RESTART_OPTION    1
#define EXIT_OPTION       2  

/* Opcion seleccionada default */
#define DEFAULT_SELECTION   0

/* Mensaje de opciones */
#define RESUME_MSG      "RESUME"
#define RESTART_MSG     "RESTART"
#define EXIT_MSG        "EXIT"

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* frogger_pausemenu_close 
 * Cierra el menu de pausa de frogger
 */
void frogger_pausemenu_close(void);

/* frogger_pausemenu_update
 * Actualiza el menu en la interfaz
 */
void frogger_pausemenu_update(void);

/* frogger_pausemenu_move
 * Mueve la seleccion del menu segun la entrada
 * del usuario por interfaz
 *
 * input: Entrada del usuario
 */
void frogger_pausemenu_move(uint16_t input);

/* frogger_mainmenu_selected 
 * Devuelve numero de opcion actualmente seleccionada
 */
uint16_t frogger_pausemenu_selected(void);

#endif /* NEWFILE_H */


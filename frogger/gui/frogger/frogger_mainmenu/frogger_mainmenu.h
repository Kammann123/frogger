/* FROGGER_MAINMENU
 * Este modulo maneja la interfaz de entrada y salida
 * para visualizar el menu principal del juego frogger
 */

#ifndef FROGGER_MAINMENU_H
#define FROGGER_MAINMENU_H

/* Librerias utilizadas */
#include <stdint.h>

#include "../../gui_input/gui_input.h"
#include "../../../settings.h"

/**************/
/* Constantes */
/**************/

/* Opciones del menu */
#define MAINMENU_PLAY_OPTION     0
#define MAINMENU_RANK_OPTION     1
#define MAINMENU_HOWTO_OPTION    2   
#define MAINMENU_EXIT_OPTION     3

/* Opcion seleccionada default */
#define DEFAULT_SELECTION   0

/* Mensaje de opciones */
#define PLAY_MSG        "JUGAR"
#define RANK_MSG        "SCORE"
#define HOWTO_MSG       "HOWTO"
#define EXIT_MSG        "EXIT"

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* frogger_mainmenu_close 
 * Cierra el menuprincipal de frogger
 */
void frogger_mainmenu_close(void);

/* frogger_mainmenu_update
 * Actualiza el menu en la interfaz
 */
void frogger_mainmenu_update(void);

/* frogger_mainmenu_move
 * Mueve la seleccion del menu segun la entrada
 * del usuario por interfaz
 *
 * input: Entrada del usuario
 */
void frogger_mainmenu_move(uint16_t input);

/* frogger_mainmenu_selected 
 * Devuelve numero de opcion actualmente seleccionada
 */
uint16_t frogger_mainmenu_selected(void);


#endif /* FROGGER_MAINMENU_H */


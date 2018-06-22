/* FROGGER_GAME
 * Este modulo permite manejar la interfaz con el usuario
 * del juego frogger game
 */

#ifndef FROGGER_GAME_H
#define FROGGER_GAME_H

/* Librerias utilizadas */
#include <stdint.h>
#include <stdbool.h>

#include "../../../settings.h"
#include "../../allegro/frogger/game/allegro_frogger_game.h"

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* frogger_game_screen_update
 * Actualiza la pantalla del frogger
 */
void frogger_game_screen_update(void);

#endif /* FROGGER_GAME_H */


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
#include "../../gui_animation/gui_animation.h"
#include "../../allegro/frogger/game/allegro_frogger_game.h"

/**************/
/* Constantes */
/**************/

/* Parametros default */
#define DEFAULT_FROG_X  7
#define DEFAULT_FROG_Y  10
#define DEFAULT_FROG_ORIENTATION GUI_ANIMATION_VERTICAL_UP

/* Parametros de tablero */
#define DISPLAY_DIVISIONS_X 16
#define DISPLAY_DIVISIONS_Y 11

/******************/
/* Tipos de datos */
/******************/

typedef struct{
    ANIMATED_OBJECT* object;
    ANIMATED_OBJECT* transport;
} FROG;

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* move_frog
 * Mueve el objeto frog en la direccion segun input
 *
 * input: Direccion ingresada por usuario 
 */
void frogger_game_move_frog(uint16_t input);

/* frogger_game_pause
 * Pausa el juego
 */
void frogger_game_pause(void);

/* frogger_game_continue
 * Reanuda el juego
 */
void frogger_game_continue(void);

/* frogger_game_close 
 * Cierra y libera los objetos del juego creados
 */
void frogger_game_close(void);

/* frogger_game_init 
 * Inicializa los objetos del juego
 */
bool frogger_game_init(void);

/* frogger_game_screen_update
 * Actualiza la pantalla del frogger
 */
void frogger_game_screen_update(void);

/* frogger_game_screen_close 
 * Cierra la pantalla del juego del
 * frogger 
 */
void frogger_game_screen_close(void);

#endif /* FROGGER_GAME_H */


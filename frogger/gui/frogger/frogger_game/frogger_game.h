/* FROGGER_GAME
 * Este modulo permite manejar la interfaz con el usuario
 * del juego frogger game
 */

#ifndef FROGGER_GAME_H
#define FROGGER_GAME_H

/* Librerias utilizadas */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../../../settings.h"
#include "../../gui_animation/gui_animation.h"

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

/* Definicion de carriles */
#define NUMBER_OF_LANES 8

/******************/
/* Tipos de datos */
/******************/

typedef struct{
    ANIMATED_OBJECT* object;
    ANIMATED_OBJECT* transport;
} FROG;

typedef ANIMATED_OBJECT* MOTORBIKE;
typedef ANIMATED_OBJECT* CAR;
typedef ANIMATED_OBJECT* TRUCK;
typedef ANIMATED_OBJECT* BOAT;
typedef ANIMATED_OBJECT* YACHT;

typedef struct{
    /* Objetos del carril */
    ANIMATED_OBJECT* objects;
    uint16_t objectsQty;
    
    /* Atributos del carril */
    uint16_t laneNumber;
    
    /* Movimiento del carril */
    uint16_t type;
    uint16_t orientation;
    SPEED speed;
} LANE;

typedef LANE** LANES;

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* frogger_game_move_frog
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


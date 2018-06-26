/* FROGGER_OUTPUT
 * Maneja la interfaz de salida del videojuego
 */

#ifndef FROGGER_OUTPUT_H
#define FROGGER_OUTPUT_H

/* Librerias utilizadas */
#include <stdint.h>

#include "../gui_input.h"

#include "frogger_physics.h"

/**************/
/* Constantes */
/**************/

typedef enum {
    PLAY_OPTION,
    RANK_OPTION,
    HOWTO_OPTION,
    MAIN_EXIT_OPTION
} MAINMENU_OPTIONS;

typedef enum {
    RESUME_OPTION,
    RESTART_OPTION,
    PAUSE_EXIT_OPTION
} PAUSEMENU_OPTIONS;

/* Variables externas para acceso */
extern MAINMENU_OPTIONS mainmenuSelection;
extern PAUSEMENU_OPTIONS pausemenuSelection;

/*******************/
/* SCREEN handlers */
/*******************/

/* frogger_screen_close
 * Cierra aquello que use para la pantalla
 */
void frogger_screen_close(void);

/*********************/
/* MAINMENU handlers */
/*********************/

/* frogger_mainmenu
 * Menu principal del juego
 */
bool frogger_mainmenu(void);

/* frogger_mainmenu_move
 * Mueve el menu principal
 * 
 * input: Entrada de usuario
 *  */
void frogger_mainmenu_move(INPUT_VALUES input);

/**********************/
/* PAUSEMENU handlers */
/**********************/

/* frogger_pausemenu
 * Menu pausa del juego
 */
bool frogger_pausemenu(void);

/* frogger_pausemenu_move
 * Mueve el menu de pausa
 * 
 * input: Entrada de usuario
 *  */
void frogger_pausemenu_move(INPUT_VALUES input);

/*************************/
/* CHANGESCREEN handlers */
/*************************/

/* frogger_changescreen
 * Pantalla cambio de nivel y stage
 *
 * level: Nivel
 * Stage: Stage
 */
bool frogger_changescreen(uint32_t level, uint32_t stage);

/***********************/
/* LOSTSCREEN handlers */
/***********************/

/* frogger_lostscreen
 * Pantalla final con puntaje
 *
 * score: Puntaje 
 */
bool frogger_lostscreen(uint32_t score);

/**********************/
/* GAMESCREEN handler */
/**********************/

/* frogger_gamescreen
 * Pantalla del juego
 *
 * field: Campo de juego
 * frog: Ranita
 * lifes: Vidas
 * time: Tiempo jugado
 * score: Puntaje
 */
bool frogger_gamescreen(FIELD field, FROG frog, uint32_t lifes, uint32_t time, uint32_t score);

#endif /* FROGGER_OUTPUT_H */
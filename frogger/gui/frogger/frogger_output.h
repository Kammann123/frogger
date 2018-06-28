/* FROGGER_OUTPUT
 * Maneja la interfaz de salida del videojuego
 */

#ifndef FROGGER_OUTPUT_H
#define FROGGER_OUTPUT_H

/* Librerias utilizadas */
#include <stdint.h>

#include "../gui_input.h"

#include "frogger_physics.h"
#include "../gui_types.h"

/* Configuracion de timers de tasks */
#define CHANGESCREEN_TIME       120
#define CHANGESCREEN_TIMER      2
#define CHANGESCREEN_DIV        10

#define PAUSEMENU_TIME          120
#define PAUSEMENU_TIMER         3

#define LOSTSCREEN_TIME         120
#define LOSTSCREEN_TIMER        4

#define TOPSCREEN_TIME          100
#define TOPSCREEN_TIMER         5

/**************/
/* Constantes */
/**************/

typedef enum{
    TOPSCREEN_INIT,
    TOPSCREEN_OP
} TOPSCREEN_STAGES;

typedef enum{
    LOSTSCREEN_INIT,
    LOSTSCREEN_OP
} LOSTSCREEN_STAGES;

typedef enum{
    CHANGESCREEN_INIT,
    CHANGESCREEN_OP
} CHANGESCREEN_STAGES;

typedef enum {
    PAUSEMENU_INIT,
    PAUSEMENU_OP
} PAUSEMENU_STAGES;

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

/*******************/
/* ACCESS handlers */
/*******************/

/* frogger_get_mainmenu_selection
 * Devuelve la opcion actualmente elegida del
 * menu principal
 */
MAINMENU_OPTIONS frogger_get_mainmenu_selection(void);

/* frogger_get_pausemenu_selection
 * Devuelve la opcion actualmente elegida del
 * menu de pausa
 */
PAUSEMENU_OPTIONS frogger_get_pausemenu_selection(void);

/******************/
/* HOWTO handlers */
/******************/

/* frogger_howscreen
 * Maneja la actualizacion de pantalla del modo 
 * de tutorial
 */
bool frogger_howscreen(GAME_STAGE* stage);

/* frogger_howscreen_move
 * Controla el movimiento dentro del menu
 * de tutorial
 *
 * stage: Instancia
 * input: Entrada del usuario
 */
void frogger_howscreen_move(GAME_STAGE* stage, EVENT input);

/****************/
/* TOP handlers */
/****************/

/* frogger_topscreen
 * Actualiza la salida
 *
 * stage: Instancia
 */
bool frogger_topscreen(GAME_STAGE* stage);

/* frogger_topscreen_tasks
 * Tareas realizadas durante el topscreen stage
 *
 * stage: Instancia
 */
void frogger_topscreen_tasks(GAME_STAGE* stage);

/* frogger_topscreen_move
 * Mueve entre el menu de top score
 *
 * stage: Instancia
 * input: Entrada del usuario
 */
void frogger_topscreen_move(GAME_STAGE* stage, INPUT_VALUES input);

/*******************/
/* SCREEN handlers */
/*******************/

/* frogger_screen_close
 * Cierra aquello que use para la pantalla
 */
void frogger_screen_close(GAME_STAGE* stage);

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

/* frogger_pausemenu_tasks
 * Maneja las tareas durante menu de pausa
 *
 * stage: Estado del juego
 */
void frogger_pausemenu_tasks(GAME_STAGE *stage);

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

/* frogger_changescreen_tasks
 * Maneja las tareas de espera durante
 * el cambio de nivel
 *
 * stage: Estado del juego
 */
void frogger_changescreen_tasks(GAME_STAGE *stage);

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

/* frogger_lostscreen_tasks
 * Maneja las tareas al perder
 *
 * stage: Estado del juego
 */
void frogger_lostscreen_tasks(GAME_STAGE *stage);

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

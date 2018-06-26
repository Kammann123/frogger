/* RPI_OUTPUT
 * Maneja la interfaz de visualizacion en raspberry pi
 */

#include "../frogger_output.h"

#include "disdrv.h"

/*******************/
/* SCREEN handlers */
/*******************/

/* frogger_screen_close */
void frogger_screen_close(void);

/*********************/
/* MAINMENU handlers */
/*********************/

/* frogger_mainmenu */
bool frogger_mainmenu(void);

/* frogger_mainmenu_move */
void frogger_mainmenu_move(INPUT_VALUES input);

/**********************/
/* PAUSEMENU handlers */
/**********************/

/* frogger_pausemenu */
bool frogger_pausemenu(void);

/* frogger_pausemenu_move */
void frogger_pausemenu_move(INPUT_VALUES input);

/*************************/
/* CHANGESCREEN handlers */
/*************************/

/* frogger_changescreen */
bool frogger_changescreen(uint32_t level, uint32_t stage);

/***********************/
/* LOSTSCREEN handlers */
/***********************/

/* frogger_lostscreen */
bool frogger_lostscreen(uint32_t score);

/**********************/
/* GAMESCREEN handler */
/**********************/

/* frogger_gamescreen */
bool frogger_gamescreen(FIELD field, FROG frog, uint32_t lifes, uint32_t time, uint32_t score);
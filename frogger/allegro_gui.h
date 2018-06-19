#ifndef ALLEGRO_GUI_H
#define ALLEGRO_GUI_H

// Poner aqui todos los benditos includes de allegro

#include "game_kernel.h"
#include "game_types.h"
#include <allegro5/allegro.h>

/**********************/
/* Funciones publicas */
/**********************/

/* main_menu_allegro 
 * Muestra el menu principal del juego
 * en la plataforma de rpi
 */
void main_menu_allegro(void);

/* print_frogger_allegro
 * Muestra en la plataforma rpi el juego
 * con todos sus elementos
 *
 * frog: Objeto de la rana
 * lanes: Lista de carriles
 */
void print_frogger_allegro(FROG_CLASS* frog, LANE_LIST* lanes, uint16_t listSize);

/* pause_menu_allegro
 * Muestra un menu de pausa del juego
 * en la plataforma rpi
 */
uint16_t pause_menu_allegro(void);

/* input_keyboard
 * Lee el estado del joystick 
 */
uint16_t input_keyboard(void);

/* allegro_init
 * Inicializa lo necesario para utilizar la plataforma rpi
 */
bool allegro_init(void);

/* allegro_close
 * Cierra la plataforma
 */
void allegro_close(void);

#endif /* ALLEGRO_GUI_H */


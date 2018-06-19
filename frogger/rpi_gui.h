#ifndef RPI_GUI_H
#define RPI_GUI_H

#include <stdint.h>
#include "disdrv.h"
#include "joydrv.h"
#include "game_kernel.h"

/**********************/
/* Funciones publicas */
/**********************/

/* main_menu_rpi 
 * Muestra el menu principal del juego
 * en la plataforma de rpi
 */
void main_menu_rpi(void);

/* print_frogger_rpi
 * Muestra en la plataforma rpi el juego
 * con todos sus elementos
 *
 * frog: Objeto de la rana
 * lanes: Lista de carriles
 */
void print_frogger_rpi(FROG_CLASS* frog, LANE_LIST* lanes, uint16_t listSize);

/* pause_menu_rpi
 * Muestra un menu de pausa del juego
 * en la plataforma rpi
 */
uint16_t pause_menu_rpi(void);

/* input_joystick
 * Lee el estado del joystick 
 */
uint16_t input_joystick(void);

/* rpi_init
 * Inicializa lo necesario para utilizar la plataforma rpi
 */
void rpi_init(void);

/* rpi_close
 * Cierra la plataforma
 */
void rpi_close(void);

#endif /* RPI_GUI_H */


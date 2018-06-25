#ifndef RPI_GUI_H
#define RPI_GUI_H

#include "game_kernel.h"
#include "disdrv.h"
#include "joydrv.h"
#include "game_types.h"
#include "rpi_input.h"

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
void print_frogger_rpi(FROG_CLASS* frog, LANE* lanes);

/* pause_menu_rpi
 * Muestra un menu de pausa del juego
 * en la plataforma rpi
 */
uint16_t pause_menu_rpi(void);

/* input_joystick
 * Lee el estado del joystick 
 */
jcoord_t input_joystick(void);




/* print_display_menu
 * imprime en el display el menu
 */

void print_display_menu();



#endif /* RPI_GUI_H */


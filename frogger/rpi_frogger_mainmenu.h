#ifndef RPI_FROGGER_MAINMENU_H
#define RPI_FROGGER_MAINMENU_H

#include <stdio.h>
#include "disdrv.h"
#include "joydrv.h"
#include "game_types.h"
#include "rpi_input.h"

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* rpi_frogger_mainmenu
 * Muestra en el display el menu principal 
 * del juego frogger
 * 
 * options: Puntero al arreglo de mensajes
 * optionSelected: Opcion seleccionada actualmente
 */

void rpi_frogger_mainmenu(const char** options, uint16_t optionSelected);


#endif /* RPI_FROGGER_MAINMENU_H */


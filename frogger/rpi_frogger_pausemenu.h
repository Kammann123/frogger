#ifndef RPI_FROGGER_PAUSEMENU_H
#define RPI_FROGGER_PAUSEMENU_H

#include <stdio.h>
#include "disdrv.h"
#include "joydrv.h"
#include "game_types.h"
#include "rpi_input.h"

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* rpi_frogger_pausemenu
 * Muestra en el display el menu de pausa 
 * del juego frogger
 * 
 * options: Puntero al arreglo de mensajes
 * optionSelected: Opcion seleccionada actualmente
 */

void rpi_frogger_pausemenu(const char** options, uint16_t optionSelected);

#endif /* RPI_FROGGER_PAUSEMENU_H */


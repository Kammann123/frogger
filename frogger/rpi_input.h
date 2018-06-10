#ifndef RPI_INPUT_H
#define RPI_INPUT_H

#include <stdint.h>
#include <stdbool.h>
#include "joydrv.h"
#include "input.h"

/**********************/
/*    Coordenadas     */
/**********************/
#define RIGHT 128
#define LEFT -128
#define UP 128
#define DOWN -128
#define QUIET 0

/**********************/
/*        Macros      */
/**********************/
#define ABS(x) ( ((x) < 0 ) ? -(x) : (x))

/**********************/
/* Funciones publicas */
/**********************/

/*joystick_init
 *Inicializa configuraciones
 * de joystick
 */
void joystick_init(void);

/*joystick_state
 *Actualiza el estado de
 *entrada del joystick
 */
uint16_t joystick_state(jcoord_t* coords, jswitch_t* click);





#endif /* RPI_INPUT_H */


#ifndef RPI_INPUT_H
#define RPI_INPUT_H
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
<<<<<<< HEAD
#include <stdlib.h>

#include "joydrv.h"
#include "disdrv.h"
#include "gui/gui_events/gui_events.h"
#include "gui/gui_input/gui_input.h"
=======
#include "joydrv.h"
#include "input.h"
#include "gui_events.h"
#include "gui_input.h"
>>>>>>> f5d5fc0fb1a57cb6e21ff043148672adea3fb906

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
bool joystick_input_init(void);

/*joystick_state
 *Actualiza el estado de
 *entrada del joystick
 */
<<<<<<< HEAD
bool joystick_state(EVENT* event, void* none);
=======
bool joystick_state(jcoord_t* coords, jswitch_t* click);




>>>>>>> f5d5fc0fb1a57cb6e21ff043148672adea3fb906

#endif /* RPI_INPUT_H */


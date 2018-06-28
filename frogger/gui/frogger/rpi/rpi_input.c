/* RPI_INPUT
 * Maneja las entradas de usuario en la interfaz
 * en raspberry pi
 */

#include <stdlib.h>

#include "../../gui_input.h"
#include "joydrv.h"

/* Configuracion */

#define JOYSTICK_PRESET 40

/****************/
/* Definiciones */
/****************/

/* gui_input_init */
bool gui_input_init(void){
    /* Inicializo el joystick */
    joy_init();

    /* Configuro joystick */
    set_joy_axis(JOY_NORMAL);

    return true;
}

/* gui_input_close */
void gui_input_close(void){
    return;
}

/* gui_input_source */
bool gui_input_source(EVENT* event, void* none){
    /* Variables de control */
    static EVENT previousEvent;

    /* Variables temporales */
    jcoord_t coords;
    jswitch_t click;

    /* Actualizo los datos del joystick */
    joystick_update();

    /* Preparo el evento */
    event->source = RPI_INPUT_SOURCE;

    /* Busco los datos */
    coords = joystick_get_coord();
    click = joystick_get_switch_value();

    /* Si hubo click del joystick */
    if( click == J_PRESS ){

        /* Mando evento */
        event->type = ACTION_EVENT;
        event->data = ENTER;
    }else{

        /* Verifico estados */
        if( (abs(coords.x) > JOYSTICK_PRESET) && (abs(coords.y) > JOYSTICK_PRESET) ){
            previousEvent.data = MOVE_NONE;
            return false;
        }else if( (abs(coords.x) < JOYSTICK_PRESET) && (abs(coords.y) < JOYSTICK_PRESET) ){
            previousEvent.data = MOVE_NONE;
            return false;
        }else{
            /* Armo el evento */
            event->type = MOVEMENT_EVENT;

            /* Seteo la direccion */
            if( coords.x > JOYSTICK_PRESET ){
                event->data = MOVE_RIGHT;
            }else if( coords.x < (-JOYSTICK_PRESET) ){
                event->data = MOVE_LEFT;
            }else if( coords.y > JOYSTICK_PRESET ){
                event->data = MOVE_UP;
            }else{
                event->data = MOVE_DOWN;
            }
        }
    }

    /* Compruebo que no se repita el evento */
    if( !memcmp(event, &previousEvent, sizeof(EVENT)) ){
        return false;
    }

    /* Me guardo el evento */
    previousEvent = *event;

    /* Mandar el evento */
    return true;
}

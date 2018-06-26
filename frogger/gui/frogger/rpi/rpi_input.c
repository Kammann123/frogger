/* RPI_INPUT
 * Maneja las entradas de usuario en la interfaz
 * en raspberry pi
 */

#include <stdlib.h>

#include "../../gui_input.h"
#include "joydrv.h"

/* Configuracion */

#define JOYSTICK_PRESET 70

/****************/
/* Definiciones */
/****************/

/* gui_input_init */
bool gui_input_init(void){
    /* Inicializo el joystick */
    joy_init();
    
    /* Configuro joystick */
    set_joy_axis(JOY_NORMAL);
}

/* gui_input_close */
void gui_input_close(void);

/* gui_input_source */
bool gui_input_source(EVENT* event, void* none){
    /* Variables temporales */
    jcoord_t coords;
    jswitch_t click;
    
    /* Actualizo los datos del joystick */
    joystick_update();
    
    /* Preparo el evento */
    event->source = RPI_INPUT_SOURCE;
    
    /* Si hubo click del joystick */
    if( click == J_PRESS ){
        
        /* Mando evento */
        event->type = ACTION_EVENT;
        event->data = ENTER;
        return true;
    }else{
        
        /* Verifico estados */
        if( (abs(coords.x) > JOYSTICK_PRESET) && (abs(coords.y) > JOYSTICK_PRESET) ){
            return false;
        }else if( (abs(coords.x) < JOYSTICK_PRESET) && (abs(coords.y) < JOYSTICK_PRESET) ){
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
            
            return true;
        }
    }
}
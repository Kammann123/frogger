/* 
 * File:   rpi_input.c
 * Author: gonza
 *
 * Created on 10 de junio de 2018, 13:47
 */

#include "rpi_input.h"

void joystick_init(void){
	joy_init();
	set_joy_axis(NORMAL);
}

uint16_t joystick_state(jcoord_t* coords, jswitch_t* click){
	uint16_t state = NO_EVENT;
	joystick_update();
	(*coords) = joystick_get_coord();
	(*click) = joystick_get_switch_value();
	if(*click == J_PRESS){
		state = ENTER;
	}
	else if((coords->x) >= QUIET){
		if((coords->x) > (ABS(coords->y))){
			state = MOVE_RIGHT;
		}
		else if((coords->x) < (coords->y)){
			state = MOVE_UP;
		}
		else if((coords->x) > (coords->y)){   //significa que y es mayor que x en cuanto a modulo
			state = MOVE_DOWN;
		}  
	}
	else if(((coords->x) <= QUIET)){
		if(ABS(coords->x) > ABS(coords->y)){
			state = MOVE_LEFT;
		}
		else if(ABS(coords->x) < (coords->y)){
			state = MOVE_UP;
		}
		else if(ABS(coords->x) > (coords->y)){   //significa que y es mayor que x en cuanto a modulo
			state = MOVE_DOWN;
		}  
	}

	return state;
}
/* 
 * File:   rpi_input.c
 * Author: gonza
 *
 * Created on 10 de junio de 2018, 13:47
 */


#include "rpi_input.h"


bool joystick_input_init(void){
	joy_init();
<<<<<<< HEAD
	set_joy_axis(JOY_NORMAL);
=======
	set_joy_axis(NORMAL);
>>>>>>> f5d5fc0fb1a57cb6e21ff043148672adea3fb906
        return true;
}

bool joystick_state(EVENT* event, void* none){
	joystick_update();
<<<<<<< HEAD
	jcoord_t coords = joystick_get_coord();
	jswitch_t click = joystick_get_switch_value();
	
	event->source = RPI_INPUT_SOURCE;
	
	if(click == J_PRESS){
=======
	jcoord_t* coords = joystick_get_coord();
	jswitch_t* click = joystick_get_switch_value();
	
	event->source = RPI_INPUT_SOURCE;
	
	if(*click == J_PRESS){
>>>>>>> f5d5fc0fb1a57cb6e21ff043148672adea3fb906
		event->type = MOVEMENT_EVENT;
		event->data = ENTER;
		return true;
	}
<<<<<<< HEAD
	else if((coords.x) >= QUIET){
		if((coords.x) > (ABS(coords.y))){
			event->type = MOVEMENT_EVENT;
			event->data = MOVE_RIGHT;
		}
		else if((coords.x) < (coords.y)){
			event->type = MOVEMENT_EVENT;
			event->data = MOVE_UP;
		}
		else if((coords.x) > (coords.y)){ 
=======
	else if((coords->x) >= QUIET){
		if((coords->x) > (ABS(coords->y))){
			event->type = MOVEMENT_EVENT;
			event->data = MOVE_RIGHT;
		}
		else if((coords->x) < (coords->y)){
			event->type = MOVEMENT_EVENT;
			event->data = MOVE_UP;
		}
		else if((coords->x) > (coords->y)){ 
>>>>>>> f5d5fc0fb1a57cb6e21ff043148672adea3fb906
			event->type = MOVEMENT_EVENT;//significa que y es mayor que x en cuanto a modulo
			event->data = MOVE_DOWN;
		}  
		return true;
	}
<<<<<<< HEAD
	else if(((coords.x) <= QUIET)){
		if(ABS(coords.x) > ABS(coords.y)){
			event->type = MOVEMENT_EVENT;
			event->data = MOVE_LEFT;
		}
		else if(ABS(coords.x) < (coords.y)){
			event->type = MOVEMENT_EVENT;
			event->data = MOVE_UP;
		}
		else if(ABS(coords.x) > (coords.y)){//significa que y es mayor que x en cuanto a modulo
=======
	else if(((coords->x) <= QUIET)){
		if(ABS(coords->x) > ABS(coords->y)){
			event->type = MOVEMENT_EVENT;
			event->data = MOVE_LEFT;
		}
		else if(ABS(coords->x) < (coords->y)){
			event->type = MOVEMENT_EVENT;
			event->data = MOVE_UP;
		}
		else if(ABS(coords->x) > (coords->y)){//significa que y es mayor que x en cuanto a modulo
>>>>>>> f5d5fc0fb1a57cb6e21ff043148672adea3fb906
			event->type = MOVEMENT_EVENT;
			event->data = MOVE_DOWN;
		}  
		return true;
	}

	return false;
}

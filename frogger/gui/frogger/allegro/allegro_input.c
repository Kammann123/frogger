/* ALLEGRO_INPUT
 * Maneja entradas de allegro 
 */

#include "../../gui_input.h"

#include <allegro5/allegro.h>

/***********/
/* Objetos */
/***********/

/* Cola de eventos de allegro */
static ALLEGRO_EVENT_QUEUE* allegroEventQueue;

/**********************/
/* Funciones publicas */
/**********************/

/* gui_input_init */
bool gui_input_init(void){
    
    /* Creo la cola de eventos */
    allegroEventQueue = al_create_event_queue();
    if( allegroEventQueue == NULL ){
        return false;
    }
    
    /* Registro los eventos */
    al_register_event_source(allegroEventQueue, al_get_keyboard_event_source());
    
    return true;
}

/* gui_input_close */
void gui_input_close(void){
    
    /* Destruyo la cola de eventos */
    al_destroy_event_queue(allegroEventQueue);    
}

/* gui_input_source */
bool gui_input_source(EVENT* event, void* none){
    ALLEGRO_EVENT allegroEvent;
    
    /* Inicializo el evento */
    event->source = ALLEGRO_INPUT_SOURCE;
    
    /* Busco un nuevo evento */
    if( al_get_next_event(allegroEventQueue, &allegroEvent) ){
        if( allegroEvent.type == ALLEGRO_EVENT_KEY_DOWN ){
            switch( allegroEvent.keyboard.keycode ){
                case ALLEGRO_KEY_UP:
                    event->type = MOVEMENT_EVENT;
                    event->data = MOVE_UP;
                    break;
                case ALLEGRO_KEY_DOWN:
                    event->type = MOVEMENT_EVENT;
                    event->data = MOVE_DOWN;
                    break;
                case ALLEGRO_KEY_LEFT:
                    event->type = MOVEMENT_EVENT;
                    event->data = MOVE_LEFT;
                    break;
                case ALLEGRO_KEY_RIGHT:
                    event->type = MOVEMENT_EVENT;
                    event->data = MOVE_RIGHT;
                    break;
                case ALLEGRO_KEY_ENTER:
                    event->type = ACTION_EVENT;
                    event->data = ENTER;
                    break;
                default:
                    return false;
                    break;
            }
            return true;
        }
    }
    
    /* No hubo evento */
    return false;
}
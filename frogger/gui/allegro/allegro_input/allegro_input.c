#include "allegro_input.h"

#include <stdlib.h>
#include <allegro5/events.h>
#include <allegro5/keyboard.h>

/**************************/
/* Objetos de la libreria */
/**************************/

/* Cola de eventos de allegro */
static ALLEGRO_EVENT_QUEUE* allegroEventQueue;

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* allegro_input_init */
bool allegro_input_init(void){
    
    /* Creo la cola de eventos */
    allegroEventQueue = al_create_event_queue();
    if( allegroEventQueue == NULL ){
        return false;
    }
    
    /* Registro los eventos */
    al_register_event_source(allegroEventQueue, al_get_keyboard_event_source());
    
    return true;
}

/* allegro_input_close */
void allegro_input_close(void){
    
    /* Destruyo la cola de eventos */
    al_destroy_event_queue(allegroEventQueue);
}

/* allegro_input_event */
EVENT* allegro_input_event(void* null){
    ALLEGRO_EVENT allegroEvent;
    
    /* Inicializo el evento */
    
    /* Busco un nuevo evento */
    if( al_get_next_event(allegroEventQueue, &allegroEvent) ){
        if( allegroEvent.type == ALLEGRO_EVENT_KEY_DOWN ){
            switch( allegroEvent.keyboard.keycode ){
                case ALLEGRO_KEY_UP:
                    break;
                case ALLEGRO_KEY_DOWN:
                    break;
                case ALLEGRO_KEY_LEFT:
                    break;
                case ALLEGRO_KEY_RIGHT:
                    break;
                case ALLEGRO_KEY_ENTER:
                    break;
            }
        }
    }
    
    /* No hubo evento */
    return NULL;
}
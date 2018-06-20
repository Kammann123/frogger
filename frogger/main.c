/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: lucas
 *
 * Created on June 18, 2018, 6:54 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gui/gui_init.h"
#include "gui/gui_input/gui_input.h"
#include "gui/gui_events/gui_events.h"

/*
 * 
 */
int main(int argc, char** argv){
    EVENT_QUEUE* queue;
    EVENT event;
    ALLEGRO_DISPLAY* display;
    
    /* Inicializo la interfaz */
    if( !gui_init() ){
        return 0;
    }
    
    /* Inicializo las entradas */
    if( !gui_input_init() ){
        return 0;
    }
    
    /* Display */
    display = al_create_display(300, 300);
    
    /* Inicializo los eventos */
    queue = create_queue();
    if( !register_source(queue, gui_input_event) ){
        return 0;
    }
    
    /* Inicio la cola de eventos */
    queue_start(queue);
    
    /* Veo los eventos */
    while( true ){
        if( queue_next_event(queue, &event) ){
            switch( event.data ){
                case MOVE_UP:
                    al_clear_to_color( al_map_rgb(0, 0, 0) );
                    al_flip_display();
                    break;
                case MOVE_DOWN:
                    al_clear_to_color( al_map_rgb(255, 0, 0) );
                    al_flip_display();
                    break;
                case MOVE_LEFT:
                    al_clear_to_color( al_map_rgb(0, 255, 0) );
                    al_flip_display();
                    break;
                case MOVE_RIGHT:
                    al_clear_to_color( al_map_rgb(0, 0, 255) );
                    al_flip_display();
                    break;
                case ENTER:
                    al_clear_to_color( al_map_rgb(255, 255, 255) );
                    al_flip_display();
                    break;
            }
        }
    }
    
    /* Cierro la cola de eventos */
    queue_close(queue);
    
    /* Cierro las entradas */
    gui_input_close();
    
    al_destroy_display(display);
}


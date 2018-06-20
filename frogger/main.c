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
#include "gui/gui_timer/gui_timer.h"
#include "gui/allegro/frogger/mainmenu/allegro_frogger_mainmenu.h"
#include "gui/frogger/frogger_mainmenu/frogger_mainmenu.h"

#define CAMBIAR_COLOR 1

/*
 * 
 */
int main(int argc, char** argv){
    EVENT_QUEUE* queue;
    EVENT event;
    ALLEGRO_DISPLAY* display;
    TIMER_QUEUE* timer;
    
    bool state = false;
    
    /* Inicializo la interfaz */
    if( !gui_init() ){
        return 0;
    }
    
    /* Inicializo las entradas */
    if( !gui_input_init() ){
        return 0;
    }
    
    /* Inicializo el timer */
    timer = gui_timer_create();
    if( timer == NULL ){
        return 0;
    }
    
    /* Agrego un evento de timer */
    if( !gui_timer_new_event(timer, 100, CAMBIAR_COLOR) ){
        return 0;
    }
    
    /* Inicio el timer */
    gui_timer_start(timer);
    
    /* Display */
    display = al_create_display(300, 300);
    if( display == NULL ){
        return 0;
        
    }
    /* Inicializo los eventos */
    queue = create_queue();
    if( !register_source(queue, gui_input_source, NULL) ){
        return 0;
    }
    if( !register_source(queue, gui_timer_source, timer) ){
        return 0;
    }
    
    /* Inicio la cola de eventos */
    queue_start(queue);
    
    /* Veo los eventos */
    while( true ){
        if( queue_next_event(queue, &event) ){
            if( event.source == ALLEGRO_INPUT_SOURCE ){
                switch( event.data ){
                    case ENTER:
                        break;
                    default:
                        frogger_mainmenu_move(event.data);
                        break;
                }
            }else if( event.source == TIMER_SOURCE ){
                if( event.data == CAMBIAR_COLOR ){
                    
                    frogger_mainmenu_update();
                }
            }
        }
    }
    
    /* Cierro la cola de eventos */
    queue_close(queue);
    
    /* Cierro las entradas */
    gui_input_close();
    
    /* Destruyo el timer */
    gui_timer_destroy(timer);
    
    al_destroy_display(display);
}


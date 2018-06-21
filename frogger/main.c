#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gui/gui_init.h"
#include "gui/gui_input/gui_input.h"
#include "gui/gui_events/gui_events.h"
#include "gui/gui_timer/gui_timer.h"
#include "gui/allegro/frogger/mainmenu/allegro_frogger_mainmenu.h"
#include "gui/frogger/frogger_mainmenu/frogger_mainmenu.h"
#include "gui/frogger/frogger_pausemenu/frogger_pausemenu.h"
#include "gui/gui_files/gui_files.h"
#include "gui/gui_animation/gui_animation.h"

/**************/
/* Constantes */
/**************/

/* Etapas del flujo del programa */

typedef enum {
    MAINMENU_STAGE, 
    RANKING_STAGE,
    HOWTO_STAGE, 
    FROGGER_STAGE,
    PAUSEMENU_STAGE
} GAME_STAGE;

#define DEFAULT_STAGE MAINMENU_STAGE

/* Eventos del timer */
#define TIMER_DEFINITION    1000
#define REFRESH_FPS         60
#define REFRESH_DISPLAY     0
#define REFRESH_TIME        TIMER_DEFINITION/REFRESH_FPS

/**************/
/* Prototipos */
/**************/

/* switch_input_target 
 * Maneja quien recibe un evento de entrada
 * segun la etapa del flujo del juego
 *
 * stage: Etapa del programa
 * event: Evento de entrada
 */
void switch_input_target(GAME_STAGE* stage, EVENT event);

/* switch_update_target
 * Maneja quien maneja la actualizacion de la pantalla
 * segun la etapa del juego
 * 
 * stage: Etapa del programa
 */
void switch_update_target(GAME_STAGE* stage);

/* *******************************
 * main
 * Funcion principal del programa.
 * *******************************/
int main(int argc, char** argv){
    
    /* Variables */
    TIMER_QUEUE* timer;
    EVENT_QUEUE* queue;
    EVENT event;
    /* Game stage variable */
    GAME_STAGE stage = DEFAULT_STAGE;
    
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
    if( !gui_timer_new_event(timer, REFRESH_TIME, REFRESH_DISPLAY) ){
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
    
    /* Inicio el timer */
    gui_timer_start(timer);
    
    /* Inicio la cola de eventos */
    queue_start(queue);
    
    /* Main loop */
    while( true ){
        /* Pregunto por eventos en la queue */
        if( queue_next_event(queue, &event) ){
            if( event.source == ALLEGRO_INPUT_SOURCE ){
                switch_input_target(&stage, event);
            }else if( event.source == TIMER_SOURCE ){
                if( event.data == REFRESH_DISPLAY ){
                    switch_update_target(&stage);
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
    
}

/***************************/
/* Definicion de funciones */
/***************************/

/* switch_update_target */
void switch_update_target(GAME_STAGE* stage){
    switch(*stage){
        case MAINMENU_STAGE:
            frogger_mainmenu_update();
            break;
        case RANKING_STAGE:
            break;
        case HOWTO_STAGE:
            break;
        case FROGGER_STAGE:
            break;
        case PAUSEMENU_STAGE:
            frogger_pausemenu_update();
            break;
    }
}

/* switch_input_target */
void switch_input_target(GAME_STAGE* stage, EVENT event){
    switch(*stage){
        case MAINMENU_STAGE:
            if( event.type == MOVEMENT_EVENT ){
                frogger_mainmenu_move(event.data);
            }else if( event.type == ACTION_EVENT ){
                
            }
            break;
        case RANKING_STAGE:
            break;
        case HOWTO_STAGE:
            break;
        case FROGGER_STAGE:
            break;
        case PAUSEMENU_STAGE:
            if( event.type == MOVEMENT_EVENT ){
                frogger_pausemenu_move(event.data);
            }else if( event.type == ACTION_EVENT ){
                // SELECCIONO OPCION                
            }
            break;
    }
}
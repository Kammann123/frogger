#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gui/gui_init.h"
#include "gui/gui_input/gui_input.h"
#include "gui/gui_events/gui_events.h"
#include "gui/gui_timer/gui_timer.h"
#include "gui/frogger/frogger_mainmenu/frogger_mainmenu.h"
#include "gui/frogger/frogger_pausemenu/frogger_pausemenu.h"
#include "gui/frogger/frogger_game/frogger_game.h"
#include "gui/gui_files/gui_files.h"
#include "gui/gui_animation/gui_animation.h"

/**************/
/* Constantes */
/**************/

/* Etapas del flujo del programa */

typedef struct{
    uint16_t value;
    bool hasChanged;
} GAME_STAGE;

typedef enum {
    MAINMENU_STAGE, 
    RANKING_STAGE,
    HOWTO_STAGE, 
    FROGGER_STAGE,
    PAUSEMENU_STAGE,
    CLOSING_STAGE
} STAGE_VALUES;

#define DEFAULT_STAGE MAINMENU_STAGE

/* Eventos del timer */
#define TIMER_DEFINITION    1000
#define REFRESH_FPS         60
#define REFRESH_DISPLAY     0
#define REFRESH_TIME        10

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

/* on_mainmenu_enter 
 * Maneja la direccion del programa desde el menu
 * al seleccionar una opcion
 */
void on_mainmenu_enter(GAME_STAGE* stage);

/* on_pausemenu_Enter
 * Maneja la direccion del programa desde
 * el menu de pausa segun la opcion elegida 
 */
void on_pausemenu_enter(GAME_STAGE* stage);

/* on_game_enter
 * Maneja la direccion del programa desde el juego
 * al apretar enter
 */
void on_game_enter(GAME_STAGE* stage);

/* change_stage
 * Permite modificar el estado del programa
 * y realizar tareas secundarias de limpieza
 *
 * stage: Estado del programa
 * newStage: Nuevo estado del programa
 */
void change_stage(GAME_STAGE* stage, uint16_t newStage);

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
    GAME_STAGE stage = {
        .value = DEFAULT_STAGE,
        .hasChanged = false
    };
    
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
    while( stage.value != CLOSING_STAGE ){
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
        
        /* Hago una limpieza si hubo un cambio */
        if( stage.hasChanged ){
            /* Limpio el flag */
            stage.hasChanged = false;
        
            /* Limpio la cola de eventos */
            queue_flush(queue);
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

/* change_stage */
void change_stage(GAME_STAGE* stage, uint16_t newStage){
    /* Cambio el valor del estado */
    stage->value = newStage;
    
    /* Activo flag */
    stage->hasChanged = true;
}

/* on_pausemenu_enter */
void on_pausemenu_enter(GAME_STAGE* stage){
    /* Cambio de etapa el juego */
    switch( frogger_pausemenu_selected() ){
        case PAUSEMENU_RESUME_OPTION:
            frogger_game_continue();
            change_stage(stage, FROGGER_STAGE);
            break;
        case PAUSEMENU_RESTART_OPTION:
            break;
        case PAUSEMENU_EXIT_OPTION:
            frogger_game_close();
            change_stage(stage, CLOSING_STAGE);
            break;
    }
    
    /* Cierro la ventana del menu */
    frogger_pausemenu_close();
}

/* on_game_enter */
void on_game_enter(GAME_STAGE* stage){
    /* Cambio de etapa de juego */
    change_stage(stage, PAUSEMENU_STAGE);
    
    /* Cierro la ventana del juego */
    frogger_game_screen_close();
    
    /* Pauso el juego */
    frogger_game_pause();
}

/* on_mainmenu_enter */
void on_mainmenu_enter(GAME_STAGE* stage){
    /* Cambio de etapa el programa */
    switch( frogger_mainmenu_selected() ){
        case MAINMENU_PLAY_OPTION:
            frogger_game_init();
            change_stage(stage, FROGGER_STAGE);
            break;
        case MAINMENU_RANK_OPTION:
            change_stage(stage, RANKING_STAGE);
            break;
        case MAINMENU_HOWTO_OPTION:
            change_stage(stage, HOWTO_STAGE);
            break;
        case MAINMENU_EXIT_OPTION:
            change_stage(stage, CLOSING_STAGE);
            break;
        default:
            return;
            break;
    }
    
    /* Cierro el mainmenu */
    frogger_mainmenu_close();
}

/* switch_update_target */
void switch_update_target(GAME_STAGE* stage){
    switch(stage->value){
        case MAINMENU_STAGE:
            frogger_mainmenu_update();
            break;
        case RANKING_STAGE:
            break;
        case HOWTO_STAGE:
            break;
        case FROGGER_STAGE:
            frogger_game_screen_update();
            break;
        case PAUSEMENU_STAGE:
            frogger_pausemenu_update();
            break;
    }
}

/* switch_input_target */
void switch_input_target(GAME_STAGE* stage, EVENT event){
    switch(stage->value){
        case MAINMENU_STAGE:
            if( event.type == MOVEMENT_EVENT ){
                frogger_mainmenu_move(event.data);
            }else if( event.type == ACTION_EVENT ){
                if( event.data == ENTER ){
                    on_mainmenu_enter(stage);                    
                }
            }
            break;
        case RANKING_STAGE:
            break;
        case HOWTO_STAGE:
            break;
        case FROGGER_STAGE:
            if( event.type == ACTION_EVENT ){
                if( event.data == ENTER ){
                    on_game_enter(stage);
                }
            }else if( event.type == MOVEMENT_EVENT ){
                frogger_game_move_frog( event.data );
            }
            break;
        case PAUSEMENU_STAGE:
            if( event.type == MOVEMENT_EVENT ){
                frogger_pausemenu_move(event.data);
            }else if( event.type == ACTION_EVENT ){
                if( event.data == ENTER ){
                    on_pausemenu_enter(stage);
                }
            }
            break;
    }
}
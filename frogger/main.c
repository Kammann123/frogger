#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gui/gui_init.h"
#include "gui/gui_input.h"
#include "gui/gui_events/gui_events.h"
#include "gui/gui_timer/gui_timer.h"
#include "gui/gui_files/gui_files.h"
#include "gui/gui_animation/gui_animation.h"
#include "logic/frogger/frogger_kernel.h"
#include "gui/frogger/frogger_output.h"
#include "gui/frogger/frogger_physics.h"

#include "gui/frogger/rpi/bitmaps.h"

/**************/
/* Constantes */
/**************/

/* Etapas del flujo del programa */

typedef struct{
    uint16_t value;
    bool hasChanged;
} GAME_STAGE;

typedef struct{
    GAME_STAGE* stage;
    uint16_t newStage;
} EXTERN_STAGE_DATA;

typedef enum {
    MAINMENU_STAGE,
    RANKING_STAGE,
    HOWTO_STAGE,
    FROGGER_STAGE,
    PAUSEMENU_STAGE,
    LOSTSCREEN_STAGE,
    CHANGESCREEN_STAGE,
    DEAD_STAGE,
    CLOSING_STAGE
} STAGE_VALUES;

#define DEFAULT_GAME_STAGE MAINMENU_STAGE

/* Eventos del timer */
#define TIMER_DEFINITION    1000
#define REFRESH_FPS         100

typedef enum{
    REFRESH_DISPLAY,
    GAME_COUNTER,
    CHANGESCREEN_TIMER
} TIMER_EVENT_IDS;

#define REFRESH_TIME    TIMER_DEFINITION/REFRESH_FPS
#define GAME_TIME       TIMER_DEFINITION
#define CHANGESCREEN_TIME     TIMER_DEFINITION*2

/***********************/
/* GAME_STAGE handlers */
/***********************/

/* default_stage 
 * Crea y devuelve una instancia de GAME_STAGE
 * default, con parametros por defecto
 */
GAME_STAGE default_stage(void);

/* change_stage
 * Permite modificar el estado del programa
 * y realizar tareas secundarias de limpieza
 *
 * stage: Estado del programa
 * newStage: Nuevo estado del programa
 */
void change_stage(GAME_STAGE* stage, uint16_t newStage);

/* switch_tasks_target
 * Maneja quien ejecuta tareas de forma concurrente
 * mientras se esperan entradas y eventos de la cola de eventos
 *
 * stage: Etapa del programa
 */
void switch_tasks_target(GAME_STAGE* stage);

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

/* on_frogger_event
 * Maneja eventos durante la partida
 *
 * stage: Estado
 * event: Id
 */
void on_frogger_event(GAME_STAGE* stage, uint32_t event);

/**************/
/* :D MAIN :D */
/**************/
int main(int argc, char** argv){

    /* Variables */
    EVENT_QUEUE* queue;
    EVENT event;
    
    /* Game stage variable */
    GAME_STAGE stage = default_stage();
    
    /* Error */
    bool error;

    /* Inicializo la interfaz */
    if( !gui_graphics_init( ) ){
        testing_msg("No se pudo inicializar graficos.");
        return 0;
    }

    /* Inicializo las entradas */
    if( !gui_input_init() ){
        testing_msg("No se pudo inicializar entradas.");
        return 0;
    }

    /* Inicializo el timer */
    if( !gui_timer_global_init() ){
        testing_msg("No se pudo inicializar el timer.");
        return 0;
    }

    /* Agrego un evento de timer */
    error = gui_timer_new_event(gui_timer_global_get(), REFRESH_TIME, REFRESH_DISPLAY);
    error &= gui_timer_new_event(gui_timer_global_get(), GAME_TIME, GAME_COUNTER);
    error &= gui_timer_new_event(gui_timer_global_get(), CHANGESCREEN_TIME, CHANGESCREEN_TIMER);
    if( !error ){
        testing_msg("No se pudo agregar eventos de timer");
        return 0;
    }
            
    /* Inicializo los eventos */
    queue = create_queue();
    error = register_source(queue, gui_input_source, NULL);
    error &= register_source(queue, gui_timer_source, gui_timer_global_get());
    if( !error ){
        testing_msg("No se pudo registrar el timer en la cola de eventos.");
        return 0;
    }

    /* Inicio el timer */
    gui_timer_start(gui_timer_global_get());

    /* Inicio la cola de eventos */
    queue_start(queue);

    /* Main loop */
    while( stage.value != CLOSING_STAGE ){
        /* Pregunto por eventos en la queue */
        if( queue_next_event(queue, &event) ){
            if( event.source == ALLEGRO_INPUT_SOURCE || event.source == RPI_INPUT_SOURCE ){
                testing_msg("Evento de entrada recibido.");
                switch_input_target(&stage, event);
            }else if( event.source == TIMER_SOURCE ){
                if( event.data == REFRESH_DISPLAY ){
                    switch_update_target(&stage);
                }else if( event.data == GAME_COUNTER ){
                    if( stage.value == FROGGER_STAGE ){
                        frogger_time_count();
                    }
                }else if( event.data == CHANGESCREEN_TIMER ){
                    if( stage.value == CHANGESCREEN_STAGE ){
                        change_stage(&stage, FROGGER_STAGE);
                        frogger_game_continue();
                    }
                }
                /* Limpio el timer */
                gui_timer_clear(gui_timer_global_get(), event.data);
            }
        }

        /* Algunos controles mientras */
        switch_tasks_target(&stage);

        /* Hago una limpieza si hubo un cambio */
        if( stage.hasChanged ){
            /* Limpio el flag */
            stage.hasChanged = false;

            /* Limpio la cola de eventos */
            queue_flush(queue);

            /* Limpio el timer */
            gui_timer_clear(gui_timer_global_get(), REFRESH_DISPLAY);
            gui_timer_clear(gui_timer_global_get(), GAME_COUNTER);
            gui_timer_clear(gui_timer_global_get(), CHANGESCREEN_TIMER);
        }
    }

    /* Cierro la cola de eventos */
    queue_close(queue);

    /* Cierro las entradas */
    gui_input_close();

    /* Destruyo el timer */
    gui_timer_global_close();
}

/***********************/
/* GAME_STAGE handlers */
/***********************/

/* default_stage */
GAME_STAGE default_stage(void){
    GAME_STAGE stage = {
        .value = DEFAULT_GAME_STAGE,
        .hasChanged = false
    };
    
    return stage;
}

/* change_stage */
void change_stage(GAME_STAGE* stage, uint16_t newStage){
    /* Cambio el valor del estado */
    stage->value = newStage;

    /* Activo flag */
    stage->hasChanged = true;
}

/***************************/
/* Definicion de funciones */
/***************************/

/* on_frogger_event */
void on_frogger_event(GAME_STAGE* stage, uint32_t event){
    switch( event ){
        case FROGGER_HAS_LOST:
            frogger_game_close();
            change_stage(stage, LOSTSCREEN_STAGE);
            break;
        case FROGGER_HAS_WON:
            frogger_game_pause();
            change_stage(stage, CHANGESCREEN_STAGE);
            break;
        case FROGGER_HAS_DIED: case FROGGER_HAS_DROWN:
            frogger_game_dead_animation();
            change_stage(stage, DEAD_STAGE);
            break;
    }
}


/* on_pausemenu_enter */
void on_pausemenu_enter(GAME_STAGE* stage){
    /* Cambio de etapa el juego */
    switch( pausemenuSelection ){
        case RESUME_OPTION:
            frogger_game_continue();
            change_stage(stage, FROGGER_STAGE);
            break;
        case RESTART_OPTION:
            frogger_game_close();
            frogger_restart();
            if( !frogger_game_init() ){
                change_stage(stage, MAINMENU_STAGE);
            }else{
                change_stage(stage, FROGGER_STAGE);
            }
            break;
        case PAUSE_EXIT_OPTION:
            frogger_game_close();
            change_stage(stage, MAINMENU_STAGE);
            break;
    }
}

/* on_game_enter */
void on_game_enter(GAME_STAGE* stage){
    /* Cambio de etapa de juego */
    change_stage(stage, PAUSEMENU_STAGE);

    /* Pauso el juego */
    frogger_game_pause();
}

/* on_mainmenu_enter */
void on_mainmenu_enter(GAME_STAGE* stage){
    /* Cambio de etapa el programa */
    switch( mainmenuSelection ){
        case PLAY_OPTION:
            if( frogger_game_init() ){
                frogger_restart();
                change_stage(stage, FROGGER_STAGE);
            }
            break;
        case RANK_OPTION:
            change_stage(stage, RANKING_STAGE);
            break;
        case HOWTO_OPTION:
            change_stage(stage, HOWTO_STAGE);
            break;
        case MAIN_EXIT_OPTION:
            frogger_screen_close();
            change_stage(stage, CLOSING_STAGE);
            break;
        default:
            return;
            break;
    }
}

/* switch_tasks_target */
void switch_tasks_target(GAME_STAGE* stage){
    switch(stage->value){
        case FROGGER_STAGE:
            on_frogger_event(stage, frogger_flow());
            break;
        case DEAD_STAGE:
            if( frogger_game_is_frog_static() ){
                frogger_game_reset_frog_position();
                change_stage(stage, FROGGER_STAGE);
            }
            break;
    }
}

/* switch_update_target */
void switch_update_target(GAME_STAGE* stage){
    switch(stage->value){
        case MAINMENU_STAGE:
            frogger_mainmenu();
            break;
        case CHANGESCREEN_STAGE:
            frogger_changescreen(frogger_get_level(), frogger_get_stage());
            break;
        case RANKING_STAGE:
            break;
        case HOWTO_STAGE:
            break;
        case FROGGER_STAGE: case DEAD_STAGE:
            frogger_gamescreen(field, frog, frogger_get_level(), frogger_get_time(), frogger_get_stage());
            break;
        case PAUSEMENU_STAGE:
            frogger_pausemenu();
            break;
        case LOSTSCREEN_STAGE:
            frogger_lostscreen(frogger_get_score());
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
        case LOSTSCREEN_STAGE:
            if( event.type == ACTION_EVENT ){
                if( event.data == ENTER ){
                    frogger_lostscreen(frogger_get_score());
                    change_stage(stage, MAINMENU_STAGE);
                }
            }
            break;
    }
}

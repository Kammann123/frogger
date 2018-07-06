#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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

#include "bitmaps/bitmaps.h"
#include "score_board/score_board.h"

#include "console_args/console_args.h"

/**************/
/* Constantes */
/**************/

/* Etapas del flujo del programa */

typedef enum{
    FROGGER_TASKS_INIT,
    FROGGER_TASKS_OP,
} FROGGER_TASKS_STAGES;

/* Eventos del timer */
#define TIMER_DEFINITION    1000
#define REFRESH_FPS         100

#define REFRESH_DISPLAY     0
#define GAME_COUNTER        1

#define REFRESH_TIME    TIMER_DEFINITION/REFRESH_FPS
#define GAME_TIME       TIMER_DEFINITION

/****************/
/* TASKS thread */
/****************/

/* tasks_thread
 * Thread encargado de manejar las tareas
 * de cada etapa del programa, de caracter logicas,
 * sin manejar I/O
 */
void* tasks_thread(void* gameStage);

/*******************/
/* TARGET handlers */
/*******************/

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

/********************/
/* FROGGER handlers */
/********************/

/* frogger_tasks
 * Maneja las tareas del juego que no sean I/O
 *
 * stage: Estado del juego
 */
void frogger_tasks(GAME_STAGE* stage);

/* on_frogger_event
 * Maneja eventos durante la partida
 *
 * stage: Estado
 * event: Id
 */
bool on_frogger_event(GAME_STAGE* stage, uint32_t event);

/******************/
/* ENTER handlers */
/******************/

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

/**************/
/* :D MAIN :D */
/**************/
int main(int argc, char** argv){

    /* Tasks thread */
    pthread_t tasksThread;

    /* Variables */
    EVENT_QUEUE* queue;
    EVENT event;

    /* Game stage variable */
    GAME_STAGE stage;

    /* Interprete de comandos consola */
    ENTRIES entries;

    char* username = NULL;
    uint32_t characterId = 0;
    bool error;

    /* Leo las entradas de consola */
    if( read_entries(&entries, &argv[1], argc-1) ){
        /* Si no hubo error de sintaxis y se cargaron los comandos */
        username = entries_get_string(&entries, "name");
        entries_get_int(&entries, "character", &characterId);
    }

    /* Inicializo estructura estado del juego */
    stage = stage_init(username, characterId);

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
        gui_input_close();
        return 0;
    }

    /* Agrego un evento de timer */
    error = gui_timer_new_event(gui_timer_global_get(), REFRESH_TIME, REFRESH_DISPLAY, true);
    error &= gui_timer_new_event(gui_timer_global_get(), GAME_TIME, GAME_COUNTER, false);
    error &= gui_timer_new_event(gui_timer_global_get(), CHANGESCREEN_TIME, CHANGESCREEN_TIMER, false);
    error &= gui_timer_new_event(gui_timer_global_get(), PAUSEMENU_TIME, PAUSEMENU_TIMER, false);
    error &= gui_timer_new_event(gui_timer_global_get(), LOSTSCREEN_TIME, LOSTSCREEN_TIMER, false);
    error &= gui_timer_new_event(gui_timer_global_get(), TOPSCREEN_TIME, TOPSCREEN_TIMER, false);
    error &= gui_timer_new_event(gui_timer_global_get(), INFOSCREEN_TIME, INFOSCREEN_TIMER, false);
    if( !error ){
        testing_msg("No se pudo agregar eventos de timer");
        gui_timer_global_close();
        gui_input_close();
        return 0;
    }

    /* Inicializo los eventos */
    queue = create_queue();
    error = register_source(queue, gui_input_source, NULL);
    error &= register_source(queue, gui_timer_source, gui_timer_global_get());
    if( !error ){
        testing_msg("No se pudo registrar el timer en la cola de eventos.");
        gui_timer_global_close();
        gui_input_close();
        return 0;
    }

    /* Inicializo memoria de juego */
    if( !frogger_game_init(&stage) ){
        testing_msg("No se pudo cargar en memoria objetos.");
        gui_timer_global_close();
        gui_input_close();
        queue_close(queue);
        return 0;
    }

    /* Inicio el timer */
    gui_timer_start(gui_timer_global_get());

    /* Inicio la cola de eventos */
    queue_start(queue);

    /* Inicio el thread de tareas */
    pthread_create(&tasksThread, NULL, tasks_thread, &stage);

    /* Main loop */
    while( !is_stage(&stage, CLOSING_STAGE) ){
        /* Pregunto por eventos en la queue */
        if( queue_next_event(queue, &event) ){
            if( event.source == ALLEGRO_INPUT_SOURCE || event.source == RPI_INPUT_SOURCE ){
                testing_msg("Evento de entrada recibido.");
                switch_input_target(&stage, event);
            }else if( event.source == TIMER_SOURCE ){
                if( event.data == REFRESH_DISPLAY ){
                    switch_update_target(&stage);
                }
                /* Limpio el timer */
                gui_timer_clear(gui_timer_global_get(), event.data);
            }
        }

        /* Hago una limpieza si hubo un cambio */
        if( stage.hasChanged ){
            /* Limpio el flag */
            stage.hasChanged = false;

            /* Limpio la cola de eventos */
            queue_flush(queue);

            /* Limpio el timer */
            gui_timer_clear_all(gui_timer_global_get());
        }
    }

    /* Antes de empezar a cerrar espero al thread */
    pthread_join(tasksThread, NULL);

    /* Cierro la cola de eventos */
    queue_close(queue);

    /* Cierro las entradas */
    gui_input_close();

    /* Destruyo el timer */
    gui_timer_global_close();

    /* Libero memoria de objetos */
    frogger_game_close();

    /* Cierro interfaz */
    gui_graphics_close();

    return 0;
}

/****************/
/* TASKS thread */
/****************/

/* tasks_thread */
void* tasks_thread(void* gameStage){
    GAME_STAGE* stage = gameStage;

    /* Mientras no se este cerrando el juego */
    while( stage->value != CLOSING_STAGE ){

        /* Ejecuto tareas */
        switch_tasks_target(stage);
    }

    return NULL;
}

/*******************/
/* TARGET handlers */
/*******************/

/* switch_tasks_target */
void switch_tasks_target(GAME_STAGE* stage){
    switch(stage->value){
        case FROGGER_STAGE:
            frogger_tasks(stage);
            break;
        case RANKING_STAGE:
            frogger_topscreen_tasks(stage);
            break;
        case PAUSEMENU_STAGE:
            frogger_pausemenu_tasks(stage );
            break;
        case CHANGESCREEN_STAGE:
            frogger_changescreen_tasks(stage);
            break;
        case LOSTSCREEN_STAGE:
            frogger_lostscreen_tasks(stage);
            break;
        case DEAD_STAGE:
            if( frogger_game_is_frog_static() ){
                frogger_game_reset_frog_position();
                change_stage(stage, INFO_STAGE);
            }
            break;
        case INFO_STAGE:
            frogger_infoscreen_tasks(stage);
            break;
        default:
            return;
            break;
    }
}

/* switch_update_target */
void switch_update_target(GAME_STAGE* stage){
    /* Selecciono estado de juego y eligo funcion de display */
    switch(stage->value){
        case MAINMENU_STAGE:
            frogger_mainmenu();
            break;
        case CHANGESCREEN_STAGE:
            frogger_changescreen(frogger_get_level(), frogger_get_stage());
            break;
        case RANKING_STAGE:
            frogger_topscreen(stage);
            break;
        case HOWTO_STAGE:
            frogger_howscreen(stage);
            break;
        case FROGGER_STAGE: case DEAD_STAGE:
            frogger_gamescreen(stage, frogger_get_field(), frogger_get_frog(), frogger_get_lifes(), frogger_get_time(), frogger_get_score());
            break;
        case PAUSEMENU_STAGE:
            frogger_pausemenu();
            break;
        case LOSTSCREEN_STAGE:
            frogger_lostscreen(frogger_get_score());
            break;
        case INFO_STAGE:
            frogger_infoscreen(stage);
        default:
            return;
            break;
    }
}

/* switch_input_target */
void switch_input_target(GAME_STAGE* stage, EVENT event){
    /* Selecciono a quien mandar entrada segun etapa del juego */
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
            if( event.type == ACTION_EVENT ){
                if( event.data == ENTER ){
                    frogger_screen_close(stage);
                    change_stage(stage, MAINMENU_STAGE);
                }
            }else if( event.type == MOVEMENT_EVENT ){
                frogger_topscreen_move(stage, event.data);
            }
            break;
        case HOWTO_STAGE:
            frogger_howscreen_move(stage, event);
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
                    frogger_screen_close(stage);
                    change_stage(stage, MAINMENU_STAGE);
                    save_score(stage, frogger_get_score());
                }
            }
            break;
        default:
            return;
            break;
    }
}

/********************/
/* FROGGER handlers */
/********************/

/* frogger_tasks */
void frogger_tasks(GAME_STAGE* stage){
    static FROGGER_TASKS_STAGES state = FROGGER_TASKS_INIT;

    /* Manejo segun etapa */
    switch( state ){
        case FROGGER_TASKS_INIT:
            /* Inicializo el timer */
            gui_timer_clear(gui_timer_global_get(), GAME_COUNTER);
            gui_timer_continue(gui_timer_global_get(), GAME_COUNTER);

            /* Paso al otro estado */
            state = FROGGER_TASKS_OP;
            break;
        case FROGGER_TASKS_OP:
            /* Verifico el timer */
            if( gui_timer_overflow(gui_timer_global_get(), GAME_COUNTER)){
                /* Cuento el tiempo */
                frogger_time_count();

                /* Lo limpio */
                gui_timer_clear(gui_timer_global_get(), GAME_COUNTER);
            }

            /* Controlo el flujo del videojuego */
            if( on_frogger_event(stage, frogger_flow()) ){

                /* Si hubo algun evento cambio de estado */
                state = FROGGER_TASKS_INIT;

                /* Pauso por ahora el timer */
                gui_timer_clear(gui_timer_global_get(), GAME_COUNTER);
                gui_timer_pause(gui_timer_global_get(), GAME_COUNTER);
            }
            break;
    }
}

/* on_frogger_event */
bool on_frogger_event(GAME_STAGE* stage, uint32_t event){
    switch( event ){
        case FROGGER_HAS_LOST:
            frogger_game_pause();
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
        default:
            return false;
            break;
    }
    return true;
}

/******************/
/* ENTER handlers */
/******************/

/* on_pausemenu_enter */
void on_pausemenu_enter(GAME_STAGE* stage){
    /* Cambio de etapa el juego */
    switch( frogger_get_pausemenu_selection() ){
        case RESUME_OPTION:
            frogger_game_start();
            change_stage(stage, FROGGER_STAGE);
            break;
        case RESTART_OPTION:
            if( frogger_game_restart() ){
                change_stage(stage, FROGGER_STAGE);
            }else{
                change_stage(stage, FROGGER_STAGE);
            }
            break;
        case PAUSE_EXIT_OPTION:
            change_stage(stage, MAINMENU_STAGE);
            break;
        default:
            return;
            break;
    }

    /* Cierro la pantalla */
    frogger_screen_close(stage);
}

/* on_game_enter */
void on_game_enter(GAME_STAGE* stage){
    /* Pauso el juego */
    frogger_game_pause();

    /* Cambio de etapa de juego */
    change_stage(stage, PAUSEMENU_STAGE);
}

/* on_mainmenu_enter */
void on_mainmenu_enter(GAME_STAGE* stage){
    /* Cambio de etapa el programa */
    switch( frogger_get_mainmenu_selection() ){
        case PLAY_OPTION:
            if( frogger_game_restart() ){
                change_stage(stage, INFO_STAGE);
            }
            break;
        case RANK_OPTION:
            change_stage(stage, RANKING_STAGE);
            break;
        case HOWTO_OPTION:
            change_stage(stage, HOWTO_STAGE);
            break;
        case MAIN_EXIT_OPTION:
            change_stage(stage, CLOSING_STAGE);
            break;
        default:
            return;
            break;
    }
}

/* RPI_OUTPUT
 * Maneja la interfaz de visualizacion en raspberry pi
 */

#include "../frogger_output.h"
#include "../../gui_animation/gui_animation.h"
#include "rpi_graphics.h"
#include "../../gui_timer/gui_timer.h"

#include "../../gui_types.h"

#include "disdrv.h"

/* Configuraciones generales */

#define MAX_STRING      512

/**********************/
/* Funciones privadas */
/**********************/

/* frogger_pausemenu_close
 * Cierra el menu de pause
 */
void frogger_pausemenu_close(void);

/*******************/
/* SCREEN handlers */
/*******************/

/* frogger_screen_close */
void frogger_screen_close(GAME_STAGE* stage){
    if( is_stage(stage, PAUSEMENU_STAGE) ){
        frogger_pausemenu_close();
    }
}

/*********************/
/* MAINMENU handlers */
/*********************/

/* Configuracion */
#define MAINMENU_PATH   "gui/frogger/rpi/mainmenu/"
#define MAINMENU_IMAGE  MAINMENU_PATH "mainmenu_"

/* Variable del menu principal */
MAINMENU_OPTIONS mainmenuSelection = PLAY_OPTION;

/* frogger_mainmenu */
bool frogger_mainmenu(void){
    BITMAP* bitmap;
    char text[MAX_STRING];

    /* Armo el nombre del archivo a cargar */
    sprintf(text, "%s%d.bmp", MAINMENU_IMAGE, mainmenuSelection);

    /* Cargo el bitmap */
    bitmap = rpi_load_bitmap(text);
    if( bitmap == NULL ){
        return false;
    }

    /* Imprimo el menu */
    if( !rpi_draw_bitmap(bitmap, map_position(0, 0)) ){
        return false;
    }

    /* Actualizo la pantalla */
    rpi_display_update();

    /* Libero el bitmap */
    destroy_bitmap(bitmap);

    /* Exitoso! */
    return true;

}

/* frogger_mainmenu_move */
void frogger_mainmenu_move(INPUT_VALUES input){
    switch( input ){
        case MOVE_UP:
            if( mainmenuSelection != PLAY_OPTION ){
                mainmenuSelection -= 1;
            }
            break;
        case MOVE_DOWN:
            if( mainmenuSelection != MAIN_EXIT_OPTION ){
                mainmenuSelection += 1;
            }
            break;
        default:
            break;
    }
}

/**********************/
/* PAUSEMENU handlers */
/**********************/

/* Configuracion */
#define PAUSEMENU_PATH   "gui/frogger/rpi/pausemenu/"
#define PAUSEOPTION_IMAGE  PAUSEMENU_PATH "pauseoption_"
#define PAUSEBACK_IMAGE  PAUSEMENU_PATH "pauseback_"

#define RESUME_X    0
#define RESUME_Y    5
#define RESTART_X   0
#define RESTART_Y   5
#define EXIT_X      0
#define EXIT_Y      5

/* Variable del menu de pausa */
PAUSEMENU_OPTIONS pausemenuSelection = RESUME_OPTION;

/* Mensajes desplazables */
static MOTION_BMP* options[3] = { NULL, NULL, NULL };
static POSITION initPos[] = {
    { .x=RESUME_X, .y=RESUME_Y },
    { .x=RESTART_X, .y=RESTART_Y },
    { .x=EXIT_X, .y=EXIT_Y }
};

/* frogger_pausemenu_tasks */
void frogger_pausemenu_tasks(GAME_STAGE *stage){
    static PAUSEMENU_STAGES state = PAUSEMENU_INIT;
    uint32_t i;
    char str[MAX_STRING];

    /* Segun la etapa */
    switch( state ){
        case PAUSEMENU_INIT:
            /* Inicializo mensajes */
            for(i = 0;i < 3;i++){
                /* Armo el filename */
                sprintf(str, "%s%d.bmp", PAUSEOPTION_IMAGE, i);
                /* Creo el motion bmp */
                options[i] = rpi_create_motion_bmp();
                /* Inicializo */
                rpi_init_motion_bmp(options[i], str, initPos[i]);
            }

            /* Paso al otro estado */
            state = PAUSEMENU_OP;

            /* Inicializo los timers */
            gui_timer_clear(gui_timer_global_get(), PAUSEMENU_TIMER);
            gui_timer_continue(gui_timer_global_get(), PAUSEMENU_TIMER);
            break;
        case PAUSEMENU_OP:
            /* Me fijo que esten inicializados los objetos */
            if( options[0] == NULL ){
                state = PAUSEMENU_INIT;
                return;
            }

            /* Espero el overflow */
            if( gui_timer_overflow(gui_timer_global_get(), PAUSEMENU_TIMER) ){
                /* Muevo los mensajes */
                rpi_move_motion_bmp(options[pausemenuSelection]);

                /* Limpio el timer */
                gui_timer_clear(gui_timer_global_get(), PAUSEMENU_TIMER);
            }
            break;
    }
}

/* frogger_pausemenu */
bool frogger_pausemenu(void){
    char str[MAX_STRING];
    BITMAP* bitmap;

    /* Cargo el nombre del bitmap del fondo */
    sprintf(str, "%s%d.bmp", PAUSEBACK_IMAGE, pausemenuSelection);

    /* Limpio el display */
    bitmap = rpi_load_bitmap(str);
    if( bitmap == NULL ){
        return false;
    }

    if( !rpi_draw_bitmap(bitmap, map_position(0, 0)) ){
        destroy_bitmap(bitmap);
        return false;
    }

    /* Imprimo la opcion */
    if( !rpi_draw_motion_bmp(options[pausemenuSelection]) ){
        destroy_bitmap(bitmap);
        return false;
    }

    /* Actualizo el display */
    rpi_display_update();

    /* Exito */
    destroy_bitmap(bitmap);
    return true;
}

/* frogger_pausemenu_move */
void frogger_pausemenu_move(INPUT_VALUES input){
    switch( input ){
        case MOVE_UP:
            if( pausemenuSelection != RESUME_OPTION ){
                pausemenuSelection -= 1;
            }
            break;
        case MOVE_DOWN:
            if( pausemenuSelection != PAUSE_EXIT_OPTION ){
                pausemenuSelection += 1;
            }
            break;
        default:
            break;
    }
}

/* frogger_pausemenu_close */
void frogger_pausemenu_close(void){
    uint32_t i;

    for(i = 0;i < 3;i++){
        rpi_destroy_motion_bmp(options[i]);
    }
}

/*************************/
/* CHANGESCREEN handlers */
/*************************/

/* frogger_changescreen */
bool frogger_changescreen(uint32_t level, uint32_t stage){
    return false;
}

/***********************/
/* LOSTSCREEN handlers */
/***********************/

/* frogger_lostscreen */
bool frogger_lostscreen(uint32_t score){
    return false;
}

/**********************/
/* GAMESCREEN handler */
/**********************/

/* frogger_gamescreen */
bool frogger_gamescreen(FIELD field, FROG frog, uint32_t lifes, uint32_t time, uint32_t score){
    return false;
}

/* RPI_OUTPUT
 * Maneja la interfaz de visualizacion en raspberry pi
 */

#include "../frogger_output.h"
#include "../../gui_animation/gui_animation.h"
#include "rpi_graphics.h"
#include "../../gui_timer/gui_timer.h"
#include "../../../logic/frogger/frogger_kernel.h"

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

/* frogger_changescreen_close
 * Cierra la pantalla de cambio de nivel
 */
void frogger_changescreen_close(void);

/* frogger_lostscreen_close
 * Cierra la pantalla de cambio de nivel
 */
void frogger_lostscreen_close(void);

/*******************/
/* SCREEN handlers */
/*******************/

/* frogger_screen_close */
void frogger_screen_close(GAME_STAGE* stage){
    if( is_stage(stage, PAUSEMENU_STAGE) ){
        frogger_pausemenu_close();
    }else if( is_stage(stage, CHANGESCREEN_STAGE) ){
        frogger_changescreen_close();
    }else if( is_stage(stage, LOSTSCREEN_STAGE) ){
        frogger_lostscreen_close();
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
    /* Recorro el menu */
    for(i = 0;i < 3;i++){
        /* Verifico inicializado */
        if( options[i] != NULL ){
            /* Destruyo instancia */
            rpi_destroy_motion_bmp(options[i]);

            /* Guardo */
            options[i] = NULL;
        }
    }
}

/*************************/
/* CHANGESCREEN handlers */
/*************************/

/* Configuracion */

#define LEVEL_X     DISPLAY_WIDTH
#define LEVEL_Y     2

#define STAGE_X     DISPLAY_WIDTH
#define STAGE_Y     9

/* Motion texts */
static MOTION_TEXT* levelText = NULL;
static MOTION_TEXT* stageText = NULL;
static LENGTH maxLength = 0;

/* frogger_changescreen_tasks */
void frogger_changescreen_tasks(GAME_STAGE *stage){
    static CHANGESCREEN_STAGES state = CHANGESCREEN_INIT;
    static uint32_t counter = 0;
    char str[MAX_STRING];

    /* Estados */
    switch( state ){
        case CHANGESCREEN_INIT:
            /* Creo el motion text level */
            sprintf(str, "LEVEL %d", frogger_get_level());
            levelText = rpi_load_motion_text(str, map_position(LEVEL_X, LEVEL_Y));
            if( levelText == NULL ){
                return;
            }
            maxLength = strlen(str);

            /* Creo el motion text level */
            sprintf(str, "STAGE %d", frogger_get_stage());
            stageText = rpi_load_motion_text(str, map_position(STAGE_X, STAGE_Y));
            if( stageText == NULL ){
                return;
            }
            if( strlen(str) > maxLength ){
                maxLength = strlen(str);
            }

            /* Offset length */
            maxLength *= 4 + DISPLAY_WIDTH/2    ;

            /* Cambio de estado */
            state = CHANGESCREEN_OP;
            counter = 0;

            /* Inicializo el timer */
            gui_timer_clear(gui_timer_global_get(), CHANGESCREEN_TIMER);
            gui_timer_continue(gui_timer_global_get(), CHANGESCREEN_TIMER);
            break;
        case CHANGESCREEN_OP:
            /* Verifico inicializacion */
            if( levelText == NULL || stageText == NULL ){
                state = CHANGESCREEN_INIT;
                return;
            }

            /* Me fijo si hubo overflow */
            if( gui_timer_overflow(gui_timer_global_get(), CHANGESCREEN_TIMER) ){
                /* Muevo los motion texts */
                rpi_move_motion_text(levelText);
                rpi_move_motion_text(stageText);

                /* Limpio el timer */
                gui_timer_clear(gui_timer_global_get(), CHANGESCREEN_TIMER);

                counter++;
                if( counter > maxLength ){
                    /* Cambio al nuevo nivel */
                    frogger_game_start();
                    change_stage(stage, FROGGER_STAGE);
                    /* Cierro ventana actual */
                    frogger_screen_close(stage);
                    /* Pauso el timer */
                    gui_timer_pause(gui_timer_global_get(), CHANGESCREEN_TIMER);
                    /* Reset variables */
                    maxLength = 0;
                    counter = 0;
                }
            }
            break;
    }
}

/* frogger_changescreen */
bool frogger_changescreen(uint32_t level, uint32_t stage){

    /* Verifico objetos */
    if( levelText == NULL ){
        return false;
    }

    if( stageText == NULL ){
        return false;
    }

    /* Limpio el display */
    rpi_display_clear();

    /* Imprimo objetos */
    rpi_draw_motion_text( levelText );
    rpi_draw_motion_text( stageText );

    /* Actualizo el display */
    rpi_display_update();

    /* Exito */
    return true;
}

/* frogger_changescreen_close */
void frogger_changescreen_close(void){
    /* Verifico level */
    if( levelText != NULL ){
        /* Destruyo la instancia */
        rpi_destroy_motion_text(levelText);

        /* Guardo */
        levelText = NULL;
    }

    /* Verifico stage */
    if( stageText != NULL ){
        /* Destruyo la instancia */
        rpi_destroy_motion_text(stageText);

        /* Guardo */
        stageText = NULL;
    }
}

/***********************/
/* LOSTSCREEN handlers */
/***********************/

/* Configuraciones */
#define GAME_OVER_POS   map_position(DISPLAY_WIDTH, 2)
#define SCORE_POS       map_position(DISPLAY_WIDTH, 9)

/* Objetos motion text */
static MOTION_TEXT* gameOver = NULL;
static MOTION_TEXT* scoreText = NULL;

/* frogger_lostscreen_tasks */
void frogger_lostscreen_tasks(GAME_STAGE *stage){
    static LOSTSCREEN_STAGES state = LOSTSCREEN_INIT;
    char str[MAX_STRING];

    /* Estados */
    switch( state ){
        case LOSTSCREEN_INIT:
            /* Creo los objetos MOTION TEXT */
            gameOver = rpi_load_motion_text("GAME OVER", GAME_OVER_POS);
            if( gameOver == NULL ){
                return;
            }

            sprintf(str, "SCORE %d", frogger_get_score());
            scoreText = rpi_load_motion_text(str, SCORE_POS);
            if( scoreText == NULL ){
                return;
            }

            /* Cambio de estado */
            state = LOSTSCREEN_OP;

            /* Inicializo el timer */
            gui_timer_clear(gui_timer_global_get(), LOSTSCREEN_TIMER);
            gui_timer_continue(gui_timer_global_get(), LOSTSCREEN_TIMER);
            break;
        case LOSTSCREEN_OP:
            /* Verifico inicializacion */
            if( gameOver == NULL || scoreText == NULL ){
                state = LOSTSCREEN_INIT;
                return;
            }

            /* Verifico overflow */
            if( gui_timer_overflow(gui_timer_global_get(), LOSTSCREEN_TIMER) ){

                /* Muevo los motion texts */
                rpi_move_motion_text(gameOver);
                rpi_move_motion_text(scoreText);

                /* Reinicio el timer */
                gui_timer_clear(gui_timer_global_get(), LOSTSCREEN_TIMER);
            }
            break;
    }
}

/* frogger_lostscreen */
bool frogger_lostscreen(uint32_t score){

    /* Verifico objetos */
    if( scoreText == NULL || gameOver == NULL ){
        return false;
    }

    /* Limpio la pantalla */
    if( !rpi_display_clear() ){
        return false;
    }

    /* Imprimo los motions */
    if( !rpi_draw_motion_text(gameOver) ){
        return false;
    }
    if( !rpi_draw_motion_text(scoreText) ){
        return false;
    }

    /* Actualizo display */
    rpi_display_update();

    return true;
}

/* frogger_lostscreen_close */
void frogger_lostscreen_close(void){
    /* Verifico gameOver */
    if( gameOver != NULL ){
        /* Destruyo la instancia */
        rpi_destroy_motion_text(gameOver);

        /* Guardo */
        gameOver = NULL;
    }

    /* Verifico score */
    if( scoreText != NULL ){
        /* Destruyo la instancia */
        rpi_destroy_motion_text(scoreText);

        /* Guardo */
        scoreText = NULL;
    }
}

/**********************/
/* GAMESCREEN handler */
/**********************/

/* frogger_gamescreen */
bool frogger_gamescreen(FIELD field, FROG frog, uint32_t lifes, uint32_t time, uint32_t score){
    return false;
}

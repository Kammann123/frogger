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

/**********************/
/* Funciones privadas */
/**********************/

/* frogger_convert_position
 * Convierte la posicion para adaptarla al tablero o matriz de leds
 *
 * pos: Posicion a adaptarla
 */
static POSITION frogger_convert_position(POSITION pos);

/* frogger_pausemenu_close
 * Cierra el menu de pause
 */
static void frogger_pausemenu_close(void);

/* frogger_changescreen_close
 * Cierra la pantalla de cambio de nivel
 */
static void frogger_changescreen_close(void);

/* frogger_lostscreen_close
 * Cierra la pantalla de cambio de nivel
 */
static void frogger_lostscreen_close(void);

/* frogger_topscreen_close
 * Cerra la ventana de top score
 */
static void frogger_topscreen_close(void);

/* frogger_howscreen_close
 * Cierra la ventana de howto
 */
static void frogger_howscreen_close(void);

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
    }else if( is_stage(stage, RANKING_STAGE) ){
        frogger_topscreen_close();
    }else if( is_stage(stage, HOWTO_STAGE) ){
        frogger_howscreen_close();
    }
}

/******************/
/* HOWTO handlers */
/******************/

/* Configuracion */
#define HOWSCREEN_PATH  "gui/frogger/rpi/howto/"
#define TUTO_OBJFILE    HOWSCREEN_PATH "howto"

#define TUTO_POS        map_position(0, 0)
#define AVOID_ANIM      "AVOID"
#define ENTER_ANIM      "ENTER"
#define OK_ANIM         "OK"

/* Estados del tutorial */
typedef enum{
    HOWTO_AVOID,
    HOWTO_ENTER,
    HOWTO_OK
} TUTO_STEPS;

/* Objetos */
static ANIMATED_OBJECT* tutorial = NULL;
static TUTO_STEPS tutoStep = HOWTO_AVOID;

/* frogger_howscreen */
bool frogger_howscreen(GAME_STAGE* stage){
    BITMAP* bitmap;

    /* Verifico inicializacion */
    if( tutorial == NULL ){
        tutorial = gui_animation_load_object(TUTO_OBJFILE, TUTO_POS, AVOID_ANIM);
        if( tutorial == NULL ){
            return false;
        }
        testing_msg("Cargo el objeto.");

        /* Inicializo etapa */
        tutoStep = HOWTO_AVOID;

        /* Lo agrego al motor */
        if( !gui_animation_attach_engine(frogger_get_engine(), tutorial) ){
            return false;
        }
        testing_msg("Agrego objeto al motor.");
    }

    /* Verifico estado del movimiento */
    if( tutorial->status == GUI_ANIMATION_STATE_STATIC ){

        /* Prendo el motor */
        gui_animation_start_engine(frogger_get_engine());
        testing_msg("Inicio el motor");

        /* Arranco la animacion */
        gui_animation_start_loop(tutorial, AVOID_ANIM);
        testing_msg("Inicio el objeto.");
    }

    /* Cargo el bitmap */
    bitmap = rpi_load_bitmap( gui_animation_get_frame(tutorial) );
    if( bitmap == NULL ){
        return false;
    }
    rpi_draw_bitmap(bitmap, tutorial->pos);
    destroy_bitmap(bitmap);

    testing_msg("Manda objetos al display");

    /* Flip update */
    rpi_display_update();

    return true;
}

/* frogger_howscreen_move */
void frogger_howscreen_move(GAME_STAGE* stage, EVENT input){
    if( input.type == ACTION_EVENT ){
        if( input.data == ENTER ){
            if( tutoStep == HOWTO_OK ){
                change_stage(stage, MAINMENU_STAGE);
                gui_animation_pause_engine(frogger_get_engine());
            }
        }
        return;
    }else if( input.type == MOVEMENT_EVENT ){
        if( input.data == MOVE_LEFT ){
            if( tutoStep > HOWTO_AVOID ){
                tutoStep--;
            }else{
                return;
            }
        }else if( input.data == MOVE_RIGHT ){
            if( tutoStep < HOWTO_OK ){
                tutoStep++;
            }else{
                return;
            }
        }else{
            return;
        }
    }else{
        return;
    }

    /* Cambio animacion */
    switch( tutoStep ){
        case HOWTO_AVOID:
            gui_animation_set_animation(tutorial, AVOID_ANIM);
            break;
        case HOWTO_ENTER:
            gui_animation_set_animation(tutorial, ENTER_ANIM);
            break;
        case HOWTO_OK:
            gui_animation_set_animation(tutorial, OK_ANIM);
            break;
    }
}

/* frogger_howscreen_close */
static void frogger_howscreen_close(void){
    if( tutorial != NULL ){
        gui_animation_destroy_object( tutorial );
    }
}

/**********************/
/* TOPSCREEN handlers */
/**********************/

/* Configuracion */
#define TOP_POS       map_position(DISPLAY_WIDTH, 2)
#define SCORE_POS     map_position(DISPLAY_WIDTH, 9)

/* Motion text objects */
static MOTION_TEXT* topText = NULL;
static MOTION_TEXT* posText = NULL;

/* Posicion del top */
static LENGTH scorePos = 1;
static bool posChanged = false;

/* frogger_topscreen_tasks */
void frogger_topscreen_tasks(GAME_STAGE* stage){
    static TOPSCREEN_STAGES state = TOPSCREEN_INIT;
    STRING str;

    /* Estados */
    switch(state){
        case TOPSCREEN_INIT:
            /* Inicial  izo el titulo */
            topText = rpi_load_motion_text("TOP SCORE", TOP_POS);
            if( topText == NULL ){
                return;
            }
            /* Inicializo posicion de score */
            if( stage->topLength ){
                if( !get_score_position(stage, str, scorePos-1) ){
                    rpi_destroy_motion_text(topText);
                    topText = NULL;
                    return;
                }
                posText = rpi_load_motion_text(str, SCORE_POS);
                if( posText == NULL ){
                    rpi_destroy_motion_text(topText);
                    topText = NULL;
                    return;
                }
            }else{
                posText = rpi_load_motion_text("EMPTY", SCORE_POS);
                if( posText == NULL ){
                    rpi_destroy_motion_text(topText);
                    topText = NULL;
                    return;
                }
            }

            /* Cambio de estado */
            state = TOPSCREEN_OP;

            /* Reinicio el timer */
            gui_timer_clear(gui_timer_global_get(), TOPSCREEN_TIMER);
            gui_timer_continue(gui_timer_global_get(), TOPSCREEN_TIMER);
            break;
        case TOPSCREEN_OP:
            /* Verifico existencia */
            if( topText == NULL || posText == NULL ){
                state = TOPSCREEN_INIT;
                return;
            }

            /* Verifico cambio */
            if( posChanged ){
                /* Reinicio */
                posChanged = false;
                stage = TOPSCREEN_INIT;

                /* Los borro para crear otra vez */
                rpi_destroy_motion_text(topText);
                rpi_destroy_motion_text(posText);
                topText = NULL;
                posText = NULL;

                /* Salgo */
                return;
            }

            /* Verifico overflow de timer */
            if( gui_timer_overflow(gui_timer_global_get(), TOPSCREEN_TIMER) ){
                /* Muevo los objetos */
                rpi_move_motion_text(topText);
                rpi_move_motion_text(posText);

                /* Reinicio el timer */
                gui_timer_clear(gui_timer_global_get(), TOPSCREEN_TIMER);
            }
            break;
    }
}

/* frogger_topscreen */
bool frogger_topscreen(GAME_STAGE* stage){

    /* Verifico objetos */
    if( topText == NULL || posText == NULL ){
        return false;
    }

    /* Limpio la pantalla */
    if( !rpi_display_clear() ){
        return false;
    }

    /* Imprimo los motions */
    if( !rpi_draw_motion_text(topText) ){
        return false;
    }
    if( !rpi_draw_motion_text(posText) ){
        return false;
    }

    /* Actualizo display */
    rpi_display_update();

    return true;
}

/* frogger_topscreen_move */
void frogger_topscreen_move(GAME_STAGE* stage, INPUT_VALUES input){
    switch( input ){
        case MOVE_UP:
            if( scorePos > 1 ){
                scorePos--;
            }else{
                return;
            }
            break;
        case MOVE_DOWN:
            if( scorePos < stage->topLength ){
                scorePos++;
            }else{
                return;
            }
            break;
        default:
            return;
            break;
    }
    /* Flag */
    posChanged = true;
}

/* frogger_topscreen_close */
static void frogger_topscreen_close(void){
    /* Verifico existencia */
    if( topText != NULL ){

        /* Destruyo */
        rpi_destroy_motion_text(topText);

        /* Guardo */
        topText = NULL;
    }
    /* Verifico existencia */
    if( posText != NULL ){

        /* Destruyo */
        rpi_destroy_motion_text(posText);

        /* Guardo */
        posText = NULL;
    }
    /* Pauso el timer */
    gui_timer_pause(gui_timer_global_get(), TOPSCREEN_TIMER);

    /* Reinicio pos */
    scorePos = 1;
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
    STRING text;

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
    STRING str;
    uint32_t i;

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
    STRING str;
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
static void frogger_pausemenu_close(void){
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

    /* Pauso el timer */
    gui_timer_pause(gui_timer_global_get(), PAUSEMENU_TIMER);
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
    static LENGTH counter = 0;
    STRING str;

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
            maxLength = maxLength * 4 + DISPLAY_WIDTH / 2;

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
                    /* Reset variables */
                    maxLength = 0;
                    counter = 0;
                    /* Cambio al nuevo nivel */
                    frogger_game_start();
                    /* Cierro ventana actual */
                    frogger_screen_close(stage);
                    /* Cambio la etapa */
                    change_stage(stage, FROGGER_STAGE);
                    /* Pauso el timer */
                    gui_timer_pause(gui_timer_global_get(), CHANGESCREEN_TIMER);
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
static void frogger_changescreen_close(void){
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
    STRING str;

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
static void frogger_lostscreen_close(void){
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
    /* Pauso el timer */
    gui_timer_pause(gui_timer_global_get(), LOSTSCREEN_TIMER);
}

/**********************/
/* GAMESCREEN handler */
/**********************/

/* Configuraciones */
#define RPI_PATH           "gui/frogger/rpi/"
#define GAMESCREEN_PATH    RPI_PATH "gamescreen/"
#define GAMESCREEN_FIELD   GAMESCREEN_PATH "field.bmp"

#define BLINK_VALUE        3

/* gui_graphics_close */
void gui_graphics_close(void){
    frogger_howscreen_close();
    rpi_display_clear();
    rpi_display_update();
}

/* frogger_gamescreen */
bool frogger_gamescreen(FIELD field, FROG frog, uint32_t lifes, uint32_t time, uint32_t score){
    static LENGTH frogBlink = BLINK_VALUE;
    BITMAP* bitmap;
    LANE lane;
    FROGGER_OBJECT object;
    uint32_t i, ii;

    /* Limpio la ventana */
    bitmap = rpi_load_bitmap( GAMESCREEN_FIELD );
    if( bitmap == NULL ){
        return false;
    }
    if( !rpi_draw_bitmap(bitmap, map_position(0, 0)) ){
        return false;
    }
    destroy_bitmap(bitmap);

    /* Pongo los objetos */
    for(i = 0;i < field.lanesQty;i++){
        lane = field.lanes[i];

        for(ii = 0;ii < lane.objectsQty;ii++){
            object = lane.objects[ii];

            if( object->pos.x < DISPLAY_WIDTH ){
                bitmap = rpi_load_bitmap( gui_animation_get_frame(object) );
                if( bitmap == NULL ){
                  return false;
                }

                if( !rpi_draw_bitmap(bitmap, frogger_convert_position(object->pos)) ){
                    return false;
                }
                destroy_bitmap(bitmap);
            }

        }
    }

    frogBlink--;
    if( !frogBlink ){
        /* Pongo la ranita :) */
        bitmap = rpi_load_bitmap( gui_animation_get_frame(frog.object) );
        if( bitmap == NULL ){
            return false;
        }
        if( !rpi_draw_bitmap(bitmap, frogger_convert_position(frog.object->pos)) ){
            return false;
        }
        destroy_bitmap(bitmap);
        /* Reincio */
        frogBlink = BLINK_VALUE;
    }

    /* Mando el buffer al display */
    rpi_display_update();

    return true;
}

/**********************/
/* Funciones privadas */
/**********************/

/* frogger_convert_position */
static POSITION frogger_convert_position(POSITION pos){
    POSITION newPos;

    /* Guardo posicion */
    newPos = pos;

    /* Adapto la posicion a la estructura de la matriz de leds */
    if( newPos.y < 6 ){
        newPos.y *= 2;
    }else{
        newPos.y += 5;
    }

    return newPos;
}

/*******************/
/* ACCESS handlers */
/*******************/

/* frogger_get_mainmenu_selection */
MAINMENU_OPTIONS frogger_get_mainmenu_selection(void){
    return mainmenuSelection;
}

/* frogger_get_pausemenu_selection */
PAUSEMENU_OPTIONS frogger_get_pausemenu_selection(void){
    return pausemenuSelection;
}

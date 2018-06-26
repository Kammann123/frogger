/* ALLEGRO_OUTPUT
 * Maneja la interfaz de salida del videojuego con allegro
 */

#include "../frogger_output.h"
#include "../../gui_animation/gui_animation.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "../../gui_timer/gui_timer.h"

#include <stdio.h>

/* Configuraciones generales */

#define DISPLAY_WIDTH   640
#define DISPLAY_HEIGHT  480
#define DISPLAY_POS_X   300
#define DISPLAY_POS_Y   100

#define CLEAR_COLOR     al_map_rgb(0, 0, 0)

/***********/
/* Objetos */
/***********/

/* Ventana de allegro */
static ALLEGRO_DISPLAY* display = NULL;

/* Funciones privadas */

/* frogger_back_selector 
 * Selecciona el archivo de fondo del juego
 * 
 * index: Numero de fondo
 */
static char* frogger_back_selector(uint16_t index);

/*******************/
/* SCREEN handlers */
/*******************/

/* frogger_screen_close */
void frogger_screen_close(GAME_STAGE* stage){
    if( display != NULL ){
        al_destroy_display( display );
        display = NULL;
    }
}

/*****************/
/* INFO handlers */
/*****************/

/* frogger_infoscreen */
bool frogger_infoscreen(GAME_STAGE* stage){
    return true;
}

/* frogger_infoscreen_tasks */
void frogger_infoscreen_tasks(GAME_STAGE* stage){
    change_stage(stage, FROGGER_STAGE);
    return;
}

/******************/
/* HOWTO handlers */
/******************/

/* Configuracion */
#define HOWSCREEN_PATH  "gui/frogger/allegro/howtoscreen/"
#define HOWSCREEN_IMAGE HOWSCREEN_PATH "background.png"

/* frogger_howscreen */
bool frogger_howscreen(GAME_STAGE* stage){
    ALLEGRO_BITMAP* bitmap;

    /* Abro el display si no lo esta */
    if( display == NULL ){
        display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
        if( display == NULL ){
            return false;
        }
        al_set_window_position(display, DISPLAY_POS_X, DISPLAY_POS_Y);
    }

    /* Cargo el bitmap */
    bitmap = al_load_bitmap(HOWSCREEN_IMAGE);
    if( bitmap == NULL ){
        return false;
    }
    /* Lo imprimo */
    al_draw_bitmap(bitmap, 0, 0, 0);
    al_destroy_bitmap(bitmap);
    /* Flip display */
    al_flip_display();

    return true;
}

/* frogger_howscreen_move */
void frogger_howscreen_move(GAME_STAGE* stage, EVENT input){
    if( input.type == ACTION_EVENT ){
        if( input.data == ENTER ){
            change_stage(stage, MAINMENU_STAGE);
        }
    }
}

/**********************/
/* TOPSCREEN handlers */
/**********************/

/* Configuracion */
#define TOPSCREEN_PATH  "gui/frogger/allegro/topscreen/"
#define TOPSCREEN_BACK  TOPSCREEN_PATH "background.png"
#define TOPSCREEN_FONT  TOPSCREEN_PATH "Neon.ttf"
#define TOPSCREEN_SIZE  15
#define HIGH_COLOR      al_map_rgb(0, 0, 0)
#define HIGH_X          230
#define HIGH_Y          160
#define HIGH_DY         20

/* frogger_topscreen */
bool frogger_topscreen(GAME_STAGE* stage){
    ALLEGRO_BITMAP* bitmap;
    ALLEGRO_FONT* font;
    LENGTH i;
    STRING str;

    /* Abro ventana si no lo esta */
    if( display == NULL ){
        display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
        if( display == NULL ){
            return false;
        }
        al_set_window_position(display, DISPLAY_POS_X, DISPLAY_POS_Y);
    }

    /* Cargo el fondo */
    bitmap = al_load_bitmap(TOPSCREEN_BACK);
    if( bitmap == NULL ){
        return false;
    }

    /* Lo imprimo */
    al_draw_bitmap(bitmap, 0, 0, 0);

    /* Cargo fuente */
    font = al_load_ttf_font(TOPSCREEN_FONT, TOPSCREEN_SIZE, 0);
    if( font == NULL ){
        al_destroy_bitmap(bitmap);
        return false;
    }

    /* Imprimo los textos */
    for(i = 0;i < stage->topLength;i++){
        /* Cargo la posicion */
        if( !get_score_position(stage, str, i) ){
            al_destroy_font(font);
            al_destroy_bitmap(bitmap);
            return false;
        }

        /* Imprimo */
        al_draw_text(font, HIGH_COLOR, HIGH_X, HIGH_Y + HIGH_DY * i, 0, str);
    }

    /* Flip display */
    al_flip_display();

    /* Exito */
    al_destroy_font(font);
    al_destroy_bitmap(bitmap);
    return true;
}

/* frogger_topscreen_tasks */
void frogger_topscreen_tasks(GAME_STAGE* stage){
    return;
}

/* frogger_topscreen_move */
void frogger_topscreen_move(GAME_STAGE* stage, INPUT_VALUES input){
    return;
}

/*********************/
/* MAINMENU handlers */
/*********************/

/* Configuracion */
#define MAINMENU_PATH   "gui/frogger/allegro/mainmenu/"
#define MAINMENU_IMAGE  MAINMENU_PATH "mainmenu_"

/* Variable del menu principal */
MAINMENU_OPTIONS mainmenuSelection = PLAY_OPTION;

/* frogger_mainmenu */
bool frogger_mainmenu(void){
    ALLEGRO_BITMAP* bitmap = NULL;
    char filename[MAX_STRING];

    /* Inicializo el display */
    if( display == NULL ){
        display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
        if( display == NULL ){
            return false;
        }
        al_set_window_position(display, DISPLAY_POS_X, DISPLAY_POS_Y);
    }

    /* Cargo el bitmap */
    sprintf(filename, "%s%d.png", MAINMENU_IMAGE, mainmenuSelection);
    bitmap = al_load_bitmap(filename);
    if( bitmap == NULL ){
        return false;
    }

    /* Pongo el fondo */
    al_draw_bitmap(bitmap, 0, 0, 0);
    al_flip_display();

    al_destroy_bitmap(bitmap);
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
#define PAUSEMENU_PATH   "gui/frogger/allegro/pausemenu/"
#define PAUSEMENU_IMAGE  PAUSEMENU_PATH "pausemenu_"

/* Variable del menu de pausa */
PAUSEMENU_OPTIONS pausemenuSelection = RESUME_OPTION;

/* frogger_pausemenu_tasks */
void frogger_pausemenu_tasks(GAME_STAGE* stage){
    return;
}

/* frogger_pausemenu */
bool frogger_pausemenu(void){
    ALLEGRO_BITMAP* bitmap = NULL;
    STRING filename;

    /* Inicializo el display */
    if( display == NULL ){
        display = al_create_display(DISPLAY_WIDTH, DISPLAY_WIDTH);
        if( display == NULL ){
            return false;
        }
        al_set_window_position(display, DISPLAY_POS_X, DISPLAY_POS_Y);
    }

    /* Cargo el bitmap */
    sprintf(filename, "%s%d.png", PAUSEMENU_IMAGE, pausemenuSelection);
    bitmap = al_load_bitmap( filename );
    if( bitmap == NULL ){
        return false;
    }

    /* Pongo el fondo */
    al_draw_bitmap(bitmap, 0, 40, 0);
    al_flip_display();

    al_destroy_bitmap(bitmap);
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

/*************************/
/* CHANGESCREEN handlers */
/*************************/

/* Configuracion */
#define CHANGESCREEN_PATH   "gui/frogger/allegro/changescreen/"
#define CHANGESCREEN_IMAGE  CHANGESCREEN_PATH "change.png"
#define CHANGESCREEN_FONT   CHANGESCREEN_PATH "American Captain.otf"
#define CHANGESCREEN_SIZE   120

#define LEVEL_COLOR     al_map_rgb(10, 38, 179)
#define STAGE_COLOR     al_map_rgb(138, 7, 154)
#define LEVEL_X         130
#define LEVEL_Y         200
#define STAGE_X         340
#define STAGE_Y         360

/* frogger_changescreen_tasks */
void frogger_changescreen_tasks(GAME_STAGE *stage){
    static CHANGESCREEN_STAGES state = CHANGESCREEN_INIT;
    static uint32_t counter = 0;

    /* Manejo segun etapa */
    switch( state ){
        case CHANGESCREEN_INIT:

            /* Reinicio contador */
            counter = CHANGESCREEN_DIV;

            /* Cambio de etapa */
            state = CHANGESCREEN_OP;

            /* Limpio e inicializo el timer */
            gui_timer_clear(gui_timer_global_get(), CHANGESCREEN_TIMER);
            gui_timer_continue(gui_timer_global_get(), CHANGESCREEN_TIMER);
            break;
        case CHANGESCREEN_OP:

            /* Espero el timer */
            if( gui_timer_overflow(gui_timer_global_get(), CHANGESCREEN_TIMER) ){

                /* Limpio el timer y lo pauso */
                gui_timer_clear(gui_timer_global_get(), CHANGESCREEN_TIMER);

                /* Cuento */
                counter--;
                if( !counter ){
                    /* Cambio al nuevo nivel */
                    frogger_game_start();
                    change_stage(stage, FROGGER_STAGE);

                    /* Reinicio */
                    counter = CHANGESCREEN_DIV;
                    gui_timer_pause(gui_timer_global_get(), CHANGESCREEN_TIMER);

                    /* Restauro estado init */
                    state = CHANGESCREEN_INIT;
                }
            }
            break;
        default:
            break;
    }
}

/* frogger_changescreen */
bool frogger_changescreen(uint32_t level, uint32_t stage){
    ALLEGRO_BITMAP* bitmap;
    ALLEGRO_FONT* font;
    STRING text;

    /* Abro la ventana */
    if( display == NULL ){
        display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
        if( display == NULL ){
            return false;
        }
        al_set_window_position(display, DISPLAY_POS_X, DISPLAY_POS_Y);
    }

    /* Limpio el fondo */
    al_clear_to_color( CLEAR_COLOR );

    /* Cargo el bitmap */
    bitmap = al_load_bitmap( CHANGESCREEN_IMAGE );
    if( bitmap == NULL ){
        return false;
    }
    al_draw_bitmap(bitmap, 0, 0, 0);

    /* Fuente */
    font = al_load_font(CHANGESCREEN_FONT, CHANGESCREEN_SIZE, 0);
    if( font == NULL ){
        al_destroy_bitmap(bitmap);
        return false;
    }

    /* Nivel */
    sprintf(text, "%d", level);
    al_draw_text(font, LEVEL_COLOR, LEVEL_X, LEVEL_Y, 0, text);

    /* Stage */
    sprintf(text, "%d", stage);
    al_draw_text(font, STAGE_COLOR, STAGE_X, STAGE_Y, 0, text);

    /* Mando el buffer al display */
    al_flip_display();

    al_destroy_bitmap(bitmap);
    al_destroy_font(font);

    return true;
}

/***********************/
/* LOSTSCREEN handlers */
/***********************/

/* Configuracion */
#define LOSTSCREEN_PATH   "gui/frogger/allegro/lostscreen/"
#define LOSTSCREEN_IMAGE  LOSTSCREEN_PATH "lost.png"
#define LOSTSCREEN_FONT   LOSTSCREEN_PATH "Fluo Gums.ttf"
#define LOSTSCREEN_SIZE   35

#define SCORE_COLOR     al_map_rgb(255, 255, 255)
#define SCORE_X         380
#define SCORE_Y         300

/* frogger_lostscreen_tasks */
void frogger_lostscreen_tasks(GAME_STAGE *stage){
    return;
}

/* frogger_lostscreen */
bool frogger_lostscreen(uint32_t score){
    ALLEGRO_BITMAP* bitmap;
    ALLEGRO_FONT* font;
    STRING text;

    /* Abro la ventana */
    if( display == NULL ){
        display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
        if( display == NULL ){
            return false;
        }
        al_set_window_position(display, 300, 100);
    }

    /* Limpio el fondo */
    al_clear_to_color( CLEAR_COLOR );

    /* Cargo el bitmap */
    bitmap = al_load_bitmap( LOSTSCREEN_IMAGE );
    if( bitmap == NULL ){
        return false;
    }
    al_draw_bitmap(bitmap, 0, 0, 0);

    /* Puntuacion */
    font = al_load_ttf_font( LOSTSCREEN_FONT, LOSTSCREEN_SIZE, 0);
    if( font == NULL ){
        al_destroy_bitmap(bitmap);
        return false;
    }
    sprintf(text, "%d", score);
    al_draw_text(font, SCORE_COLOR, SCORE_X, SCORE_Y, 0, text);

    /* Mando el buffer al display */
    al_flip_display();

    al_destroy_bitmap(bitmap);
    al_destroy_font(font);

    return true;
}

/**********************/
/* GAMESCREEN handler */
/**********************/

/* Configuracion */
#define GAMESCREEN_PATH             "gui/frogger/allegro/gamescreen/"
#define GAMESCREEN_LIFES            GAMESCREEN_PATH "lifes_"
#define GAMESCREEN_IMAGE_FIELD      GAMESCREEN_PATH "field.png"
#define GAMESCREEN_IMAGE_LAVA       GAMESCREEN_PATH "lava.png"
#define GAMESCREEN_IMAGE_SKY        GAMESCREEN_PATH "sky.png"
#define GAMESCREEN_IMAGE_FULL_SKY   GAMESCREEN_PATH "full_sky.png"
#define GAMESCREEN_FONT             GAMESCREEN_PATH "Bubblegum.ttf"
#define GAMESCREEN_SIZE             25

#define GAME_SCORE_COLOR     al_map_rgb(255, 255, 255)
#define GAME_TIME_COLOR      al_map_rgb(255, 255, 255)
#define GAME_SCORE_MSG       "Score"
#define GAME_TIME_MSG        "Time"

#define GAME_DISPLAY_LIFES_X     10
#define GAME_DISPLAY_LIFES_Y     440
#define GAME_DISPLAY_SCORE_X     250
#define GAME_DISPLAY_SCORE_Y     445
#define GAME_DISPLAY_TIME_X      510
#define GAME_DISPLAY_TIME_Y      445


static char* frogger_back_selector(uint16_t index){
    switch(index){
        case 1:
            return GAMESCREEN_IMAGE_LAVA;
            break;
        case 2:
            return GAMESCREEN_IMAGE_SKY;
            break;
        case 3:
            return GAMESCREEN_IMAGE_FULL_SKY;
            break;
        default:
            return GAMESCREEN_IMAGE_FIELD;
            break;
    }
}

/* frogger_gamescreen */
bool frogger_gamescreen(GAME_STAGE* stage, FIELD field, FROG frog, uint32_t lifes, uint32_t time, uint32_t score){
    ALLEGRO_FONT* font;
    ALLEGRO_BITMAP* bitmap;
    LANE lane;
    FROGGER_OBJECT object;
    STRING text;
    uint32_t i, ii;

    /* Abro la ventana */
    if( display == NULL ){
        display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
        if( display == NULL ){
            return false;
        }
        al_set_window_position(display, DISPLAY_POS_X, DISPLAY_POS_Y);
    }

    /* Limpio la ventana */
    al_clear_to_color( CLEAR_COLOR );

    /* Pongo el campo de juego */
    bitmap = al_load_bitmap( frogger_back_selector(stage->backgroundId) );
    if( bitmap == NULL ){
        return false;
    }
    al_draw_bitmap(bitmap, 0, 0, 0);
    al_destroy_bitmap(bitmap);

    /* Pongo las vidas */
    sprintf(text, "%s%d.png", GAMESCREEN_LIFES, lifes);
    bitmap = al_load_bitmap( text );
    if( bitmap == NULL ){
        return false;
    }
    al_draw_bitmap(bitmap, GAME_DISPLAY_LIFES_X, GAME_DISPLAY_LIFES_Y, 0);
    al_destroy_bitmap(bitmap);

    /* Pongo la info de score */
    font = al_load_ttf_font(GAMESCREEN_FONT, GAMESCREEN_SIZE, 0);
    if( font == NULL ){
        return false;
    }
    sprintf(text, "%s %d", GAME_SCORE_MSG, score);
    al_draw_text(font, SCORE_COLOR, GAME_DISPLAY_SCORE_X, GAME_DISPLAY_SCORE_Y, 0, text);
    al_destroy_font(font);

    /* Pongo la info de time */
    font = al_load_ttf_font(GAMESCREEN_FONT, GAMESCREEN_SIZE, 0);
    if( font == NULL ){
        return false;
    }
    sprintf(text, "%s %d", GAME_TIME_MSG, time);
    al_draw_text(font, GAME_TIME_COLOR, GAME_DISPLAY_TIME_X, GAME_DISPLAY_TIME_Y, 0, text);
    al_destroy_font(font);

    /* Pongo los objetos */
    for(i = 0;i < field.lanesQty;i++){
        lane = field.lanes[i];

        for(ii = 0;ii < lane.objectsQty;ii++){
            object = lane.objects[ii];

            if( object->pos.x < DISPLAY_WIDTH ){
                bitmap = al_load_bitmap( gui_animation_get_frame(object) );
                if( bitmap == NULL ){
                  return false;
                }
                al_draw_bitmap(bitmap, object->pos.x, object->pos.y, 0);
                al_destroy_bitmap(bitmap);
            }

        }
    }

    /* Pongo la ranita :) */
    bitmap = al_load_bitmap( gui_animation_get_frame(frog.object) );
    if( bitmap == NULL ){
        return false;
    }
    al_draw_bitmap(bitmap, frog.object->pos.x, frog.object->pos.y, 0);
    al_destroy_bitmap(bitmap);

    /* Mando el buffer al display */
    al_flip_display();

    return true;
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

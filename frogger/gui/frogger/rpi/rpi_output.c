/* RPI_OUTPUT
 * Maneja la interfaz de visualizacion en raspberry pi
 */

#include "../frogger_output.h"
#include "../../gui_animation/gui_animation.h"
#include "rpi_graphics.h"

#include "../../gui_types.h"

#include "disdrv.h"

/* Configuraciones generales */

#define MAX_STRING      512

/*******************/
/* SCREEN handlers */
/*******************/

/* frogger_screen_close */
void frogger_screen_close(void);

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

    /* Limpio el display */
    rpi_display_clear();

    /* Imprimo el menu */
    if( !rpi_draw_bitmap(bitmap, map_position(0, 0)) ){
        return false;
    }

    /* Actualizo la pantalla */
    rpi_display_update();

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
    }
}

/**********************/
/* PAUSEMENU handlers */
/**********************/

/* frogger_pausemenu */
bool frogger_pausemenu(void);

/* frogger_pausemenu_move */
void frogger_pausemenu_move(INPUT_VALUES input);

/*************************/
/* CHANGESCREEN handlers */
/*************************/

/* frogger_changescreen */
bool frogger_changescreen(uint32_t level, uint32_t stage);

/***********************/
/* LOSTSCREEN handlers */
/***********************/

/* frogger_lostscreen */
bool frogger_lostscreen(uint32_t score);

/**********************/
/* GAMESCREEN handler */
/**********************/

/* frogger_gamescreen */
bool frogger_gamescreen(FIELD field, FROG frog, uint32_t lifes, uint32_t time, uint32_t score);

/* RPI_GRAPHICS
 */

#include "../../gui_init.h"
#include "rpi_graphics.h"
#include "../frogger_output.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Objetos */
static bool init = false;

/***************/
/* MOTION_TEXT */
/***************/

/* rpi_create_motion_text */
MOTION_TEXT* rpi_create_motion_text(void){
    MOTION_TEXT* motion;

    /* Reservo memoria para la instancia */
    motion = malloc( sizeof(MOTION_TEXT) );
    if( motion == NULL ){
        return NULL;
    }

    /* Parametros default */
    motion->init = false;

    /* Devuelvo instancia */
    return motion;
}

/* rpi_init_motion_text */
bool rpi_init_motion_text(MOTION_TEXT* motion, LENGTH length){
    LENGTH i;

    /* Me fijo que haya una instancia */
    if( motion == NULL ){
        return false;
    }

    /* Reservo memoria para el contenido del MOTION TEXT */
    motion->bitmaps = malloc( sizeof(BMP_POINTER) * length );
    if( motion->bitmaps == NULL ){
        return false;
    }
    /* Inicializo contenido */
    for(i = 0;i < length;i++){
        motion->bitmaps[i] = NULL;
    }

    /* Inicializado!! */
    motion->length = length;
    motion->width = 0;
    motion->init = true;

    /* Exito! */
    return true;
}

/* rpi_destroy_motion_text */
void rpi_destroy_motion_text(MOTION_TEXT* motion){
    LENGTH i;
    /* Verifico la instancia */
    if( motion == NULL ){
        return;
    }

    /* Verifico inicializacion */
    if( motion->init ){

        /* Libero memoria de sus elementos */
        for(i = 0;i < motion->length;i++){
            if( motion->bitmaps[i] != NULL ){
                destroy_bitmap(motion->bitmaps[i]);
            }
        }
    }

    /* Libero memoria de instancia */
    free(motion);
}

/* rpi_load_motion_text */
MOTION_TEXT* rpi_load_motion_text(char* text, POSITION pos){
    MOTION_TEXT* motion;
    LENGTH i;
    char str[MAX_STRING];

    /* Creo la instancia */
    motion = rpi_create_motion_text();
    if( motion == NULL ){
        return NULL;
    }

    /* Inicializo memoria para su contenido */
    if( !rpi_init_motion_text(motion, strlen(text)) ){
        free(motion);
        return NULL;
    }

    /* Cargo cada caracter */
    for(i = 0;i < strlen(text);i++){
        /* Armo el string filename */
        if( text[i] >= 'A' && text[i] <= 'Z' ){
            sprintf(str, "%s%c.bmp", FONTS_FOLDER, text[i]);
        }else if( text[i] >= 'a' && text[i] <= 'z' ){
            sprintf(str, "%s%c.BMP", FONTS_FOLDER, text[i] - 'a' + 'A');
        }else if( text[i] >= '0' && text[i] <= '9' ){
            sprintf(str, "%s%c.bmp", FONTS_FOLDER, text[i]);
        }else if( text[i] == ' ' ){
            sprintf(str, "%sspace.bmp", FONTS_FOLDER);
        }else{
            rpi_destroy_motion_text(motion);
            return NULL;
        }

        /* Cargo el bitmap */
        motion->bitmaps[i] = rpi_load_bitmap(str);
        if( motion->bitmaps[i] == NULL ){
            rpi_destroy_motion_text(motion);
            return NULL;
        }

        /* Calculo largo total */
        motion->width += motion->bitmaps[i]->header.width + 1;
    }

    /* Cargo posicion */
    motion->pos = pos;

    /* Exito */
    return motion;
}

/* rpi_move_motion_text */
void rpi_move_motion_text(MOTION_TEXT* motion){

    /* Muevo su posicion */
    motion->pos.x--;

    /* Verifico que no haya dado la vuelta */
    if( (motion->pos.x + motion->width) == 0 ){
        motion->pos.x = DISPLAY_WIDTH - 1;
    }
}

/* rpi_draw_motion_text */
bool rpi_draw_motion_text(MOTION_TEXT* motion){
    LENGTH i;
    int32_t x = 0;

    /* Verifico inicializacion */
    if( !motion->init ){
        return false;
    }

    /* Me muevo entre los bitmaps */
    for(i = 0;i < motion->length;i++){
        /* Dibujo el bitmap */
        if( !rpi_draw_bitmap(motion->bitmaps[i], map_position(motion->pos.x + x, motion->pos.y)) ){
            return false;
        }
        /* Calculo largo */
        x += motion->bitmaps[i]->header.width + 1;
    }

    return true;
}

/***********************/
/* MOTION_BMP handlers */
/***********************/

/* rpi_create_motion_bmp */
MOTION_BMP* rpi_create_motion_bmp(void){
    /* Creo la instancia */
    MOTION_BMP* motion;

    /* Reservo memoria para el motion */
    motion = malloc( sizeof(MOTION_BMP) );
    if( motion == NULL ){
        return NULL;
    }

    /* Parametros por defecto */
    motion->init = false;

    /* Devuelvo instancia */
    return motion;
}

/* rpi_init_motion_bmp */
bool rpi_init_motion_bmp(MOTION_BMP* motion, char* file, POSITION pos){

    /* Verifico que no este inicializado */
    if( motion->init ){
        return false;
    }

    /* Cargo el bitmap */
    motion->bitmap = rpi_load_bitmap(file);
    if( motion->bitmap == NULL ){
        return false;
    }

    /* Inicializado!! */
    motion->pos = pos;
    motion->init = true;

    /* Exito! */
    return true;
}

/* rpi_destroy_motion_bmp */
void rpi_destroy_motion_bmp(MOTION_BMP* motion){
    /* Verifico inicializado */
    if( motion == NULL ){
        return;
    }

    if( motion->init ){
        /* Libero el bitmap */
        destroy_bitmap(motion->bitmap);
    }

    free(motion);
}

/* rpi_move_motion_bmp */
void rpi_move_motion_bmp(MOTION_BMP* motion){

    /* Muevo su posicion */
    motion->pos.x--;

    /* Verifico que no haya dado la vuelta */
    if( (motion->pos.x + motion->bitmap->header.width) == 0 ){
        motion->pos.x = DISPLAY_WIDTH - 1;
    }
}

/* rpi_draw_motion_bmp */
bool rpi_draw_motion_bmp(MOTION_BMP* motion){

    /* Verifico inicializacion */
    if( !motion->init ){
        return false;
    }

    /* Dibujo el bitmap */
    if( rpi_draw_bitmap(motion->bitmap, motion->pos) ){
        return true;
    }
    return false;
}

/************************/
/* RPI_DISPLAY handlers */
/************************/

/* rpi_display_clear */
bool rpi_display_clear(void){
    BITMAP* bitmap;

    /* Me fijo que este inicializado */
    if( !init ){
        return false;
    }

    /* Limpio el display */
    bitmap = rpi_load_bitmap(CLEAR_BMP);
    if( bitmap == NULL ){
        return false;
    }
    if( !rpi_draw_bitmap(bitmap, map_position(0, 0)) ){
        return false;
    }

    /* Exito */
    destroy_bitmap(bitmap);
    return true;
}

/* rpi_display_update */
bool rpi_display_update(void){

    /* Si el display no esta inicializado */
    if( !init ){
        return false;
    }

    /* Llamo al updater */
    display_update();

    /* Se pudo hacer correctamente */
    return true;
}

/* gui_graphics_init */
bool gui_graphics_init(void){
    /* Inicializo el display */
    display_init();

    /* Configuro display */
    set_display_axis(NORMAL);

    /* Inicializa */
    init = true;

    return true;
}

/********************/
/* BITMAPS handlers */
/********************/

/* rpi_load_bitmap */
BITMAP* rpi_load_bitmap(char* filename){
    BITMAP* bitmap;

    /* Cargo el bitmap */
    bitmap = read_bitmap(filename);
    if( bitmap == NULL ){
        return NULL;
    }

    /* Devuelvo el bitmap */
    return bitmap;
}

/* rpi_draw_bitmap */
bool rpi_draw_bitmap(BITMAP* bitmap, POSITION pos){
    int32_t  x, y;
    int32_t xScreen, yScreen;

    /* Si no esta inicializado el display */
    if( !init ){
      return false;
    }

    /* Error de parametros */
    if( bitmap == NULL ){
        return false;
    }

    /* Itero los puntos del bitmap */
    for(y = 0;y < bitmap->header.height;y++){

        for(x = 0;x < bitmap->header.width;x++){

            /* Calculo posicion del bit */
            xScreen = pos.x + x;
            yScreen = pos.y + y;

            /* Compruebo que esta en el display la posicion del led */
            if( (xScreen >= 0 && xScreen < DISPLAY_WIDTH) && (yScreen >= 0 && yScreen < DISPLAY_HEIGHT) ){

                /* Verifico que este prendido */
                if( bitmap->pixels[(bitmap->header.height - y - 1) * bitmap->header.width + x] == BLACK_COLOR ){
                    /* Prendo el led */
                    display_write(yScreen, xScreen, D_ON);
                }else{
                    /* Apago el led */
                    display_write(yScreen, xScreen, D_OFF);
                }
            }
        }
    }

    /* Se pudo hacer correctamente */
    return true;
}

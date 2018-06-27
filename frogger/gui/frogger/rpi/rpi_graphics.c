/* RPI_GRAPHICS
 */

#include "../../gui_init.h"
#include "rpi_graphics.h"

#include <stdbool.h>

/* Objetos */
static bool init = false;

/***********************/
/* MOTION_BMP handlers */
/***********************/

/* rpi_create_motion_bmp */
MOTION_BMP rpi_create_motion_bmp(void){
    /* Creo la instancia */
    MOTION_BMP motion;

    /* Parametros por defecto */
    motion.init = false;

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
    if( motion->init ){

        /* Libero el bitmap */
        destroy_bitmap(motion->bitmap);
    }

    motion->init = false;
}

/* rpi_move_motion_bmp */
void rpi_move_motion_bmp(MOTION_BMP* motion){

    /* Muevo su posicion */
    motion->pos.x--;

    /* Verifico que no haya dado la vuelta */
    if( (motion->pos.x + motion->bitmap.width) == 0 ){
        motion->pos.x = DISPLAY_WIDTH - 1;
    }
}

/* rpi_draw_motion_bmp */
bool rpi_draw_motion_bmp(MOTION_BMP* motion){

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

    /* Me fijo que este inicializado */
    if( !init ){
        return false;
    }

    /* Limpio el display */
    display_clear();

    /* Exito */
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

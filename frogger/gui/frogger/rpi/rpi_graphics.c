/* RPI_GRAPHICS
 */

#include "rpi_graphics.h"

#include <stdbool.h>

/* Objetos */
static bool init = false;

/*************************/
/* RPI_GRAPHICS handlers */
/*************************/

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
void gui_graphics_init(void){
    /* Inicializo el display */
    display_init();

    /* Configuro display */
    set_display_axis(NORMAL);

    /* Inicializa */
    init = true;
}

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
            if( (xScreen > 0 && xScreen < DISPLAY_WIDTH) && (yScreen > 0 && yScreen < DISPLAY_HEIGHT) ){

                /* Verifico que este prendido */
                if( bitmap->pixels[y * bitmap->header.width + x] == BLACK_COLOR ){
                    /* Prendo el led */
                    display_write(xScreen, yScreen, D_ON);
                }else{
                    /* Apago el led */
                    display_write(xScreen, yScreen, D_OFF);
                }
            }
        }
    }

    /* Se pudo hacer correctamente */
    return true;
}

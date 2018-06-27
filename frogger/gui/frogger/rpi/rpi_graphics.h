/* RPI_GRAPHICS
 * Este modulo maneja el display matricial de leds
 * mediante bitmaps
 */

#ifndef RPI_GRAPHICS_H
#define RPI_GRAPHICS_H

/* Librerias utilizadas */
#include "bitmaps.h"
#include "disdrv.h"
#include "../../gui_types.h"

#include <stdbool.h>

/* Configuraciones del display */
#define DISPLAY_WIDTH   16
#define DISPLAY_HEIGHT  16

#define BLACK_COLOR     0x00
#define WHITE_COLOR     0xFF

#define FONTS_FOLDER    "gui/frogger/rpi/fonts/"
#define CLEAR_BMP       FONTS_FOLDER "clear.bmp"

/* Tipos de datos */

typedef BITMAP* BMP_POINTER;

typedef struct{
    /* Imagen bitmap a procesar */
    BMP_POINTER bitmap;

    /* Posicion */
    POSITION pos;

    /* Inicializacion */
    bool init;
} MOTION_BMP;

typedef struct{
    /* Arreglo de bitmaps */
    BMP_POINTER* bitmaps;
    LENGTH length;

    /* Ancho de todo el texto */
    uint32_t width;

    /* Posicion */
    POSITION pos;

    /* Inicializacion */
    bool init;
} MOTION_TEXT;

/***************/
/* MOTION_TEXT */
/***************/

/* rpi_create_motion_text
 * Crea una instancia de un MOTION TEXT
 */
MOTION_TEXT* rpi_create_motion_text(void);

/* rpi_init_motion_text
 * Inicializa la memoria necesaria para los BITMAPS del MOTION TEXT
 *
 * motion: Instancia a inicializar
 * length: Cantidad de bitmaps del texto
 */
bool rpi_init_motion_text(MOTION_TEXT* motion, LENGTH length);

/* rpi_destroy_motion_text
 * Destruye y libera memoria utilizada por el motion text
 *
 * motion: Instancia
 */
void rpi_destroy_motion_text(MOTION_TEXT* motion);

/* rpi_load_motion_text
 * Carga una instancia MOTION TEXT con el texto indicado
 * y tiene por posicion inicial, la indicada.
 *
 * text: Texto del MOTION TEXT
 * pos: Posicion inicial
 */
MOTION_TEXT* rpi_load_motion_text(char* text, POSITION pos);

/* rpi_move_motion_text
 * Mueve un objeto MOTION TEXT en el display
 *
 * motion: instancia
 */
void rpi_move_motion_text(MOTION_TEXT* motion);

/* rpi_draw_motion_text
 * Manda al display matricial de leds el MOTION TEXT
 *
 * motion: Instancia
 */
bool rpi_draw_motion_text(MOTION_TEXT* motion);

/***********************/
/* MOTION_BMP handlers */
/***********************/

/* rpi_create_motion_bmp
 * Crea una instancia default de un MOTION MOTION_BMP
 */
MOTION_BMP* rpi_create_motion_bmp(void);

/* rpi_init_motion_bmp
 * Inicializa memoria para la instancia
 *
 * motion: Instancia
 * file: Nombre de archivo .MOTION_BMP
 * pos: Posicion inicial del bitmap
 */
bool rpi_init_motion_bmp(MOTION_BMP* motion, char* file, POSITION pos);

/* rpi_destroy_motion_bmp
 * Destruye y libera memoria de la Instancia
 *
 * motion: Instancia
 */
void rpi_destroy_motion_bmp(MOTION_BMP* motion);

/* rpi_move_motion_bmp
 * Mueve el bitmap de posicion a la siguiente
 *
 * motion: Instancia
 */
void rpi_move_motion_bmp(MOTION_BMP* motion);

/* rpi_draw_motion_bmp
 * Dibuja el bitmap del objeto MOTION BMP
 *
 * motion: Instancia
 */
bool rpi_draw_motion_bmp(MOTION_BMP* motion);

/********************/
/* BITMAPS handlers */
/********************/

/* rpi_load_bitmap
 * Carga un bitmap en memoria
 *
 * filename: Nombre del archivo
 */
BITMAP* rpi_load_bitmap(char* filename);

/* rpi_draw_bitmap
 * Dibuja un bitmap en pantalla con la
 * ubicacion indicada
 *
 */
bool rpi_draw_bitmap(BITMAP* bitmap, POSITION pos);

/************************/
/* RPI_DISPLAY handlers */
/************************/

/* rpi_display_update
 * Manda el backbuffer al display matricial de leds
 */
bool rpi_display_update(void);

/* rpi_display_clear
 * Limpia el display matricial de leds
 */
bool rpi_display_clear(void);

#endif /* RPI_GRAPHICS_H */

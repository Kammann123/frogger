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

#define BLACK_COLOR     0

/*************************/
/* RPI_GRAPHICS handlers */
/*************************/

/* gui_graphics_init
 * Inicializa el display matricial de leds
 */
void gui_graphics_init(void);

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

/* rpi_display_update
 * Manda el backbuffer al display matricial de leds
 */
bool rpi_display_update(void);

/* rpi_display_clear
 * Limpia el display matricial de leds
 */
bool rpi_display_clear(void);

#endif /* RPI_GRAPHICS_H */

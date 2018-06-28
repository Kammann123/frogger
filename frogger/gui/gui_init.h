/* GUI_INIT
 * Maneja la inicializacion de las interfaces graficas.
 */

#ifndef GUI_INIT_H
#define GUI_INIT_H

/* Librerias */
#include <stdbool.h>

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* gui_graphics_init
 * Inicializa la interfaz IO
 */
bool gui_graphics_init(void);

/* gui_graphics_close
 * Cierra y libera memoria de interfaces
 */
void gui_graphics_close(void);

#endif /* GUI_INIT_H */

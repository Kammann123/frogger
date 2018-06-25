/* GUI_INIT
 * Maneja la inicializacion de las interfaces graficas.
 * Hace de wrapper segun la plataforma que se utilice 
 */

/* Librerias utilizadas */
#include "allegro/allegro_init.h"


#ifndef GUI_INIT_H
#define GUI_INIT_H

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* gui_init
 * Inicializa la interfaz IO
 */
bool gui_init(void);

#endif /* GUI_INIT_H */


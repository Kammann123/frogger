/* RPI_INIT
 * Inicializa la interfaz en raspberry
 */

#include "../../gui_init.h"
#include "disdrv.h"
#include "joydrv.h"

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* gui_init
 * Inicializa la interfaz IO
 */
bool gui_init(void){
    /* Inicializo el display */
    display_init();
    
    /* Configuro display */
    set_display_axis(NORMAL);
    
    return true;
}
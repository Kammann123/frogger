/* ALLEGRO_FROGGER_MAINMENU
 * Este modulo permite manejar en la interfaz de allegro
 * el menu principal del frogger
 */

#ifndef ALLEGRO_FROGGER_MAINMENU_H
#define ALLEGRO_FROGGER_MAINMENU_H

/* Librerias utilizadas */
#include <stdint.h>

/* Variables externas */
extern char* MAINMENU_OPTIONS[];

/**************/
/* Constantes */
/**************/

#define MAINMENU_WIDTH  640
#define MAINMENU_HEIGHT 480

#define MAINMENU_OPTION_0   "gui/allegro/frogger/mainmenu/images/mainmenu_0.png"
#define MAINMENU_OPTION_1   "gui/allegro/frogger/mainmenu/images/mainmenu_1.png"
#define MAINMENU_OPTION_2   "gui/allegro/frogger/mainmenu/images/mainmenu_2.png"
#define MAINMENU_OPTION_3   "gui/allegro/frogger/mainmenu/images/mainmenu_3.png"

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* allegro_frogger_mainmenu
 * Muestra en pantalla el menu principal 
 * del juego frogger
 * 
 * options: Puntero al arreglo de mensajes
 * optionSelected: Opcion seleccionada actualmente
 */
void allegro_frogger_mainmenu(char** options, uint16_t optionSelected);

#endif /* ALLEGRO_FROGGER_MAINMENU_H */


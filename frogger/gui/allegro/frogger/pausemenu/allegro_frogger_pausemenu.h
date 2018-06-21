/* ALLEGRO_FROGGER_PAUSEMENU
 * Este modulo permite manejar en la interfaz de allegro
 * el menu de pausa del frogger
 */

#ifndef ALLEGRO_FROGGER_PAUSEMENU_H
#define ALLEGRO_FROGGER_PAUSEMENU_H

/* Librerias utilizadas */
#include <stdint.h>

/* Variables externas */
extern const char* PAUSEMENU_OPTIONS[];

/**************/
/* Constantes */
/**************/

#define PAUSEMENU_WIDTH  640
#define PAUSEMENU_HEIGHT 400

#define PAUSEMENU_OPTION_0   "gui/allegro/frogger/pausemenu/images/pausemenu_0.png"
#define PAUSEMENU_OPTION_1   "gui/allegro/frogger/pausemenu/images/pausemenu_1.png"
#define PAUSEMENU_OPTION_2   "gui/allegro/frogger/pausemenu/images/pausemenu_2.png"

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* allegro_frogger_pausemenu
 * Muestra en pantalla el menu de pausa 
 * del juego frogger
 * 
 * options: Puntero al arreglo de mensajes
 * optionSelected: Opcion seleccionada actualmente
 */
void allegro_frogger_pausemenu(const char** options, uint16_t optionSelected);

/* allegro_frogger_pausemenu_close 
 * Cierra el menu principal de allegro
 */
void allegro_frogger_pausemenu_close(void);

#endif /* ALLEGRO_FROGGER_MAINMENU_H */




/* ALLEGRO_FROGGER_GAME
 * Este modulo maneja la interfaz del juego frogger
 * con allegro 
 */

#ifndef ALLEGRO_FROGGER_GAME_H
#define ALLEGRO_FROGGER_GAME_H

/* Librerias utilizadas */
#include <stdbool.h>

/**************/
/* Constantes */
/**************/

/* Parametros del display */
#define ALLEGRO_FROGGER_GAME_WIDTH  640
#define ALLEGRO_FROGGER_GAME_HEIGHT 480

#define ALLEGRO_DISPLAY_STEP    40

/* Imagenes para la pantalla */
#define FROGGER_IMAGE_FIELD     "gui/allegro/frogger/game/images/field.png"
#define FROGGER_IMAGE_LIFES_0   "gui/allegro/frogger/game/images/lifes_0.png"
#define FROGGER_IMAGE_LIFES_1   "gui/allegro/frogger/game/images/lifes_1.png"
#define FROGGER_IMAGE_LIFES_2   "gui/allegro/frogger/game/images/lifes_2.png"

/* Paths para acceder a animaciones */
#define FROGGER_PATH_OBJECTS        "gui/allegro/frogger/game/objects/"

#define FROGGER_PATH_FROG           "frog/"
#define FROGGER_PATH_FROG_UP_OBJFILE   FROGGER_PATH_OBJECTS FROGGER_PATH_FROG "frog_up"
#define FROGGER_PATH_FROG_DOWN_OBJFILE   FROGGER_PATH_OBJECTS FROGGER_PATH_FROG "frog_down"
#define FROGGER_PATH_FROG_LEFT_OBJFILE   FROGGER_PATH_OBJECTS FROGGER_PATH_FROG "frog_left"
#define FROGGER_PATH_FROG_RIGHT_OBJFILE   FROGGER_PATH_OBJECTS FROGGER_PATH_FROG "frog_right"

/* Colores de allegro */
#define CLEAR_COLOR     al_map_rgb(0, 0, 0)

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* allegro_frogger_pause
 * Pausa el juego
 */
void allegro_frogger_pause(void);

/* allegro_frogger_continue
 * Reanuda el juego
 */
void allegro_frogger_continue(void);

/* allegro_frogger_close *
 * Cierra y libera objetos creados en allegro para el juego
 */
void allegro_frogger_close(void);

/* allegro_frogger_init 
 * Inicializa los objetos del juego para
 * su funcionamiento en allegro
 */
bool allegro_frogger_init(void);

/* allegro_frogger_screen_update 
 * Actualiza la pantalla del juego
 */
void allegro_frogger_screen_update(void);

/* allegro_frogger_screen_close 
 * Cierra la pantalla del juego
 */
void allegro_frogger_screen_close(void);

#endif /* ALLEGRO_FROGGER_GAME_H */


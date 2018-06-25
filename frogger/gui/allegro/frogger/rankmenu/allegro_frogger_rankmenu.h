
#ifndef ALLEGRO_FROGGER_RANKMENU_H
#define ALLEGRO_FROGGER_RANKMENU_H

/* Librerias utilizadas */
#include <stdint.h>


/**************/
/* Constantes */
/**************/

#define RANKMENU_WIDTH  640
#define RANKMENU_HEIGHT 400

#define X_RANK          300
#define Y_RANK_BASE     30
#define Y_RANK_DIFF     25

#define RANKMENU_BACKGROUND   "gui/allegro/frogger/rankmenu/images/ranking-background.png"
#define FROG_GIF              "gui/allegro/frogger/rankmenu/images/frog-rank.png"

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* allegro_frogger_rankmenu
 * Muestra en pantalla el menu de ranking 
 * del juego frogger
 * 
 * options: Puntero al arreglo de mensajes
 */
void allegro_frogger_rankmenu(const char** options);

/* allegro_frogger_rankmenu_close 
 * Cierra el menu de ranking de allegro
 */
void allegro_frogger_rankmenu_close(void);


#endif /* ALLEGRO_FROGGER_RANKMENU_H */

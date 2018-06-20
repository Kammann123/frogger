/* ALLEGRO_INIT
 * Este modulo maneja la inicializacion de la libreria allegro
 */

/* Librerias utilizadas */
#include <allegro5/allegro5.h>

#include <stdbool.h>

#ifndef ALLEGRO_INIT_H
#define ALLEGRO_INIT_H

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* allegro_init
 * Wrapper para inicializar todos los modulos de
 * la libreria init que se usan 
 */
bool allegro_init(void);

#endif /* ALLEGRO_INIT_H */
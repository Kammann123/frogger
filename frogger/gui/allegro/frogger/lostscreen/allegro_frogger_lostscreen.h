/* ALLEGRO_FROGGER_LOSTSCREEN
 * Maneja la pantalla al perder
 */

#ifndef ALLEGRO_FROGGER_LOSTSCREEN_H
#define ALLEGRO_FROGGER_LOSTSCREEN_H

/* Librerias utilizadas*/

/**************/
/* Constantes */
/**************/

/* Ubicacion de archivos */
#define ALLEGRO_FROGGER_LOST_IMAGES  "gui/allegro/frogger/lostscreen/images/"
#define ALLEGRO_FROGGER_LOSTSCREEN   ALLEGRO_FROGGER_LOST_IMAGES "lost.png"

/* Tamaño de ventana */
#define ALLEGRO_LOSTSCREEN_WIDTH    640
#define ALLEGRO_LOSTSCREEN_HEIGHT   480

/* Colores */
#define CLEAR_COLOR     al_map_rgb(0, 0, 0)
/**************/
/* Prototipos */
/**************/

/* allegro_frogger_lostscreen 
 * Actualiza la pantalla del frogger cuando perdio
 */
void allegro_frogger_lostscreen(void);

/* allegro_frogger_lostscreen_close 
 * Cierra la ventana
 */
void allegro_frogger_lostscreen_close(void);

#endif /* ALLEGRO_FROGGER_LOSTSCREEN_H */


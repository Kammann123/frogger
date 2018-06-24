/* ALLEGRO_FROGGER_CHANGESCREEN 
 * Maneja una pantalla de cambio de nivel
 */


#ifndef ALLEGRO_FROGGER_CHANGESCREEN_H
#define ALLEGRO_FROGGER_CHANGESCREEN_H

/* Librerias utilizadas*/

/**************/
/* Constantes */
/**************/

/* Ubicacion de archivos */
#define ALLEGRO_FROGGER_CHANGE_IMAGES  "gui/allegro/frogger/changescreen/images/"
#define ALLEGRO_FROGGER_CHANGESCREEN   ALLEGRO_FROGGER_CHANGE_IMAGES "change.png"

#define ALLEGRO_FROGGER_CHANGE_FONTS   "gui/allegro/frogger/changescreen/fonts/"
#define ALLEGRO_FROGGER_CHANGE_FONT   ALLEGRO_FROGGER_CHANGE_FONTS "American Captain.otf"
#define ALLEGRO_FROGGER_CHANGE_SIZE   120

/* Tamaño de ventana */
#define ALLEGRO_CHANGESCREEN_WIDTH    640
#define ALLEGRO_CHANGESCREEN_HEIGHT   480

/* Colores */
#define CLEAR_COLOR     al_map_rgb(0, 0, 0)
#define LEVEL_COLOR     al_map_rgb(10, 38, 179)
#define STAGE_COLOR     al_map_rgb(138, 7, 154)

/* Ubicacion de texto */
#define LEVEL_X         130
#define LEVEL_Y         200
#define STAGE_X         340
#define STAGE_Y         360

/**************/
/* Prototipos */
/**************/

/* allegro_frogger_changescreen 
 * Actualiza la pantalla del frogger cuando perdio
 */
void allegro_frogger_changescreen(void);

/* allegro_frogger_changescreen_close 
 * Cierra la ventana
 */
void allegro_frogger_changescreen_close(void);

#endif /* ALLEGRO_FROGGER_CHANGESCREEN_H */


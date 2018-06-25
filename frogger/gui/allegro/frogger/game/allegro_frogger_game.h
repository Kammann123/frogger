/* ALLEGRO_FROGGER_GAME
 * Este modulo maneja la interfaz del juego frogger
 * con allegro 
 */

#ifndef ALLEGRO_FROGGER_GAME_H
#define ALLEGRO_FROGGER_GAME_H

/* Librerias utilizadas */
#include <stdbool.h>
#include <stdint.h>
#include "../../../gui_animation/gui_animation.h"
#include "../../../gui_types.h"

/**************/
/* Constantes */
/**************/

/* Parametros del display */
#define ALLEGRO_FROGGER_GAME_WIDTH  640
#define ALLEGRO_FROGGER_GAME_HEIGHT 480

/* Ubicacion de info */
#define ALLEGRO_DISPLAY_LIFES_X     10
#define ALLEGRO_DISPLAY_LIFES_Y     440

#define ALLEGRO_DISPLAY_SCORE_X     250
#define ALLEGRO_DISPLAY_SCORE_Y     445
#define ALLEGRO_DISPLAY_TIME_X      510
#define ALLEGRO_DISPLAY_TIME_Y      445

/* Limites del display*/
#define ALLEGRO_DISPLAY_BORDER_UP       0  
#define ALLEGRO_DISPLAY_BORDER_DOWN     400
#define ALLEGRO_DISPLAY_BORDER_LEFT     0
#define ALLEGRO_DISPLAY_BORDER_RIGHT    600

#define ALLEGRO_DISPLAY_STEP    40
#define ALLEGRO_STEP_RESOLUTION 10

/* Obstaculos del juego */
#define TRASH_0_X 0
#define TRASH_0_Y 400
#define TRASH_1_X 40
#define TRASH_1_Y 400
#define PHONE_X 600
#define PHONE_Y 400

/* Imagenes para la pantalla */
#define ALLEGRO_PATH_IMAGES     "gui/allegro/frogger/game/images/"

#define ALLEGRO_IMAGE_FIELD     ALLEGRO_PATH_IMAGES "field.png"
#define ALLEGRO_IMAGE_LIFES_1   ALLEGRO_PATH_IMAGES "lifes_1.png"
#define ALLEGRO_IMAGE_LIFES_2   ALLEGRO_PATH_IMAGES "lifes_2.png"
#define ALLEGRO_IMAGE_LIFES_3   ALLEGRO_PATH_IMAGES "lifes_3.png"

/* Paths para acceder a fuentes */
#define ALLEGRO_PATH_FONTS          "gui/allegro/frogger/game/fonts/"

#define ALLEGRO_FONT_0              ALLEGRO_PATH_FONTS "Bubblegum.ttf"
#define ALLEGRO_SIZE_0              25

/* Paths para acceder a animaciones */
#define ALLEGRO_PATH_OBJECTS                "gui/allegro/frogger/game/objects/"

#define ALLEGRO_FROG_OBJFILE                ALLEGRO_PATH_OBJECTS "frog/frog"
#define ALLEGRO_MOTORBIKE_OBJFILE           ALLEGRO_PATH_OBJECTS "motorbike/motorbike"
#define ALLEGRO_CAR_OBJFILE                 ALLEGRO_PATH_OBJECTS "car/car"
#define ALLEGRO_TRUCK_OBJFILE               ALLEGRO_PATH_OBJECTS "truck/truck"
#define ALLEGRO_BOAT_OBJFILE                ALLEGRO_PATH_OBJECTS "boat/boat"
#define ALLEGRO_YACHT_OBJFILE               ALLEGRO_PATH_OBJECTS "yacht/yacht"

/* Mensajes */
#define ALLEGRO_SCORE_MSG           "Score"
#define ALLEGRO_TIME_MSG            "Time"

/* Colores de allegro */
#define CLEAR_COLOR     al_map_rgb(0, 0, 0)
#define SCORE_COLOR     al_map_rgb(255, 255, 255)
#define TIME_COLOR      al_map_rgb(255, 255, 255)

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* allegro_frogger_create_object 
 * Crea un objeto animado perteneciente a un carril
 *
 * pos: Posicion inicial
 * speed: Velocidad
 * orientation: Orientacion inicial
 * type: Tipo de vehiculo
 */
ANIMATED_OBJECT* allegro_frogger_create_object(POSITION pos, SPEED speed, GUI_ANIMATION_ORIENTATION orientation, uint32_t type);

/* allegro_frogger_movement_valid 
 * Valida si el movimiento en dicha orientacion es valido
 *
 * input: Entrada del usuario
 */
bool allegro_frogger_movement_valid(uint16_t input);

/* allegro_frogger_frog_init 
 * Inicializa la rana del juego para
 * su funcionamiento en allegro
 */
bool allegro_frogger_frog_init(void);

/* allegro_frogger_screen_update 
 * Actualiza la pantalla del juego
 */
void allegro_frogger_screen_update(void);

/* allegro_frogger_screen_close 
 * Cierra la pantalla del juego
 */
void allegro_frogger_screen_close(void);

#endif /* ALLEGRO_FROGGER_GAME_H */


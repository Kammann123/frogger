#ifndef ALLEGRO_GUI_H
#define ALLEGRO_GUI_H

// Poner aqui todos los benditos includes de allegro

#include "game_kernel.h"
#include "game_types.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_color.h>

/**************/
/* Constantes */
/**************/

/* Parametros del display */
#define DISPLAY_WIDTH   640
#define DISPLAY_HEIGHT  480

#define DISPLAY_X_MAX   15
#define DISPLAY_Y_MAX   10

#define DISPLAY_X_STEP  DISPLAY_WIDTH / (DISPLAY_X_MAX + 1)
#define DISPLAY_Y_STEP  DISPLAY_HEIGHT / (DISPLAY_Y_MAX + 2)

/* Colores predefinidos */
#define CLEAR_COLOR     al_map_rgb(0, 0, 0)
#define WATER_COLOR     al_map_rgb(11, 15, 70)
#define LANE_COLOR      al_map_rgb(0, 0, 0)

/* Posiciones de elementos fijos */
#define ARRIVAL_X   0
#define ARRIVAL_Y   0
#define WATER_X     0
#define WATER_Y     1
#define STREET_X    0
#define STREET_Y    5
#define LANE_X      0
#define LANE_Y      9
#define START_X     0
#define START_Y     10
#define INFO_X      0
#define INFO_Y      11

/* Parametros de fuente */
#define FONT_FILENAME   "font.ttf"
#define FONT_SIZE       15
#define FONT_COLOR      al_map_rgb(255, 255, 255)
#define FONT_MARGIN     20

/* Archivos con imagenes del juego */
#define FIELD_IMAGE     "field.bmp"
#define FROG_IMAGE      "frog.png"
#define BIKE_IMAGE      "motorbike.png"
#define CAR_IMAGE       "car.png"
#define TRUCK_IMAGE     "truck.png"
#define BOAT_IMAGE      "boat.png"
#define YACHT_IMAGE     "yacht.png"

/* Mensajes de pantalla */
#define MAX_LENGTH      30

#define TIME_MESSAGE    "Tiempo: "

/**********************/
/* Funciones publicas */
/**********************/

/* main_menu_allegro 
 * Muestra el menu principal del juego
 * en la plataforma de rpi
 */
void main_menu_allegro(void);

/* print_frogger_allegro
 * Muestra en la plataforma rpi el juego
 * con todos sus elementos
 *
 * frog: Objeto de la rana
 * lanes: Lista de carriles
 */
void print_frogger_allegro(FROG_CLASS* frog, LANE* lanes);

/* pause_menu_allegro
 * Muestra un menu de pausa del juego
 * en la plataforma rpi
 */
uint16_t pause_menu_allegro(void);

/* input_keyboard
 * Lee el estado del joystick 
 */
uint16_t input_keyboard(void);

/* allegro_init
 * Inicializa lo necesario para utilizar la plataforma rpi
 */
bool allegro_init(void);

/* allegro_close
 * Cierra la plataforma
 */
void allegro_close(void);

#endif /* ALLEGRO_GUI_H */


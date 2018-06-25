/* FROGGER_GAME
 * Este modulo permite manejar la interfaz con el usuario
 * del juego frogger game
 */

#ifndef FROGGER_GAME_H
#define FROGGER_GAME_H

/* Librerias utilizadas */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include "../../../settings.h"
#include "../../gui_animation/gui_animation.h"

/**************/
/* Constantes */
/**************/

/* Animaciones */
#define FROG_UP     "FROG_UP"
#define FROG_DOWN   "FROG_DOWN"
#define FROG_LEFT   "FROG_LEFT"
#define FROG_RIGHT  "FROG_RIGHT"

#define MOTORBIKE_LEFT     "MOTORBIKE_LEFT"
#define MOTORBIKE_RIGHT    "MOTORBIKE_RIGHT"

#define CAR_LEFT     "CAR_LEFT"
#define CAR_RIGHT    "CAR_RIGHT"

#define TRUCK_LEFT     "TRUCK_LEFT"
#define TRUCK_RIGHT    "TRUCK_RIGHT"

#define BOAT_LEFT    "BOAT_LEFT"
#define BOAT_RIGHT   "BOAT_RIGHT"

#define YACHT_LEFT  "YACHT_LEFT"
#define YACHT_RIGHT "YACHT_RIGHT"

/* Parametros default */
#define DEFAULT_FROG_X  7
#define DEFAULT_FROG_Y  10
#define DEFAULT_FROG_ANIMATION  FROG_UP

/* Parametros de tablero */
#define DISPLAY_DIVISIONS_X 16
#define DISPLAY_DIVISIONS_Y 11

#define MAP_X_MIN   -4
#define MAP_X_MAX   19

/* Regiones del mapa */
#define REGION_WATER        map_region(map_position(0, 1), map_position(15, 4))
#define REGION_STREET       map_region(map_position(0, 6), map_position(15, 9))

/* Archivos */
#define FROGGER_PATH        "gui/frogger/frogger_game/"
#define FROGGER_PATH_LANES  FROGGER_PATH "lanes/"
#define FROGGER_FIELD_CFG   FROGGER_PATH "field"

/* Configuracion */
#define FIELD_ATTRIBUTES    "attributes"
#define FIELD_FILES         "files"
#define FIELD_QUANTITY      "quantity"

#define LANE_ATTRIBUTES         "attributes"
#define LANE_ATT_NUMBER         "lanenumber"
#define LANE_ATT_TYPE           "type"
#define LANE_ATT_ORIENTATION    "orientation"
#define LANE_ATT_QUANTITY       "quantity"
#define LANE_ATT_TIME           "timedelta"
#define LANE_ATT_SPACE          "spacedelta"   
#define LANE_ATT_ACCEL          "a_factor"
    
/* Tipos de objetos */
#define FROGGER_CFG_MOTORBIKE   "MOTORBIKE"
#define FROGGER_CFG_CAR         "CAR"
#define FROGGER_CFG_TRUCK       "TRUCK"
#define FROGGER_CFG_BOAT        "BOAT"
#define FROGGER_CFG_YACHT       "YACHT"

#define FROGGER_SIZE_MOTORBIKE   2
#define FROGGER_SIZE_CAR         2
#define FROGGER_SIZE_TRUCK       4
#define FROGGER_SIZE_BOAT        2
#define FROGGER_SIZE_YACHT       3

typedef enum {
    FROGGER_MOTORBIKE,
    FROGGER_CAR,
    FROGGER_TRUCK,
    FROGGER_BOAT,
    FROGGER_YACHT,
    FROGGER_NONE
} OBJECT_TYPES;

/******************/
/* Tipos de datos */
/******************/

typedef ANIMATED_OBJECT* FROGGER_OBJECT;

typedef struct{
    FROGGER_OBJECT object;
    FROGGER_OBJECT transport;
} FROG;

typedef FROGGER_OBJECT MOTORBIKE;
typedef FROGGER_OBJECT CAR;
typedef FROGGER_OBJECT TRUCK;
typedef FROGGER_OBJECT BOAT;
typedef FROGGER_OBJECT YACHT;

typedef struct{
    /* Archivo */
    char* file;
    
    /* Inicializacion */
    bool init;
} CFG;

typedef struct{
    /* Objetos del carril */
    FROGGER_OBJECT* objects;
    LENGTH objectsQty;
    
    /* Atributos del carril */
    uint32_t laneNumber;
    
    /* Movimiento del carril */
    uint32_t type;
    GUI_ANIMATION_ORIENTATION orientation;
    SPEED speed;
    
    /* Factor de aceleracion */
    uint32_t aFactor;
    
    /* Inicializacion */
    bool init;
} LANE;
        
typedef struct{
    /* Carriles del campo */
    LANE* lanes;
    CFG* files;
    
    /* Cantidad de carriles */
    LENGTH lanesQty;
    
    /* Inicializacion */
    bool init;
} FIELD;

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* frogger_game_new_level
 * Genera un nuevo campo al cambiar nivel
 *
 * level: Nivel
 */
bool frogger_game_new_level(uint32_t level);

/* frogger_game_has_won 
 * Devuelve true si la ranita gano
 */
bool frogger_game_has_won(void);

/* frogger_game_is_frog_static
 * Devuelve true si la rana esta estatica
 */
bool frogger_game_is_frog_static(void);

/* frogger_game_frog_init 
 * Inicializa el objeto frog
 */
bool frogger_game_frog_init(void);

/* frogger_game_is_water_region
 * Devuelve true si esta en la region de agua 
 */
bool frogger_game_is_water_region(void);

/* frogger_game_is_street_region
 * Devuelve true si esta en la region de vehiculos 
 */
bool frogger_game_is_street_region(void);

/* frogger_game_transport_frog 
 * Maneja el movimiento de la rana cuando esta
 * siendo transportada por un barco
 */
void frogger_game_transport_frog(void);

/* frogger_game_is_transport
 * Comprueba si la rana se bajo o subio de un barco
 * y lo configura
 */
void frogger_game_is_transport(void);
        
/* frogger_game_drown
 * Devuelve true si la rana se cayo al agua
 */
bool frogger_game_drown(void);

/* frogger_game_reset_frog_position
 * Reinicia posicion de frog
 */
void frogger_game_reset_frog_position(void);

/* frogger_game_collisions
 * Verifica si hay colisiones en el juego
 */
bool frogger_game_collisions(void);

/* frogger_game_move_lanes
 * Mueve los carriles del juego
 */
void frogger_game_move_lanes(void);

/* frogger_game_move_frog
 * Mueve el objeto frog en la direccion segun input
 *
 * input: Direccion ingresada por usuario 
 */
bool frogger_game_move_frog(uint16_t input);

/* frogger_game_pause
 * Pausa el juego
 */
void frogger_game_pause(void);

/* frogger_game_continue
 * Reanuda el juego
 */
void frogger_game_continue(void);

/* frogger_game_close 
 * Cierra y libera los objetos del juego creados
 */
void frogger_game_close(void);

/* frogger_game_init 
 * Inicializa los objetos del juego
 */
bool frogger_game_init(void);

/* frogger_game_screen_update
 * Actualiza la pantalla del frogger
 */
void frogger_game_screen_update(void);

/* frogger_game_screen_close 
 * Cierra la pantalla del juego del
 * frogger 
 */
void frogger_game_screen_close(void);

#endif /* FROGGER_GAME_H */


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

/* Parametros default */
#define DEFAULT_FROG_X  7
#define DEFAULT_FROG_Y  10
#define DEFAULT_FROG_ORIENTATION GUI_ANIMATION_VERTICAL_UP

/* Parametros de tablero */
#define DISPLAY_DIVISIONS_X 16
#define DISPLAY_DIVISIONS_Y 11

#define MAP_X_MIN   -4
#define MAP_X_MAX   19

/* Archivos */
#define FROGGER_PATH        "gui/frogger/frogger_game/"
#define FROGGER_PATH_LANES  FROGGER_PATH "lanes/"
#define FROGGER_FIELD_CFG   FROGGER_PATH "field"

/* Configuracion */
#define FIELD_ATTRIBUTES    "attributes"
#define FIELD_FILES         "files"
#define FIELD_QUANTITY      "quantity"

#define LANE_ATTRIBUTES     "attributes"
#define LANE_NUMBER         "lanenumber"
#define LANE_TYPE           "type"
#define LANE_ORIENTATION    "orientation"
#define LANE_QUANTITY       "quantity"
#define LANE_TIMEDELTA      "timedelta"
#define LANE_SPACEDELTA     "spacedelta"    

/* Tipos de objetos */
#define FROGGER_CFG_MOTORBIKE   "MOTORBIKE"
#define FROGGER_CFG_CAR         "CAR"
#define FROGGER_CFG_TRUCK       "TRUCK"
#define FROGGER_CFG_BOAT        "BOAT"
#define FROGGER_CFG_YACHT       "YACHT"

/* Tamaño de objetos en divisiones */
#define FROGGER_SIZE_MOTORBIKE  2
#define FROGGER_SIZE_CAR        3
#define FROGGER_SIZE_TRUCK      4
#define FROGGER_SIZE_BOAT       2
#define FROGGER_SIZE_YACHT      3

typedef enum {
    FROGGER_MOTORBIKE,
    FROGGER_CAR,
    FROGGER_TRUCK,
    FROGGER_BOAT,
    FROGGER_YACHT
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
    /* Objetos del carril */
    FROGGER_OBJECT* objects;
    uint32_t objectsQty;
    
    /* Atributos del carril */
    uint32_t laneNumber;
    
    /* Movimiento del carril */
    uint32_t type;
    uint32_t orientation;
    SPEED speed;
    
    /* Flags */
    bool init;
} LANE;

typedef char* LANE_CFG;
        
typedef struct{
    /* Carriles del campo */
    LANE* lanes;
    
    /* Cantidad de carriles */
    uint32_t lanesQty;
    
    /* Archivos de configuracion de lanes */
    LANE_CFG* lanesCfg;
} FIELD;

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* frogger_game_move_lanes
 * Mueve los carriles del juego
 */
void frogger_game_move_lanes(void);

/* frogger_game_move_frog
 * Mueve el objeto frog en la direccion segun input
 *
 * input: Direccion ingresada por usuario 
 */
void frogger_game_move_frog(uint16_t input);

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


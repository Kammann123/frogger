#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <stdint.h>
#include <stdbool.h>

/**************/
/* Constantes */
/**************/

/* Valores menu de pausa */
#define RESUME      0
#define RESTART     1
#define EXIT        2

/* Direcciones de movimiento */
#define MOVE_TO_RIGHT   0
#define MOVE_TO_LEFT    1

/* Posiciones */
#define FROG_X_DEFAULT  8
#define FROG_Y_DEFAULT  10
#define FROG_X_MIN      0
#define FROG_X_MAX      15
#define FROG_Y_MIN      0
#define FROG_Y_MAX      10
#define MAP_X_MIN       -3
#define MAP_Y_MAX       18

/* Velocidades, tiempos en mS por casillero */
#define M_BIKE_SPEED_BASE   300
#define CAR_SPEED_BASE      300
#define TRUCK_SPEED_BASE    300
#define BOAT_SPEED_BASE     300
#define YACHT_SPEED_BASE    300

/* Tipos de objetos */
#define NONE    0
#define FROG    1
#define M_BIKE  2
#define CAR     3
#define TRUCK   4
#define BOAT    5
#define YACHT   6

/* Carriles y parametros */
#define NUMBER_OF_LANES 8

/*Condiciones inciales*/
#define INICIAL_LEVEL 0
#define INICIAL_SCORE 0
#define INICIAL_TIME 0
#define INICIAL_LIFES 3

/************************/
/* Estructuras de datos */
/************************/

typedef struct{
    int16_t x;
    int16_t y;
} POSITION;

typedef struct{
    POSITION pos;
    uint16_t level;
    uint32_t score;
    uint32_t time;
    uint16_t lifes;
} FROG_CLASS;

typedef struct{
    uint16_t id;
    uint16_t type;
    uint16_t direction;
    uint16_t vehiclesQty;
    uint16_t speed;
    POSITION* vehicles;
} LANE;

typedef LANE LANE_LIST[NUMBER_OF_LANES];

#endif /* GAME_TYPES_H */


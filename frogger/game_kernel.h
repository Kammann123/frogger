#ifndef GAME_KERNEL_H
#define GAME_KERNEL_H

#include <stdint.h>
#include <stdbool.h>
//2
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

/* Cantidad de objetos */
#define FROG        1
#define ROW_QTY_1   3
#define ROW_QTY_2   4
#define ROW_QTY_3   3
#define ROW_QTY_4   4
#define ROW_QTY_6   3
#define ROW_QTY_7   3
#define ROW_QTY_8   4
#define ROW_QTY_9   4
#define TOTAL_QTY   FROG+ROW_QTY_1+ROW_QTY_2+ROW_QTY_3+ROW_QTY_4+ROW_QTY_6+ROW_QTY_7+ROW_QTY_8+ROW_QTY_9

/************************/
/* Estructuras de datos */
/************************/

typedef struct{
    int16_t x;
    int16_t y;
} POSITION;

typedef struct{
    POSITION pos;
    uint16_t type;
    uint16_t speed;
} OBJECT;

typedef OBJECT OBJECT_LIST[TOTAL_QTY];

typedef struct{
    uint16_t vehiclesQty;
    uint16_t type;
    uint16_t rowNumber;
} ROW;

typedef struct{
    uint16_t level;
    uint32_t score;
    uint32_t time;
    uint16_t lifes;
} GAME_DATA;

typedef struct{
    OBJECT* objects;
    uint16_t listSize;
    uint16_t type;
    uint16_t direction;
} MOVE_DATA;

/**********************/
/* Funciones publicas */
/**********************/

/* kernel_init 
 * Inicializa el kernel, con sus objetos
 * y sus estados iniciales 
 */
bool kernel_init(void);

/* kernel_close
 * Cierra o libera cosas que utiliza el kernel
 */
void kernel_close(void);

/* object_init
 * Inicializa el arreglo con objetos, para 
 * tener primero el frog y sus datos
 *
 * list: Puntero a la lista de objetos
 */
void object_init(OBJECT* list);

/* generate_level
 * Genera los objetos del nuevo nivel
 * 
 * rowSettings: Arreglo con la configuracion de cada carril
 * rowSize: Cantidad de carriles en el rowSetting
 * list: Arreglo con los objetos
 * level: Nivel, para configurar las velocidades 
 */
void generate_level(ROW* rowSettings, uint16_t rowSize, OBJECT* list, uint16_t level);

/* move_vehicles
 * Mueve los vehiculos, se configura con un parametro de tipo
 * MOVE_DATA con sus respectivos campos.
 * 
 * param: Puntero a un MOVE_DATA con quienes y como moverlos
 */
void move_vehicles(void* param);

/* move_frog
 * Mueve la rana dentro de una lista de objetos, segun el evento
 * 
 * event: Tipo de movimiento segun evento
 * list: Arreglo con los objetos
 */
void move_frog(uint16_t event, OBJECT* list);

/* collision
 * Devuelve true si detecta que hubo una colision entre la rana
 * y alguno de los vehiculos
 * 
 * list: Arreglo con los objetos
 * listSize: Cantidad de objetos, contando la rana 
 */
bool collision(OBJECT* list, uint16_t listSize);

/* drown
 * Devuelve true si detecta que se cayo la rana al agua
 * 
 * list: Arreglo con los objetos
 * listSize: Cantidad de objetos de la lista 
 */
bool drown(OBJECT* list, uint16_t listSize);

/* has_won
 * Devuelve true si detecta que gano, por llegar
 * a la linea final o carril final
 * 
 * list: Arreglo con los objetos 
 */
bool has_won(OBJECT* list);

/* has_lost
 * Devuelve true si detecta que perdio todas las vidas
 * 
 * data: Puntero con los datos del juego
 */
bool has_lost(GAME_DATA* data);

/* calculate_speed
 * Calcula la velocidad o tiempo de movimiento entre casilleros
 * segun el tipo de vehiculo y el nivel
 * 
 * type: Tipo de vehiculo
 * level: Nivel del juego
 */
uint32_t calculate_speed(uint16_t type, uint16_t level);

/* calculate_score
 * Calcula el score final de la partida segun el nivel y el tiempo tardado
 * 
 * level: Nivel del juego
 * time: Tiempo que tardo en terminar
 */
uint32_t calculate_score(uint16_t level, uint32_t time);

/* level_up
 * Cambia los datos del juego para subir un nivel
 * 
 * data: Contiene la data principal del juego
 */
void level_up(GAME_DATA* data);

/* start_level
 * Empieza el nivel actual, generando todo con la GAME_DATA
 *
 * list: Arreglo con los objetos
 * listSize: Cantidad de objetos
 */
void start_level(OBJECT* list, uint16_t listSize);

/* reset_level 
 * Reinicia el nivel actual sin regenerar todo
 */
void reset_level(void);

/* restart
 * Reinicia la partida, con vidas y todo desde el principio
 */
void restart(void);

#endif /* GAME_KERNEL_H */


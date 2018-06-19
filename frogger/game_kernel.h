#ifndef GAME_KERNEL_H
#define GAME_KERNEL_H

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

/* lane_init
 * Inicializa un carril con sus parametros y le reserva memoria
 *
 * lane: Puntero al carril
 * id: Numero de posicion del carril en Y
 * type: Tipo de vehiculos que maneja
 * direction: Direccion en que se mueven vehiculos
 * qty: Cantidad de vehiculos
 * speed: Velocidad de desplazamiento
 */
bool lane_init(LANE* lane, uint16_t id, uint16_t type, uint16_t direction, uint16_t qty, uint16_t speed);

/* lane_close
 * Cierra y libera memoria del carril
 */
void lane_close(LANE* lane);

/* generate_level
 * Genera los objetos del nuevo nivel
 * 
 * lanes: Lista con los carriles y sus configuraciones
 * level: Nivel, para configurar las velocidades 
 */
void generate_level(LANE_LIST* lanes, uint16_t level);

/* move_vehicles
 * Mueve los vehiculos de un carril
 * 
 * param: Se le va a pasar LANE* para manejar cada carril
 */
void move_vehicles(void* param);

/* move_frog
 * Mueve la rana segun el evento del usuario
 * 
 * event: Tipo de movimiento segun evento
 * frog: Objeto de la rana
 */
void move_frog(uint16_t event, FROG_CLASS* frog);

/* collision
 * Devuelve true si detecta que hubo una colision entre la rana
 * y alguno de los vehiculos
 * 
 * frog: El objeto rana
 * lanes: La lista con los carriles
 */
bool collision(FROG_CLASS* frog, LANE_LIST* lanes);

/* drown
 * Devuelve true si detecta que se cayo la rana al agua
 * 
 * frog: El objeto rana
 * lanes: La lista con los carriles
 */
bool drown(FROG_CLASS* frog, LANE_LIST* lanes);

/* has_won
 * Devuelve true si detecta que gano, por llegar
 * a la linea final o carril final
 * 
 * frog: El objeto rana
 */
bool has_won(FROG_CLASS* frog);

/* has_lost
 * Devuelve true si detecta que perdio todas las vidas
 * 
 * frog: El objeto rana
 */
bool has_lost(FROG_CLASS* frog);

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
 * frog: Objeto de la rana
 */
void level_up(FROG_CLASS* frog);

/* start_level
 * Empieza el nivel actual, generando los nuevos carriles
 *
 * lanes: Lista de carriles
 */
void start_level(LANE_LIST* lanes);

/* reset_level 
 * Reinicia el nivel actual sin regenerar todo
 */
void reset_level(void);

/* restart
 * Reinicia la partida, con vidas y todo desde el principio
 */
void restart(void);

#endif /* GAME_KERNEL_H */


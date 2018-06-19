#ifndef GAME_KERNEL_H
#define GAME_KERNEL_H

#include <stdint.h>
#include <stdbool.h>
#include "input.h"
#include "game_types.h"

/**********************/
/* Funciones publicas */
/**********************/

/* kernel_init asd
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
 * frog: Objeto ranita
 * lanes: Lista de carriles
 */
void start_level(FROG_CLASS* frog, LANE_LIST* lanes);

/* reset_level 
 * Reinicia el nivel actual sin regenerar todo
 * 
 * frog: Objeto ranita
 */
void reset_level(FROG_CLASS* frog);

/* restart
 * Reinicia la partida, con vidas y todo desde el principio
 * 
 * frog: Objeto ranita
 * lanes: Lista de carriles
 */
void restart(FROG_CLASS* frog, LANE_LIST* lanes);

#endif /* GAME_KERNEL_H */


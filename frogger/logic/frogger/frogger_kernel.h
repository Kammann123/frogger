/* FROGGER_KERNEL
 * Este modulo maneja la logica del juego frogger
 * asi como los datos que lo nuclean
 */

#ifndef FROGGER_KERNEL_H
#define FROGGER_KERNEL_H

/* Librerias utilizadas */
#include <stdint.h>
#include <stdbool.h>

/**************/
/* Constantes */
/**************/

/* Valores default de la partida */
#define DEFAULT_LEVEL   1
#define DEFAULT_LIFES   3
#define DEFAULT_SCORE   0
#define DEFAULT_TIME    0
#define DEFAULT_STAGE   0

#define STAGE_MAX_VALUE 3
#define TIME_MAX_VALUE  60

/* Parametros de puntaje */
#define LEVEL_SCORE_BASE      20
#define TIME_SCORE_BASE       1

/* Eventos del flujo del juego */
typedef enum{
    FROGGER_HAS_DIED,
    FROGGER_HAS_DROWN,
    FROGGER_HAS_LOST,
    FROGGER_HAS_WON,
    FROGGER_ERROR,
    FROGGER_NOTHING
} FLOW_EVENTS;

/******************/
/* Tipos de datos */
/******************/

typedef struct{
    uint16_t stage;
    uint16_t level;
    uint16_t lifes;
    uint32_t score;
    uint32_t time;
} FROGGER_GAME_DATA;

/***********************/
/* Prototipos publicos */
/***********************/


/* frogger_get_lifes
 * Devuelve el score del jugador
 */
uint32_t frogger_get_lifes(void);

/* frogger_get_level
 * Devuelve el score del jugador
 */
uint32_t frogger_get_level(void);

/* frogger_get_stage
 * Devuelve el score del jugador
 */
uint32_t frogger_get_stage(void);

/* frogger_get_score
 * Devuelve el score del jugador
 */
uint32_t frogger_get_score(void);

/* frogger_get_time
 * Devuelve el time del jugador
 */
uint32_t frogger_get_time(void);

/* frogger_restart 
 * Reinicia los datos del juego
 */
void frogger_restart(void);

/* frogger_calculate_score 
 * Calcula el score agregado por el nivel, stage y time
 *
 * level: Nivel del juego
 * stage: Stage del nivel
 * time: Tiempo en que gano el stage
 */
uint32_t frogger_calculate_score(uint32_t level, uint32_t stage, uint32_t time);

/* frogger_level_up
 * Maneja subir un nivel
 */
bool frogger_level_up(void);

/* frogger_time_count
 * Cuenta tiempo de juego
 */
void frogger_time_count(void);

/* frogger_flow 
 * Maneja el flujo del programa del juego
 */
uint32_t frogger_flow(void);

/* frogger_reset
 * Resetea posicion en stage
 */
void frogger_reset(void);

#endif /* FROGGER_KERNEL_H */
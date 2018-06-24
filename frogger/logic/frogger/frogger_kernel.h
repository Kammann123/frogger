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

#define STAGE_MAX_VALUE 1
#define TIME_MAX_VALUE  60

/* Parametros de puntaje */
#define LEVEL_SCORE_BASE      20
#define TIME_SCORE_BASE       1

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
 * 
 */
bool frogger_flow(void);

/* frogger_reset
 * Resetea posicion en stage
 */
void frogger_reset(void);


#endif /* FROGGER_KERNEL_H */
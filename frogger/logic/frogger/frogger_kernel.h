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

#define STAGE_MAX_VALUE 5

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

/* frogger_time_count
 * Cuenta tiempo de juego
 */
void frogger_time_count(void);

/* frogger_flow 
 * Maneja el flujo del programa del juego
 * 
 */
void frogger_flow(void);

/* frogger_reset
 * Resetea posicion en stage
 */
void frogger_reset(void);


#endif /* FROGGER_KERNEL_H */
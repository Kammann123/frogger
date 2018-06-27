/* GUI_TYPES
 * Contiene los tipos de datos genericos utilizados
 * por igual en todas las interfaces con el usuario
 */

#ifndef GUI_TYPES_H
#define GUI_TYPES_H

/* Librerias utilizadas */
#include <stdint.h>
#include <stdbool.h>

#define TESTING

/******************/
/* Tipos de datos */
/******************/

typedef enum {
    MAINMENU_STAGE,
    RANKING_STAGE,
    HOWTO_STAGE,
    FROGGER_STAGE,
    PAUSEMENU_STAGE,
    LOSTSCREEN_STAGE,
    CHANGESCREEN_STAGE,
    DEAD_STAGE,
    CLOSING_STAGE
} STAGE_VALUES;

typedef struct{
    /* Valor del estado del juego */
    STAGE_VALUES value;
    
    /* Flags */
    bool hasChanged;    
} GAME_STAGE;

#define DEFAULT_GAME_STAGE MAINMENU_STAGE

typedef struct{
    int32_t x;
    int32_t y;
} POSITION;

typedef uint32_t LENGTH;

/**************/
/* Prototipos */
/**************/

/* map_position
 * Crea un objeto posicion
 *
 * x: Posicion x
 * y: Posicion y
 */
POSITION map_position(int32_t x, int32_t y);

/* testing_msg
 * Muestra un mensaje en la consola para informar durante
 * el proceso de pruebas
 */
void testing_msg(char *str);

/***********************/
/* GAME_STAGE handlers */
/***********************/

/* stage_init 
 * Crea y devuelve una instancia de GAME_STAGE
 * default, con parametros por defecto
 */
GAME_STAGE stage_init(void);

/* change_stage
 * Permite modificar el estado del programa
 * y realizar tareas secundarias de limpieza
 *
 * stage: Estado del programa
 * newStage: Nuevo estado del programa
 */
void change_stage(GAME_STAGE* stage, STAGE_VALUES value);

/* is_stage
 * Compara y verifica si el estado del juego es ese.
 * 
 * stage: Instancia 
 * value: Estado de juego
 */
bool is_stage(GAME_STAGE* stage, STAGE_VALUES value);

#endif /* GUI_TYPES_H */

/* GUI_TYPES
 * Contiene los tipos de datos genericos utilizados
 * por igual en todas las interfaces con el usuario
 */

#ifndef GUI_TYPES_H
#define GUI_TYPES_H

/* Librerias utilizadas */
#include <stdint.h>
#include <stdbool.h>

#include "../score_board/score_board.h"
    
/* Habilita impresiones de modo prueba */

#define MAX_STRING 512

#define DEFAULT_USERNAME    "player"

/******************/
/* Tipos de datos */
/******************/

typedef uint32_t LENGTH;

typedef char STRING[MAX_STRING];

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

    /* Nombre del jugador */
    STRING username;
    scores_t topScore[MAX_SCORES + 1];
    LENGTH topLength;

    /* Flags */
    bool hasChanged;
} GAME_STAGE;

#define DEFAULT_GAME_STAGE MAINMENU_STAGE

typedef struct{
    int32_t x;
    int32_t y;
} POSITION;

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

/* save_score
 * Guarda un nuevo puntaje del usuario en el top score
 *
 * stage: Instancia
 * score: Puntaje
 */
void save_score(GAME_STAGE* stage, uint32_t score);

/* load_score
 * Carga el score
 *
 * stage: Instancia
 */
void load_score(GAME_STAGE* stage);

/* get_score_position
 * Devuelve un string formateado con el puntaje de la posicion
 *
 * stage: Instancia
 * str: String donde se guarda el resultado
 * pos: Posicion de la tabla
 */
bool get_score_position(GAME_STAGE* stage, char* str, uint32_t pos);

/* stage_init
 * Crea y devuelve una instancia de GAME_STAGE
 * default, con parametros por defecto
 *
 * username: Nombre del jugador
 */
GAME_STAGE stage_init(char* username);

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

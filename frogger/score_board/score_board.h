#ifndef SCORE_BOARD_H
#define SCORE_BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


/* Parametros */
#define SCORE_FILE          "score_board/scoreboard.txt"
#define MAX_SCORES          10       //Maxima cantidad de puntajes mostrados.
#define MAX_LARGE_NAME      20

/************************/
/* Estructuras de datos */
/************************/

typedef struct {
    char name [MAX_LARGE_NAME];
    uint32_t score;
} scores_t;

/*
* Como podria usarse en Allegro:
*    scores_t * scoreArray =  (scores_t * ) calloc (ALLEGRO_MAX_SCORES+1, sizeof(scores_t));
*    get_score_array(scoreArray, SCORE_FILE);
*
*    add_score ("pablo", 400, scoreArray);
*    add_score ("lucas", 450, scoreArray);
*    add_score ("gonza", 4000, scoreArray);
*    add_score ("fede", 40000, scoreArray);
*
* Como podria usarse en RPI:
*    scores_t * rpiScoreArray =  (scores_t * ) calloc (RPI_MAX_SCORES+1, sizeof(scores_t));
*    get_score_array(rpiScoreArray, RPI_SCORE_FILE);
*
*    add_score (RPI_USER_NAME, 400, rpiScoreArray);
*/

/*********************************/
/* Prototipos funciones publicas */
/*********************************/

/* get_score_array
 * Inicia un array de scores_t con lo que encuentre en el archivo file.
 * scoresToInit: puntero a scores_t
 * file: direccion del archivo
 */
uint8_t get_score_array (scores_t * scoresToInit, char * file);

/* add_score
 * Agrega un puntaje al array de scores y reescribe el archivo donde se guardan.
 * name: string del nombre del jugador.
 * score: puntaje obtenido.
 * scoreBoard: puntero al array de scores.
 */
uint8_t add_score (char * name, uint32_t score, scores_t * scoreBoard);

/* get_last_pos
 * Devuelve la primera posicion que no tenga datos del array de scores.
 */
uint8_t get_last_pos (scores_t * scoreBoard);

/* print_scores
 * Imprime en consola todos los puntajes del arrat de scores.
 */
uint8_t print_scores (scores_t * scoreBoard);



#endif /* SCORE_BOARD_H */

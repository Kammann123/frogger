#ifndef SCORE_BOARD_H
#define SCORE_BOARD_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


/* Parametros */
#define SCORE_FILE      "scoreboard.txt"
#define RPI_SCORE_FILE  "rpi-scoreboard.txt"
#define ALLEGRO_MAX_SCORES 10       //Maxima cantidad de puntajes mostrados.
#define MAX_LARGE_NAME     20   
#define RPI_MAX_SCORES     1        //Maxima cantidad de puntajes mostrados
#define RPI_USER_NAME      "rpi"


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
/* Prototipos funciones privadas */
/*********************************/

/* cmp_structs
 * Compara los elementos de dos scores_t.
 * Se utiliza como parametro para qsort.
 */
static int cmp_structs (const void * a, const void * b);

/* sort_score_array
 * Reordena el array de scores en base a los nuevos puntajes ingresados.
 */
static uint8_t sort_score_array (scores_t * scoreBoard);

/* trim_array
 * Como el array tiene 1 posicion mas de la que se va a mostrar, trim_array deja en 0 dicha posicion.
 */
static uint8_t trim_array (scores_t * scoreBoard);

/* write_score_board_file
 * Reescribe el archivo donde se guardan los puntajes
 */
static uint8_t write_score_board_file (scores_t * scoreBoard, char * file);

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


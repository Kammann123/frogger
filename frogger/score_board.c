#include "score_board.h"


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


/***************************/
/* Definicion de funciones */
/***************************/

/*get_score_array*/
uint8_t get_score_array (scores_t * scoresToInit, char * file){

    int i;
    FILE * scoreBoardFile;
    scoreBoardFile = fopen (file, "r");


    if (scoreBoardFile == NULL){
        return -1;
    } else{
        for (i=0; i<=MAX_SCORES; i++){
            fread(&scoresToInit[i], sizeof(scores_t), 1, scoreBoardFile);
        }
        fclose (scoreBoardFile);
        return 0;
    }
}

/*get_last_pos*/
uint8_t get_last_pos (scores_t * scoreBoard){

    uint8_t lastPos=0;
    while((strcmp(scoreBoard[lastPos].name, "")) && (scoreBoard[lastPos].name!=NULL))
        lastPos++;
    return lastPos;

}

/*add_score*/
uint8_t add_score (char * name, uint32_t score, scores_t * scoreBoard){
    uint8_t lastPos = get_last_pos (scoreBoard);
    strcpy(scoreBoard[lastPos].name, name);
    scoreBoard [lastPos].score = score;
    sort_score_array (scoreBoard);
    trim_array (scoreBoard);
    write_score_board_file (scoreBoard, SCORE_FILE);

    return 0;
}


/*sort_score_array*/
static uint8_t sort_score_array (scores_t * scoreBoard){
    qsort(scoreBoard, MAX_SCORES+1 , sizeof (scoreBoard[0]), cmp_structs);
    return 0;
}

/*cmp_structs*/
static int cmp_structs (const void * a, const void * b) {

    scores_t * scoreA = (scores_t *) a;
    scores_t * scoreB = (scores_t *) b;
    int result = (int) ((scoreB->score) - (scoreA->score));
    return result;
}

/*trim_array*/
static uint8_t trim_array (scores_t * scoreBoard){
    strcpy(scoreBoard[MAX_SCORES].name, "");
    scoreBoard[MAX_SCORES].score = 0;

    return 0;
}

/*write_score_board_file*/
static uint8_t write_score_board_file (scores_t * scoreBoard, char * file){
    FILE * scoreBoardFile;
    int i;
    scoreBoardFile = fopen (file, "w");

    if (scoreBoardFile == NULL){
        return -1;
    } else{
        for (i=0;i<=MAX_SCORES;i++){
            fwrite (&scoreBoard[i], sizeof(scores_t) , 1, scoreBoardFile);
        }

        fclose (scoreBoardFile);
        return 0;
    }
}


/*print_scores*/
uint8_t print_scores (scores_t * scoreBoard){

    uint8_t lastPos=0;

    while((strcmp(scoreBoard[lastPos].name, "")) && (scoreBoard[lastPos].name!=NULL)){
        printf("%s: %u\n", scoreBoard[lastPos].name, scoreBoard[lastPos].score );
        lastPos++;
    }
    return 0;
}

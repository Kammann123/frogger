#include "score_board.h"


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
        
        #if PLATFORM_MODE == PC_ALLEGRO
        for (i=0; i<=ALLEGRO_MAX_SCORES; i++){
        #elif PLATFORM_MODE == RPI
	for (i=0; i<=RPI_MAX_SCORES; i++){
        #endif
            fread (&scoresToInit[i], sizeof(scores_t), 1, scoreBoardFile);
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
    
    #if PLATFORM_MODE == PC_ALLEGRO
        qsort(scoreBoard, ALLEGRO_MAX_SCORES+1 , sizeof (scoreBoard[0]), cmp_structs);
    #elif PLATFORM_MODE == RPI
        qsort(scoreBoard, RPI_MAX_SCORES+1 , sizeof (scoreBoard[0]), cmp_structs);
    #endif
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
    #if PLATFORM_MODE == PC_ALLEGRO
        strcpy(scoreBoard[ALLEGRO_MAX_SCORES].name, "");
        scoreBoard[ALLEGRO_MAX_SCORES].score = 0;
    #elif PLATFORM_MODE == RPI
        strcpy(scoreBoard[RPI_MAX_SCORES].name, "");
        scoreBoard[RPI_MAX_SCORES].score = 0;
    #endif    

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
        #if PLATFORM_MODE == PC_ALLEGRO
        for (i=0; i<=ALLEGRO_MAX_SCORES; i++){
        #elif PLATFORM_MODE == RPI
	for (i=0; i<=RPI_MAX_SCORES; i++){
        #endif
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


#include "game_kernel.h"

/**************************/
/* Objetos de la libreria */
/**************************/

// TODO ESTO VA EN EL MAIN DEL GAME KERNEL kernel_main
FROG_CLASS frog;

LANE_LIST lanes = {
    {.id=1, .type=BOAT, .vehiclesQty=3, .direction=MOVE_TO_LEFT, .speed=BOAT_SPEED_BASE},
    {.id=2, .type=BOAT, .vehiclesQty=3, .direction=MOVE_TO_LEFT, .speed=BOAT_SPEED_BASE},
    {.id=3, .type=BOAT, .vehiclesQty=3, .direction=MOVE_TO_LEFT, .speed=BOAT_SPEED_BASE},
    {.id=4, .type=BOAT, .vehiclesQty=3, .direction=MOVE_TO_LEFT, .speed=BOAT_SPEED_BASE},
    {.id=6, .type=BOAT, .vehiclesQty=3, .direction=MOVE_TO_LEFT, .speed=BOAT_SPEED_BASE},
    {.id=7, .type=BOAT, .vehiclesQty=3, .direction=MOVE_TO_LEFT, .speed=BOAT_SPEED_BASE},
    {.id=8, .type=BOAT, .vehiclesQty=3, .direction=MOVE_TO_LEFT, .speed=BOAT_SPEED_BASE},
    {.id=9, .type=BOAT, .vehiclesQty=3, .direction=MOVE_TO_LEFT, .speed=BOAT_SPEED_BASE}
};

/***************************/
/* Definicion de funciones */
/***************************/

/* kernel_init */
bool kernel_init(void){
    // Pon aqui tu codigo xD
}

/* kernel_close */
void kernel_close(void){
    // Pon aqui tu codigo xD
}

/* lane_init */
bool lane_init(LANE* lane, uint16_t id, uint16_t type, uint16_t direction, uint16_t qty, uint16_t speed){
    // Pon aqui tu codigo xD
    lane->id = id;
    lane->type = type;
    lane->direction = direction;
    lane->vehiclesQty = qty;
    lane->speed = speed;
    
    POSITION *vehicles = (POSITION*) malloc(qty*sizeof(POSITION));
    if(vehicles!=NULL){
        lane->vehicles = vehicles;
    }
    else{
        printf("No se pudo crear el carril %d", id);
    }
}

/* lane_close
 * Cierra y libera memoria del carril
 */
void lane_close(LANE* lane){
    // Pon aqui tu codigo xD
}

/* generate_level */
void generate_level(LANE_LIST* lanes, uint16_t level){
    // Pon aqui tu codigo xD
}

/* move_vehicles */
void move_vehicles(void* param){
    // Pon aqui tu codigo xD
    uint8_t i;
    LANE * laneToMove = (LANE *) param;
    for (i=0; i < laneToMove->vehiclesQty; i++){
        if (laneToMove->direction == MOVE_TO_RIGHT){
            
            laneToMove->vehicles[i].x ++;
            
            if (laneToMove->vehicles[i].x == MAP_X_MAX) {       //LLEGO AL FINAL?
                
                laneToMove->vehicles[i].x = MAP_X_MIN;
            }
        } else{
            
            laneToMove->vehicles[i].x --;
            
             if (laneToMove->vehicles[i].x == MAP_X_MIN) {      //LLEGO AL INICIO?
                
                laneToMove->vehicles[i].x = MAP_X_MAX;
            }
        }
    } 
}

/* move_frog */
void move_frog(uint16_t event, FROG_CLASS* frog){
    switch (event) {
		case MOVE_LEFT:
			if (frog->pos.x > FROG_X_MIN){
				frog->pos.x --;
			}
			break;
		case MOVE_RIGHT:
			if (frog->pos.x < FROG_X_MAX){
				frog->pos.x ++;
			}
			break;
		case MOVE_DOWN:
			if (frog->pos.y > FROG_Y_MIN){
				frog->pos.y --;
			}
			break;
		case MOVE_UP:
			if (frog->pos.y < FROG_Y_MAX){
				frog->pos.y ++;
			}
			break;
	}
}

/* collision */
bool collision(FROG_CLASS* frog, LANE_LIST* lanes){
    // Pon aqui tu codigo xD
    int i, j;
    for(i = 0; i<(NUMBER_OF_LANES/2); i++){                                                 //me desplazo por los carriles de vehiculos terrestres
        for(j = 0; j<lanes->vehiclesQty && frog->pos->y == lanes->vehicles->y; j++){        //comparo las posiciones relativas de los vehiculos que se encuentran en el carril de la rana
            if(frog->pos->x == ((lanes->vehicles)+j)->x){
                return TRUE;
            }
        }    
    }
    return FALSE;
}

/* drown */
bool drown(FROG_CLASS* frog, LANE_LIST* lanes){
    // Pon aqui tu codigo xD
}

/* has_won */
bool has_won(FROG_CLASS* frog){
    // Pon aqui tu codigo xD
    if(frog->pos->y == FROG_Y_MAX){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

/* has_lost */
bool has_lost(FROG_CLASS* frog){
    // Pon aqui tu codigo xD
    if(!frog->lifes){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

/* calculate_speed */
uint32_t calculate_speed(uint16_t type, uint16_t level){
    // Pon aqui tu codigo xD
}

/* calculate_score */
uint32_t calculate_score(uint16_t level, uint32_t time){
    //(NIVEL * 20) AL CUADRADO MENOS 5 POR CADA SEGUNDO QUE PASE.
    uint32_t score;
    uint32_t level_base = 20;
    uint32_t time_base = 10;
    score = pow (level_base * level, 2) - 5 * time;
    return score;
}

/* level_up */
void level_up(FROG_CLASS* frog){
    frog->level++;
	frog->pos.x = FROG_X_DEFAULT;
	frog->pos.y = FROG_X_DEFAULT;
}

/* start_level */
void start_level(FROG_CLASS* frog, LANE_LIST* lanes){
	
}

/* reset_level */
void reset_level(FROG_CLASS* frog){
    frog->pos.x = FROG_X_DEFAULT;
	frog->pos.y = FROG_X_DEFAULT;
}

/* restart */
void restart(FROG_CLASS* frog, LANE_LIST* lanes){
    frog->pos.x = FROG_X_DEFAULT;
	frog->pos.y = FROG_X_DEFAULT;
	frog->level = INICIAL_LEVEL;
	frog->lifes = INICIAL_LIFES;
	frog->score = INICIAL_SCORE;
	frog->time = INICIAL_TIME;
}
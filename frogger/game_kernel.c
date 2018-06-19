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
}

/* drown */
bool drown(FROG_CLASS* frog, LANE_LIST* lanes){
    // Pon aqui tu codigo xD
}

/* has_won */
bool has_won(FROG_CLASS* frog){
    // Pon aqui tu codigo xD
}

/* has_lost */
bool has_lost(FROG_CLASS* frog){
    // Pon aqui tu codigo xD
}

/* calculate_speed */
uint32_t calculate_speed(uint16_t type, uint16_t level){
    // Pon aqui tu codigo xD
}

/* calculate_score */
uint32_t calculate_score(uint16_t level, uint32_t time){
    // Pon aqui tu codigo xD
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
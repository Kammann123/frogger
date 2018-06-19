#include "game_kernel.h"

/**************************/
/* Objetos de la libreria */
/**************************/

const ROW rowSettings[] = {
    {.rowNumber=1, .type=BOAT, .vehiclesQty=3},
    {.rowNumber=2, .type=YACHT, .vehiclesQty=3},
    {.rowNumber=3, .type=BOAT, .vehiclesQty=3},
    {.rowNumber=4, .type=BOAT, .vehiclesQty=3},
    {.rowNumber=6, .type=TRUCK, .vehiclesQty=3},
    {.rowNumber=7, .type=CAR, .vehiclesQty=3},
    {.rowNumber=8, .type=M_BIKE, .vehiclesQty=3},
    {.rowNumber=9, .type=CAR, .vehiclesQty=3}
};

OBJECT_LIST kernel_objects;
GAME_DATA kernel_data;

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

/* object_init */
void object_init(OBJECT* list){
    // Pon aqui tu codigo xD
}

/* generate_level */
void generate_level(ROW* rowSettings, uint16_t rowSize, OBJECT* list, uint16_t level){
    // Pon aqui tu codigo xD
}

/* move_vehicles */
void move_vehicles(void* param){
    // Pon aqui tu codigo xD
}

/* move_frog */
void move_frog(uint16_t event, OBJECT* list){
    // Pon aqui tu codigo xD
}

/* collision */
bool collision(OBJECT* list, uint16_t listSize){
    // Pon aqui tu codigo xD
}

/* drown */
bool drown(OBJECT* list, uint16_t listSize){
    // Pon aqui tu codigo xD
}

/* has_won */
bool has_won(OBJECT* list){
    // Pon aqui tu codigo xD
}

/* has_lost */
bool has_lost(GAME_DATA* data){
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
void level_up(GAME_DATA* data){
    // Pon aqui tu codigo xD
}

/* start_level */
void start_level(OBJECT* list, uint16_t listSize){
    // Pon aqui tu codigo xD
}

/* reset_level */
void reset_level(void){
    // Pon aqui tu codigo xD
}

/* restart */
void restart(void){
    // Pon aqui tu codigo xD
}
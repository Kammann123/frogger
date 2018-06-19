#include "allegro_gui.h"

/**************/
/* Constantes */
/**************/
#define DISPLAY_WIDTH   640
#define DISPLAY_HEIGHT  480

/**************************/
/* Objetos de la libreria */
/**************************/

/* Ventana de la interfaz */
static ALLEGRO_DISPLAY* display = NULL;

/**********************/
/* Funciones privadas */
/**********************/

/***************************/
/* Definicion de funciones */
/***************************/

/* allegro_init */
bool allegro_init(void){
    
    /* Inicializacion de los modulos */
    if( !al_init() ){
        return false;
    }
    
    /* Inicializacion de objetos */
    display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if( display == NULL ){
        return false;
    }
    
    return true;
}

/* main_menu_allegro  */
void main_menu_allegro(void){
    // Pon aqui tu codigo xD
}

/* print_frogger_allegro */
void print_frogger_allegro(FROG_CLASS* frog, LANE_LIST* lanes, uint16_t listSize){
    // Pon aqui tu codigo xD
}

/* pause_menu_allegro */
uint16_t pause_menu_allegro(void){
    // Pon aqui tu codigo xD
}

/* input_keyboard */
uint16_t input_keyboard(void){
    // Pon aqui tu codigo xD
}

/* allegro_close */
void allegro_close(void){
    
    /* Cierro modulos utilizados */
    
    /* Destruyo y libero objetos */
    al_destroy_display(display);
}
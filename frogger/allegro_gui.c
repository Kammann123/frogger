#include <stdio.h>

#include "allegro_gui.h"

/**************************/
/* Objetos de la libreria */
/**************************/

/* Ventana de la interfaz */
static ALLEGRO_DISPLAY* display = NULL;

/* Bitmaps precargados */
static ALLEGRO_BITMAP* fieldBitmap;
static ALLEGRO_BITMAP* mBikeBitmap;
static ALLEGRO_BITMAP* carBitmap;
static ALLEGRO_BITMAP* truckBitmap;
static ALLEGRO_BITMAP* boatBitmap;
static ALLEGRO_BITMAP* yachtBitmap;
static ALLEGRO_BITMAP* frogBitmap;

/* Fuentes precargadas */
static ALLEGRO_FONT* font;

/* Textos para imprimir */
static char text[MAX_LENGTH];

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
    
    if( !al_init_primitives_addon() ){
        return false;
    }
    
    if( !al_init_image_addon() ){
        return false;
    }
    
    al_init_font_addon();
    
    if( !al_init_ttf_addon() ){
        return false;
    }
    
    /* Inicializacion de objetos */
    display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);
    if( display == NULL ){
        return false;
    }
    
    /* Inicializacion de bitmaps */
    fieldBitmap = al_load_bitmap(FIELD_IMAGE);
    mBikeBitmap = al_load_bitmap(BIKE_IMAGE);
    carBitmap = al_load_bitmap(CAR_IMAGE);
    truckBitmap = al_load_bitmap(TRUCK_IMAGE);
    boatBitmap = al_load_bitmap(BOAT_IMAGE);
    yachtBitmap = al_load_bitmap(YACHT_IMAGE);
    frogBitmap = al_load_bitmap(FROG_IMAGE);
    if( !fieldBitmap || !mBikeBitmap || !carBitmap || !truckBitmap || !boatBitmap || !yachtBitmap || !frogBitmap ){
        return false;
    }
    
    /* Inicializo las fuentes */
    font = al_load_ttf_font(FONT_FILENAME, FONT_SIZE, 0);
    if( !font ){
        return false;
    }
    
    return true;
}

/* main_menu_allegro  */
void main_menu_allegro(void){    
    // Pon aqui tu codigo xD
}

/* print_frogger_allegro */
void print_frogger_allegro(FROG_CLASS* frog, LANE* lanes){
    uint16_t i, ii;
    
    /* Limpio el display */
    al_clear_to_color( CLEAR_COLOR );
    
    /* Dibujo el campo */
    al_draw_bitmap(fieldBitmap, ARRIVAL_X * DISPLAY_X_STEP, ARRIVAL_Y * DISPLAY_Y_STEP, 0);
    
    /* Dibujo la info de usuario */
    sprintf(text, "%s%d", TIME_MESSAGE, frog->time);
    al_draw_text(font, FONT_COLOR, INFO_X + DISPLAY_X_STEP, INFO_Y * DISPLAY_Y_STEP + FONT_MARGIN, 0, text);
    
    /* Dibujo todos los objetos */
    for(i = 0;i < 6;i++){
        for(ii = 0;ii < lanes[i].vehiclesQty;ii++){
            switch( lanes[i].type ){
                case M_BIKE:
                    al_draw_bitmap(mBikeBitmap, lanes[i].vehicles[ii].x * DISPLAY_X_STEP, lanes[i].vehicles[ii].y * DISPLAY_Y_STEP, 0);
                    break;
                case CAR:
                    al_draw_bitmap(carBitmap, lanes[i].vehicles[ii].x * DISPLAY_X_STEP, lanes[i].vehicles[ii].y * DISPLAY_Y_STEP, 0);
                    break;
                case TRUCK:
                    al_draw_bitmap(truckBitmap, lanes[i].vehicles[ii].x * DISPLAY_X_STEP, lanes[i].vehicles[ii].y * DISPLAY_Y_STEP, 0);
                    break;
                case BOAT:
                    al_draw_bitmap(boatBitmap, lanes[i].vehicles[ii].x * DISPLAY_X_STEP, lanes[i].vehicles[ii].y * DISPLAY_Y_STEP, 0);
                    break;
                case YACHT:
                    al_draw_bitmap(yachtBitmap, lanes[i].vehicles[ii].x * DISPLAY_X_STEP, lanes[i].vehicles[ii].y * DISPLAY_Y_STEP, 0);
                    break;
            }
        }
    }
    
    /* Dibujo la rana */
    al_draw_bitmap(frogBitmap, frog->pos.x * DISPLAY_X_STEP, frog->pos.y * DISPLAY_Y_STEP, 0);
    
    /* Mando el buffer al display */
    al_flip_display();
    
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
    
    /* Destruyo objetos */
    al_destroy_font(font);
}
    #include "rpi_gui.h"

/************************/
/*	Constantes	*/
/************************/

#define PLAY	0
#define	RANK	1
#define	HOWTO	2
#define	EXIT	3

/****************************/
/*  Variables del archivo   */
/****************************/

static uint8_t *mode_selected;

/***************************/
/* Definicion de funciones */
/***************************/

/* main_menu_rpi */
void main_menu_rpi(void){
    // Pon aqui tu codigo xD
    rpi_init();
    const uint8_t index_mode[] = {PLAY, RANK, HOWTO, EXIT};
    mode_selected = index_mode;
    jcoord_t joyCoordinates;
    
    
    joyCoordinates = input_joystick();
    
    if(joyCoordinates.x > 0){            //muevo el joystick a la derecha
        if(*mode_selected < EXIT){
            mode_selected++;
            display_clear();
            print_display_menu();
        }
    }
    else if(joyCoordinates.x < 0){      //muevo el joystick a la izquierda
        if(*mode_selected > PLAY){
            mode_selected--;
            display_clear();
            print_display_menu();
        }
    }
    else{                               //caso en que no muevo el joystick (me fijo si lo presiono
        jswitch_t button;
        button = joystick_get_switch_value();
        switch(button){
            case J_NOPRESS:
                break;
            case J_PRESS:
                display_clear();
                display_write(9,9,D_ON);            //cuelaquiera solo prueba
                display_update();
        }
    }
    
}

/* print_frogger_rpi */
void print_frogger_rpi(FROG_CLASS* frog, LANE_LIST* lanes, uint16_t listSize){
    // Pon aqui tu codigo xD
}

/* pause_menu_rpi */
uint16_t pause_menu_rpi(void){
    // Pon aqui tu codigo xD
}

/* input_joystick */
jcoord_t input_joystick(void){
    // Pon aqui tu codigo xD
    jcoord_t joyCoordinates;
    
    joystick_update();
    joy_coordinates = joystick_get_coord();
    return joyCoordinates;
}

/* rpi_init */
void rpi_init(void){
    // Pon aqui tu codigo xD
    display_init();
    joy_init();
    
    set_display_axis(NORMAL);
    set_joy_axis(NORMAL);
    
}

/* rpi_close */
void rpi_close(void){
    // Pon aqui tu codigo xD
}


void print_display_menu(){
    switch(*mode_selected){
        case PLAY:
            int x = 0 , y = 0;
            bool follow = 0;
            for(y = 0 ; y <= 6 ; y++){
                display_write(x,y, D_ON);
            }
            for(x = 1 , y = 1 ; <= 3 ; x++, y++){
                display_write(x,y, D_ON);
            }
            for(x = 2 , y = 4 ; x > 0 ; x--, y++){
                display_write(x,y, D_ON);
            }
            display_update();
            break;
        case RANK:
            display_write(6,7,D_ON);            // cualquiera solo prueba
            display_update();
        case HOWTO:
            display_write(5,9,D_ON);
            display_update();
        case EXIT:
            display_write(-4,-3,D_ON);
            display_update();
            
    }
}

uint8_t get_mode(){
    return *mode_selected;
}
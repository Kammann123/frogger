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
    jswitch_t button;    
    
    joyCoordinates = input_joystick();
    
    while(button == J_NOPRESS){
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
            button = joystick_get_switch_value();
        }
    }
    
    display_clear();
    display_write(9,9,D_ON);
    display_update();
    
}

/* print_frogger_rpi */
<<<<<<< HEAD
void print_frogger_rpi(FROG_CLASS* frog, LANE* lanes){
    // Pon aqui tu codigo xD
=======
void print_frogger_rpi(FROG_CLASS* frog, LANE_LIST* lanes, uint16_t listSize){
 
    display_write(x,y, D_ON);
    uint8_t i, laneVehiclesI, posxI;
    bool somethingFound = false;
    
    for (i=0; i<listSize; i++){
        
        for (posxI = MAP_X_MIN; posxI < MAP_X_MAX; posxI++){
            
            for (laneVehiclesI = 0; laneVehiclesI < lanes[i].vehiclesQty; laneVehiclesI++){
                
                if ( lanes[i].vehicles[laneVehiclesI].x == posxI){
                    somethingFound= true;
                }         
            }
            
            if (somethingFound){
                display_write(posxI,lanes[i].vehicles[0].y, D_ON);
                somethingFound = false;
                
            } else{
                display_write(posxI,lanes[i].vehicles[0].y, D_OFF);
            }  
        } 
    }
    
    display_write(frog->pos.x,frog->pos.y, D_ON);
  
>>>>>>> 4b72c3aaab5661c0142820b850c04f50f63a0495
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
    joyCoordinates = joystick_get_coord();
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
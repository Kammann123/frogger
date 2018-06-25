    #include "rpi_gui.h"



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
    switch(*mode_selected){
        case PLAY:
                                    //aca poner que salte a la parte del juego 
            break;
        case RANK:
                                    //aca poner la parte de rank
            break;
        case HOWTO:
                                    //saltar al how to play
            break;
        case EXIT:
                                    //poner para salir
            break;
    }
}

/* print_frogger_rpi */


void print_frogger_rpi(FROG_CLASS* frog, LANE_LIST* lanes, uint16_t listSize){
 
    
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
  
}

/* pause_menu_rpi */
uint16_t pause_menu_rpi(void){
    // Pon aqui tu codigo xD
    uint8_t index_pause_mode[] = {EXIT, RESUME, RESTART};
    mode_selected = index_pause_mode;
    jcoord_t joyCoordinates;
    jswitch_t button;    
    
    joyCoordinates = input_joystick();
    
    while(button == J_NOPRESS){
        if(joyCoordinates.x > 0){            //muevo el joystick a la derecha
            if(*mode_selected < RESTART){
              mode_selected++;
              display_clear();
              print_display_menu();
            }
        }
        else if(joyCoordinates.x < 0){      //muevo el joystick a la izquierda
           if(*mode_selected > EXIT){
              mode_selected--;
              display_clear();
              print_display_menu();
            }
        }
        else{                               //caso en que no muevo el joystick (me fijo si lo presiono
            button = joystick_get_switch_value();
        }
    }
    switch(*mode_selected){
        case RESUME:
                                    //aca poner que salte a la parte del juego 
            break;
        case RESTART:
                                    //aca poner la parte de restart
            break;
        case EXIT:
                                    //poner para salir
            break;
    }
}

/* input_joystick */
jcoord_t input_joystick(void){
    jcoord_t joyCoordinates;
    joystick_update();
    joyCoordinates = joystick_get_coord();
    return joyCoordinates;
}


void print_display_menu(){
    switch(*mode_selected){
        case PLAY: case RESUME:
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
            break;
        case HOWTO:
            display_write(5,9,D_ON);
            display_update();
            break;
        case EXIT:
            display_write(-4,-3,D_ON);
            display_update();
            break;
        case RESTART:
            break;
            
    }
}

uint8_t get_mode(){
    return *mode_selected;
}
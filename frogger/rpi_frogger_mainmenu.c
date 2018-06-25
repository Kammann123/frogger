#include "rpi_frogger_mainmenu.h"


void rpi_frogger_mainmenu(const char** options, uint16_t optionSelected){
    int x = 0 , y = 0;
    display_clear();
    switch(optionSelected){
        case PLAY:
            for(y = 0 ; y <= 6 ; y++){
                display_write(x,y, D_ON);
            }
            for(x = 1 , y = 1 ; <= 3 ; x++, y++){
                display_write(x,y, D_ON);
            }
            for(x = 2 , y = 4 ; x > 0 ; x--, y++){
                display_write(x,y, D_ON);
            }
            break;
        case RANK:
            display_write(6,7,D_ON);            // cualquiera solo prueba
            break;
        case HOWTO:
            display_write(5,9,D_ON);
            break;
        case EXIT:
            display_write(-4,-3,D_ON);
            break;
        default:
            break;
    }
    display_update();
}


#include "rpi_init.h"

/* rpi_init */
bool rpi_init(void){
    display_init();
    //set_display_axis(NORMAL);
	return true;
}

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: lucas
 *
 * Created on June 18, 2018, 6:54 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "allegro_gui.h"
#include <unistd.h>

/*
 * 
 */
int main(int argc, char** argv){
    POSITION defPos = {.x = FROG_X_DEFAULT, .y = 9};
    POSITION autos[] = {{.x = 0, .y = 6 }, {.x = 1, .y = 6 }};
    POSITION moto[] = {{.x = 2, .y = 7 }, {.x = 3, .y = 7 }};
    POSITION camion[] = {{.x = 0, .y = 8 }, {.x = 1, .y = 8 }};
    POSITION autos2[] = {{.x = 0, .y = 9 }, {.x = 1, .y = 9 }};
    POSITION boat[] = {{.x = 0, .y = 2 }, {.x = 1, .y = 2 }};
    POSITION yacht[] = {{.x = 2, .y = 4 }, {.x = 3, .y = 4 }};
    FROG_CLASS frog = {.lifes = 3, .level = 0, .time = 10, .score = 0, .pos=defPos};
    LANE_LIST lanes = {
        {.id=0, .type=CAR, .direction=MOVE_TO_LEFT, .vehiclesQty=2, .speed=10, .vehicles=autos},
        {.id=1, .type=M_BIKE, .direction=MOVE_TO_LEFT, .vehiclesQty=2, .speed=10, .vehicles=moto},
        {.id=2, .type=TRUCK, .direction=MOVE_TO_LEFT, .vehiclesQty=2, .speed=10, .vehicles=camion},
        {.id=3, .type=CAR, .direction=MOVE_TO_LEFT, .vehiclesQty=2, .speed=10, .vehicles=autos2},
        {.id=4, .type=BOAT, .direction=MOVE_TO_LEFT, .vehiclesQty=2, .speed=10, .vehicles=boat},
        {.id=5, .type=YACHT, .direction=MOVE_TO_LEFT, .vehiclesQty=2, .speed=10, .vehicles=yacht},
    };
    if( !allegro_init() ){
        return 0;
    }
    
    /* Codigo test */
    print_frogger_allegro(&frog, lanes);
    getchar();
    
    allegro_close();
}


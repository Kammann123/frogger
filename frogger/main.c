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

/*
 * 
 */
int main(int argc, char** argv) {
    if( !allegro_init() ){
        return 0;
    }
    
    /* Codigo test */
    
    allegro_close();
}


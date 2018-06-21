#include "gui_files.h"

/***********************/
/* Constantes privadas */
/***********************/

/* is_section states */
#define IS_SECTION_OPEN     0
#define IS_SECTION_CHAR     1
#define IS_SECTION_CLOSE    2

/***********************************/
/* Prototipo de funciones privadas */
/***********************************/

/* is_section
 * Devuelve true si el string tiene el formato de una seccion
 * del archivo de configuracion: '[seccion]'
 *
 * str: String a evaluar 
 * section: String a guardar la seccion
 */
static bool is_section(char* str, char* section);

/* is_letter
 * Devuelve true si es el ascii de una letra
 * sin importar 
 *
 * letter: Letra a evaluar 
 */
static bool is_letter(char letter);

/************************************/
/* Definicion de funciones privadas */
/************************************/

/* is_section */
static bool is_section(char* str, char* section){
    uint16_t state = IS_SECTION_OPEN;
    
    while( str ){
        switch(state){
            case IS_SECTION_OPEN:
                if( *str == '[' ){
                    state = IS_SECTION_CHAR;
                }else{
                    return false;
                }
                break;
            case IS_SECTION_CHAR:
                if( is_letter(*str) ){
                    *section = *str;
                    section++;
                }else if( *str == ']' ){
                    *section = '\0';
                    state = IS_SECTION_CLOSE;
                }else{
                    return false;
                }
                break;
            case IS_SECTION_CLOSE:
                return false;
                break;
        }
        str++;
    }
    
    return true;
}

/* is_letter */
static bool is_letter(char letter){
    if( letter < 'a' || letter > 'z' ){
        if( letter < 'A' || letter > 'Z' ){
            return false;
        }
    }
    return true;
}
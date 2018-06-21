#include "gui_files.h"

/***********************/
/* Constantes privadas */
/***********************/

/* is_section states */
#define IS_SECTION_OPEN     0
#define IS_SECTION_CHAR     1
#define IS_SECTION_CLOSE    2

/* is_setting states */
#define IS_SETTING_KEY          0
#define IS_SETTING_SPACE        1
#define IS_SETTING_VALUE        2

/***********************************/
/* Prototipo de funciones privadas */
/***********************************/

/* is_setting
 * Devuelve true si es una configuracion bien formateada
 * y la guarda 
 *
 * str: String a comprobar
 * setting: Setting para guardar
 */
static bool is_setting(char* str, SETTING* setting);

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

/* is_number
 * Devuelve true si es el ascii de un numero
 *
 * number: Caracter a evaluar
 */
static bool is_number(char number);

/* is_valid_character
 * Devuelve true si es el ascii es valido
 *
 * character: Caracter a evaluar
 */
static bool is_valid_character(char character);

/************************************/
/* Definicion de funciones privadas */
/************************************/

/* is_setting */
static bool is_setting(char* str, SETTING* setting){
    uint16_t state = IS_SETTING_KEY;
    uint16_t i = 0;
    
    while( str ){
        switch(state){
            case IS_SETTING_KEY:
                if( is_letter(*str) ){
                    setting->key[i++] = *str;
                }else if( *str == ':' ){
                    setting->key[i] = '\0';
                    state = IS_SETTING_SPACE;
                }else{
                    return false;
                }
                break;
            case IS_SETTING_SPACE:
                if( *str == ' ' ){
                    state = IS_SETTING_VALUE;
                    i = 0;
                }else{
                    return false;
                }
                break;
            case IS_SETTING_VALUE:
                if( is_number(*str) || is_letter(*str) || is_valid_character(*str) ){
                    setting->value[i++] = *str;
                }else{
                    return false;
                }
                break;
        }
        str++;
    }
    
    /* Ok, comprobado */
    setting->value[i] = '\0';
    return true;
}

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

/* is_number */
static bool is_number(char number){
    if( number >= '0' && number <= '9' ){
        return true;
    }
    return false;
}

/* is_valid_character */
static bool is_valid_character(char character){
    if( character == '_' ){
        return true;
    }
    return false;
}
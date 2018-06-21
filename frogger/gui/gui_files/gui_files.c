#include <stdio.h>
#include <string.h>

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

/* is_numeric_string
 * Verifica que sea un string numerico
 * 
 * str: String a comprobar
 */
static bool is_numeric_string(char* str);

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

/* section_verify
 * Verifica que no haya error en el formato de la seccion
 *
 * line: Linea leida
 * sectionName: Nombre de la seccion esperado
 */
static bool section_verify(char* line, char* sectionName);

/* setting_verify
 * Verifica que no haya error en el formato de la configuracion
 *
 * line: Linea leida del archivo
 * setting: Donde se guarda la configuracion
 * settingName: Nombre de la configuracion esperada
 */
static bool setting_verify(char* line, SETTING* setting, char* settingName);

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* gui_files_read_objfile */
bool gui_files_read_objfile(char* objFile, ANIMATED_OBJECT* object){
    FILE* file;
    char fileLine[MAX_LINE];
    
    uint16_t state = OBJFILE_STATE_ATTRIBUTE;
    bool error = false;
    
    ANIMATION animation;
    SETTING settings;
    uint16_t frameIndex = 0, i;
    char indexStr[10] = "";
    
    /* Abro el archivo a leer */
    file = fopen(objFile, "r");
    if( file == NULL ){
        return false;
    }
    
    /* Leo linea por linea */
    while( (fgets(fileLine, MAX_LINE, file) != NULL) && !error ){
        /* Verifico que sea linea completa */
        if( fileLine[ strlen(fileLine)-1 ] == '\n' ){
            switch( state ){
                case OBJFILE_STATE_ATTRIBUTE:
                    if( section_verify(fileLine, OBJFILE_ATTRIBUTES) ){
                            state = OBJFILE_STATE_TIMEDELTA;
                    }else{
                        error = true;
                    }
                    break;
                case OBJFILE_STATE_TIMEDELTA:
                    if( setting_verify(fileLine, &settings, OBJFILE_TIMEDELTA) ){
                        if( is_numeric_string(settings.value) ){
                            animation.timeDelta = atoi(settings.value);
                            state = OBJFILE_STATE_SPACEDELTA;
                        }else{
                            error = true;
                        }                        
                    }else{
                        error = true;
                    }
                    break;
                case OBJFILE_STATE_SPACEDELTA:
                    if( setting_verify(fileLine, &settings, OBJFILE_SPACEDELTA) ){
                        if( is_numeric_string(settings.value) ){
                            animation.spaceDelta = atoi(settings.value);
                            state = OBJFILE_STATE_ORIENTATION;
                        }else{
                            error = true;
                        }                        
                    }else{
                        error = true;
                    }
                    break;
                case OBJFILE_STATE_ORIENTATION:
                    if( setting_verify(fileLine, &settings, OBJFILE_ORIENTATION) ){
                        if( !(strcmp(settings.value, OBJFILE_ORIENTATION_HORIZONTAL_LEFT)) ){
                            animation.orientation = GUI_ANIMATION_HORIZONTAL_LEFT;
                            state = OBJFILE_STATE_QUANTITY;
                        }else if( !(strcmp(settings.value, OBJFILE_ORIENTATION_HORIZONTAL_RIGHT)) ){
                            animation.orientation = GUI_ANIMATION_HORIZONTAL_RIGHT;
                            state = OBJFILE_STATE_QUANTITY;
                        }else if( !(strcmp(settings.value, OBJFILE_ORIENTATION_VERTICAL_UP)) ){
                            animation.orientation = GUI_ANIMATION_VERTICAL_UP;
                            state = OBJFILE_STATE_QUANTITY;
                        }else if( !(strcmp(settings.value, OBJFILE_ORIENTATION_VERTICAL_DOWN)) ){
                            animation.orientation = GUI_ANIMATION_VERTICAL_DOWN;
                            state = OBJFILE_STATE_QUANTITY;
                        }else{
                            error = true;
                        }
                    }else{
                        error = true;
                    }
                    break;
                case OBJFILE_STATE_QUANTITY:
                    if( setting_verify(fileLine, &settings, OBJFILE_QUANTITY) ){
                        if( is_numeric_string(settings.value) ){
                            animation.framesQty = atoi(settings.value);
                            animation.frames = gui_animation_create_framelist(animation.framesQty);
                            state = OBJFILE_STATE_FRAME_SECTION;
                        }else{
                            error = true;
                        }
                    }else{
                        error = true;
                    }
                    break;
                case OBJFILE_STATE_FRAME_SECTION:
                    if( section_verify(fileLine, OBJFILE_FRAMES) ){
                            state = OBJFILE_STATE_FRAMES;
                    }else{
                        error = true;
                    }
                    break;
                case OBJFILE_STATE_FRAMES:
                    /* Compruebo que la linea sea un nuevo frame */
                    if( fileLine[0] != '\n' ){
                        /* Verifico que no se haya pasado */
                        if( frameIndex < animation.framesQty ){
                            /* Creo el string con el numero de indice de frame esperado */
                            sprintf(indexStr, "%d", frameIndex);

                            /* Verifico formato de linea y numero de indice */
                            if( setting_verify(fileLine, &settings, indexStr) ){
                                animation.frames[frameIndex] = gui_animation_create_frame(settings.value);
                            }else{
                                error = true;
                            }

                            /* Me muevo al proximo frame */
                            frameIndex++;
                        }else{
                            error = true;
                        }
                    }else{
                        error = true;
                    }
                    break;
            }
        }else{
            error = true;
        }
    }
    
    /* Cierro el archivo */
    fclose(file);
    
    /* Si hubo error y llego a reservar memoria, la libera */
    if( error ){
        if( state >= OBJFILE_STATE_FRAME_SECTION ){
            gui_animation_destroy_framelist(animation.frames, animation.framesQty);
        }
    }else{
        for(i = 0;i < NUMBER_OF_ORIENTATIONS;i++){
            if( object->animations[i].orientation == GUI_ANIMATION_ORIENTATION_NULL ){
                object->animations[i] = animation;
                break;
            }
        }
    }
    
    /* Devuelve si el proceso fue exitoso */
    return !error;
}

/************************************/
/* Definicion de funciones privadas */
/************************************/

/* setting_verify */
static bool setting_verify(char* line, SETTING* setting, char* settingName){ 
    if( is_setting(line, setting) ){
        if( !(strcmp(setting->key, settingName)) ){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
} 

/* section_verify */
static bool section_verify(char* line, char* sectionName){
    char section[MAX_STRING];
    
    if( is_section(line, section) ){
        if( !(strcmp(section, sectionName)) ){
            return true;
        }else{
            return false;
        }
    }else{
        return false;
    }
}

/* is_setting */
static bool is_setting(char* str, SETTING* setting){
    uint16_t state = IS_SETTING_KEY;
    uint16_t i = 0;
    
    while( str ){
        switch(state){
            case IS_SETTING_KEY:
                if( is_letter(*str) || is_number(*str) ){
                    if( i >= MAX_STRING ){
                        return false;
                    }
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
                    if( i >= MAX_STRING ){
                        return false;
                    }
                    setting->value[i++] = *str;
                }else if( *str == '\n' ){
                    /* Ok, comprobado */
                    setting->value[i] = '\0';
                    return true;
                }else{
                    return false;
                }
                break;
        }
        str++;
    }
}

/* is_section */
static bool is_section(char* str, char* section){
    uint16_t state = IS_SECTION_OPEN;
    uint16_t i = 0;
    
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
                    if(i >= MAX_STRING){
                        return false;
                    }
                    *section = *str;
                    section++;
                    i++;
                }else if( *str == ']' ){
                    *section = '\0';
                    state = IS_SECTION_CLOSE;
                }else{
                    return false;
                }
                break;
            case IS_SECTION_CLOSE:
                if( *str == '\n' ){
                    return true;
                }else{
                    return false;
                }
                break;
        }
        str++;
    }
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
    if( character == '_' || character == '/' || character == '.' ){
        return true;
    }
    return false;
}

/* is_numeric_string */
static bool is_numeric_string(char* str){
    if( is_number(*str) ){
        return is_numeric_string( str+1);
    }else if( !(*str) ){
        return true;
    }else{
        return false;
    }
}
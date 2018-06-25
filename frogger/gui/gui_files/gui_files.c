#include <stdio.h>
#include <string.h>

#include "gui_files.h"

/***********************/
/* Constantes privadas */
/***********************/

typedef enum {
    IS_SET_KEY, 
    IS_SET_SPACE, 
    IS_SET_VALUE, 
    IS_SET_END, 
    IS_SET_OK, 
    IS_SET_ERROR
} is_set_states;

typedef enum {
    IS_SECTION_OPEN,
    IS_SECTION_CHAR,
    IS_SECTION_CLOSE,
    IS_SECTION_OK,
    IS_SECTION_ERROR,
    IS_SECTION_END
} is_section_states;

typedef enum {
    SETTING_WAIT,
    SETTING_ERROR,
    SETTING_OK
} setting_states;

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

/* is_set
 * Lee una linea de set y la devuelve instanciada
 * si hubo error, sera NULL
 *
 * str: Linea leida
 * set: Set
 */
static bool is_set(char* str, SET* set);

/* strlen_end 
 * Devuelve el largo de un string, medido
 * hasta el caracter pasado 
 * 
 * str: String
 * end: Caracter terminador
 */
static uint32_t strlen_end(char* str, char end);

/* in_string
 * Devuelve true si el caracter esta en el
 * string
 *
 * str: String
 * character: Caracter
 */
static bool in_string(char* str, char character);

/* destroy_set
 * Libera memoria de un set
 * 
 * set: Set 
 */
static void destroy_set(SET* set);

/* is_letter
 * Devuelve true si es el ascii de una letra
 * sin importar 
 *
 * letter: Letra a evaluar 
 */
static bool is_letter(char letter);

/* is_valid_character
 * Devuelve true si es el ascii es valido
 *
 * character: Caracter a evaluar
 */
static bool is_valid_character(char character);

/* is_number
 * Devuelve true si es el ascii de un numero
 *
 * number: Caracter a evaluar
 */
static bool is_number(char number);

/* read_section
 * Lee una seccion a donde apunta el archivo
 * hasta fin de seccion, esto es, una linea vacia
 *
 * file: Archivo
 * name: Nombre del archivo
 * section: Seccion
 */
static bool read_section(FILE* file, char* name, SECTION* section);

/* destroy_section
 * Libera memoria de una seccion
 * 
 * section: Seccion 
 */
static void destroy_section(SECTION* section);

/* is_empty_line
 * Devuelve true si la linea esta vacia
 * 
 * str: String
 */
static bool is_empty_line(char* str);

/* count_section_lines
 * Cuenta los sets de una seccion
 *
 * file: Archivo
 */
static uint32_t count_section_lines(FILE* file);

/* is_numeric_string
 * Verifica que sea un string numerico
 * 
 * str: String a comprobar
 */
static bool is_numeric_string(char* str);

/* count_section 
 * Cuenta la cantidad de secciones que hay
 *
 * file:Archivo 
 */
static uint32_t count_sections(FILE* file);

/* create_sets
 * Crea un arreglo de sets y los inicializa
 *
 * amount: Cantidad de sets
 */
static SET* create_sets(uint32_t amount);

/* create_sections
 * Crea un arreglo de secciones y los inicializa
 *
 * amount: Cantidad de secciones
 */
static SECTION* create_sections(uint32_t amount);

/************************************/
/* Definicion de funciones publicas */
/************************************/

/* gui_files_get_section_length */
LENGTH gui_files_get_section_length(SETTING* setting, char* section){
    uint32_t i;
    
    for(i = 0;i < setting->length;i++){
        if( !(strcmp(setting->sections[i].name, section)) ){
            return setting->sections[i].length;
        }
    }
    return 0;
}

/* gui_files_get_string */
char* gui_files_get_string(SETTING* setting, char* section, char* key){
    uint32_t i, ii;
    
    for(i = 0;i < setting->length;i++){
        if( !(strcmp(setting->sections[i].name, section)) ){
            for(ii = 0;ii < setting->sections[i].length;ii++){
                if( !(strcmp(setting->sections[i].sets[ii].key, key)) ){
                    return setting->sections[i].sets[ii].value.string;
                }
            }
        }
    }
    return NULL;
}

/* gui_files_get_int */
bool gui_files_get_int(SETTING* setting, char* section, char* key, uint32_t* value){
    uint32_t i, ii;
    
    for(i = 0;i < setting->length;i++){
        if( !(strcmp(setting->sections[i].name, section)) ){
            for(ii = 0;ii < setting->sections[i].length;ii++){
                if( !(strcmp(setting->sections[i].sets[ii].key, key)) ){
                    *value = setting->sections[i].sets[ii].value.integer;
                    return true;
                }
            }
        }
    }
    return false;
}

/* gui_files_get_bool */
bool gui_files_get_bool(SETTING* setting, char* section, char* key, bool* value){
    uint32_t i, ii;
    
    for(i = 0;i < setting->length;i++){
        if( !(strcmp(setting->sections[i].name, section)) ){
            for(ii = 0;ii < setting->sections[i].length;ii++){
                if( !(strcmp(setting->sections[i].sets[ii].key, key)) ){
                    *value = setting->sections[i].sets[ii].value.boolean;
                    return true;
                }
            }
        }
    }
    return false;
}

/* gui_files_destroy_setting */
void gui_files_destroy_setting(SETTING* setting){
    uint32_t i;
    
    /* Libero secciones */
    for(i = 0;i < setting->length;i++){
        destroy_section(setting->sections + i);
    }
    
    /* Libero la lista */
    free( setting->sections );
    
    /* Libero el setting */
    free( setting );
}

/* gui_files_load_setting */
    SETTING* gui_files_load_setting(char* filename){
    char line[MAX_LINE];
    char sectionName[MAX_LINE];
    
    /* Variables para archivo */
    FILE* file;
    
    /* Variables del objeto setting */
    SETTING* setting;
    uint32_t settingLength;
    uint32_t i = 0;
    
    /* Variable estado */
    uint16_t state = SETTING_WAIT;
    
    /* Abro el archivo */
    file = fopen(filename, "r");
    if( file == NULL ){
        return NULL;
    }
    
    /* Reservo memoria para el setting */
    setting = malloc( sizeof(SETTING) );
    if( setting == NULL ){
        fclose(file);
        return NULL;
    }
    
    /* Resservo memoria para las secciones */
    settingLength = count_sections(file);
    if( !settingLength ){
        free(setting);
        fclose(file);
        return NULL;
    }
    setting->sections = create_sections(settingLength);
    if( setting->sections == NULL ){
        free(setting);
        fclose(file);
        return NULL;
    }
    
    /* Cargo largo */
    setting->length = settingLength;
    
    /* Leo el archivo */
    while( state == SETTING_WAIT ){
        if( fgets(line, MAX_LINE, file) == NULL ){
            state = SETTING_ERROR;
        }else{
            if( !(strcmp(line, SETTING_END)) ){
                state = SETTING_OK;
            }else{
                if( is_section(line, sectionName) ){
                    if( !read_section(file, sectionName, setting->sections + i) ){
                        state = SETTING_ERROR;
                    }
                    i++;
                }else{
                    state = SETTING_ERROR;
                }
            }
        }
    }
    
    /* Cierro el archivo */
    fclose(file);
    
    if( state == SETTING_OK ){
        return setting;
    }else if( state == SETTING_ERROR ){
        gui_files_destroy_setting(setting);
        return NULL;
    }
}

/************************************/
/* Definicion de funciones privadas */
/************************************/

static uint32_t count_sections(FILE* file){
    uint32_t length = 0;
    char line[MAX_LINE];
    char section[MAX_LINE];
    bool found = false;
    
    /* Leo todas las lineas hasta el final */
    while( (fgets(line, MAX_LINE, file) != NULL) && !found ){
        
        if( is_section(line, section) ){
            length++;
        }else if( !(strcmp(line, SETTING_END)) ){
            found = true;
        }
    }    
    
    /* Rebobino el archivo */
    rewind(file);
    
    if( !found ){
        length = 0;
    }
    
    return length;
}

static SECTION* create_sections(uint32_t amount){
    uint32_t i;
    SECTION* sections;
    
    /* Reservo memoria */
    sections = malloc( sizeof(SECTION) * amount );
    if( sections == NULL ){
        return NULL;
    }
    
    /* Las inicializo */
    for(i = 0;i < amount;i++){
        sections[i].init = false;
    }
    
    return sections;
}

static SET* create_sets(uint32_t amount){
    uint32_t i;
    SET* sets;
    
    /* Reservo memoria */
    sets = malloc( sizeof(SET) * amount );
    if( sets == NULL ){
        return NULL;
    }
    
    /* Inicializo */
    for(i = 0;i < amount;i++){
        sets[i].init = false;
    }
    
    return sets;
}

/* read_section */
static bool read_section(FILE* file, char* name, SECTION* section){
    /* Variables para seccion */
    uint32_t i;
    uint32_t sectionLength;
    SET set;
    
    /* Buffer de linea */
    char line[MAX_LINE];
    
    /* Lectura previa para leer */
    sectionLength = count_section_lines(file);
    if( !sectionLength ){
        return false;
    }
    
    /* Reservo memoria para sets */
    section->sets = create_sets(sectionLength);
    if(section->sets == NULL){
        free(section);
        return false;
    }
    
    /* Reservo memoria para el nombre de la seccion */
    section->name = malloc( sizeof(char) * strlen(name) );
    if(section->name == NULL){
        free(section->sets);
        free(section);
        return false;
    }
    
    /* Guardo el nombre */
    strcpy(section->name, name);
    
    /* Inicialice, flag */
    section->init = true;
    
    /* Leo las lineas */
    for(i = 0;i < sectionLength;i++){
        /* Leo la linea */
        fgets(line, MAX_LINE, file);
        
        /* Leo el set */
        if( !is_set(line, &set) ){
            destroy_section(section);
            return false;
        }
        
        /* Guardo el set */
        section->sets[i] = set;
        
    }
    
    /* Configuro largo */
    section->length = sectionLength;
    
    /* Leo la linea de cierre */
    fgets(line, MAX_LINE, file);
    
    return true;
}

/* destroy_section */
static void destroy_section(SECTION* section){
    uint32_t i;
    
    if( section->init ){
        /* Liberas memoria del nombre */
        free(section->name);

        /* Liberas memoria de las secciones */
        for(i = 0;i < section->length;i++){
            destroy_set(section->sets + i);
        }
        
        free(section->sets);
    }
}

/* count_section_lines */
static uint32_t count_section_lines(FILE* file){
    char line[MAX_LINE];
    fpos_t initPos;
    uint32_t length = 0;
    bool found = false;
    
    /* Guardo posicion inicial */
    if( fgetpos(file, &initPos) ){
        return 0;
    }
    
    /* Cuento lineas */
    while( (fgets(line, MAX_LINE-1, file) != NULL) && !found ){
        if( line[strlen(line)-1] != '\n' ){
            break;
        }else if( !is_empty_line(line) ){
            length++;
        }else{
            found = true;
        }
    }
    
    /* Pongo posicion inicial */
    if( fsetpos(file, &initPos) ){
        return 0;
    }
    
    if( !found ){
        length = 0;
    }
    
    return length;
}

/* is_section */
static bool is_section(char* str, char* section){
    uint16_t state = IS_SECTION_OPEN;
    uint16_t i = 0;
    
    while( state != IS_SECTION_ERROR && state != IS_SECTION_OK ){
        switch(state){
            case IS_SECTION_OPEN:
                if( *str == '[' ){
                    state = IS_SECTION_CHAR;
                }else{
                    state = IS_SECTION_ERROR;
                }
                break;
            case IS_SECTION_CHAR:
                if( is_letter(*str) ){
                    if(i >= MAX_LINE){
                        state = IS_SECTION_ERROR;
                    }
                    *section = *str;
                    section++;
                    i++;
                }else if( *str == ']' ){
                    *section = '\0';
                    state = IS_SECTION_CLOSE;
                }else{
                    state = IS_SECTION_ERROR;
                }
                break;
            case IS_SECTION_CLOSE:
                if( *str == '\n' ){
                    state = IS_SECTION_END;
                }else{
                    state = IS_SECTION_ERROR;
                }
                break;
            case IS_SECTION_END:
                if( *str == '\0' ){
                    state = IS_SECTION_OK;
                }else{
                    state = IS_SECTION_ERROR;
                }
        }
        str++;
    }
    
    if( state == IS_SECTION_OK ){
        return true;
    }else if( state == IS_SECTION_ERROR ){
        return false;
    }
    
}

/* is_number */
static bool is_number(char number){
    if( number >= '0' && number <= '9' ){
        return true;
    }
    return false;
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

/* is_valid_character */
static bool is_valid_character(char character){
    if( character == '_' || character == '/' || character == '.' ){
        return true;
    }else if( character == '\''){
        return true;
    }
    return false;
}

/* is_set */
static bool is_set(char* str, SET* set){
    /* Variables para el set */
    uint32_t keyLength;
    uint32_t valueLength;
    
    /* Estado de la maquina */
    uint16_t state = IS_SET_KEY;
    
    /* Auxiliares */
    uint32_t i = 0;
    
    /* Me fijo que este bien formateado */
    if( !in_string(str, ':') ){
        return false;
    }
    
    /* Prelectura para saber largo */
    keyLength = strlen_end(str, ':');
    valueLength = strlen_end(str + keyLength, '\0');
    
    /* Reservo memoria para el key */
    set->key = malloc( sizeof(char) * keyLength );
    if( set->key == NULL ){
        return false;
    }

    /* Reservo memoria para el value */
    set->value.string = malloc( sizeof(char) * valueLength );
    if( set->value.string == NULL ){
        free(set->key);
        return false;
    }
        
    /* Inicialice, flag */
    set->init = true;
    
    while( state != IS_SET_OK && state != IS_SET_ERROR ){
        switch( state ){
            case IS_SET_KEY:
                if( *str != ':' ){
                    if( is_letter(*str) || is_number(*str) || is_valid_character(*str) ){
                        set->key[i++] = *str;
                    }else{
                        state = IS_SET_ERROR;
                    }
                }else{
                    set->key[i] = '\0';
                    i = 0;
                    state = IS_SET_SPACE;
                }
                break;
            case IS_SET_SPACE:
                if( *str == ' ' ){
                    state = IS_SET_VALUE;
                }else{
                    state = IS_SET_ERROR;
                }             
                break;
            case IS_SET_VALUE:
                if( is_letter(*str) || is_number(*str) || is_valid_character(*str) ){
                    set->value.string[i++] = *str; 
                }else if( *str == '\n' ){
                    set->value.string[i] = '\0';
                    state = IS_SET_OK;
                }else{
                    state = IS_SET_ERROR;
                }
                break;
        }
        
        str++;
    }
    
    if( state == IS_SET_OK ){
        
        /* Interpreto el valor */
        if( is_numeric_string(set->value.string) ){
            set->value.integer = atoi( set->value.string );
        }else if( !(strcmp(set->value.string, "false")) ){
            set->value.boolean = false;
        }else if( !(strcmp(set->value.string, "true")) ){
            set->value.boolean = true;
        }
        
        return true;
    }else if( state == IS_SET_ERROR ){
        destroy_set(set);
        return false;
    }
    
}

/* in_string */
static bool in_string(char* str, char character){
    while( str ){
        if( *str == character ){
            return true;
        }
        str++;
    }
    return false;
}

/* strlen_end */
static uint32_t strlen_end(char* str, char end){
    if( *str == end || *str == '\0' ){
        return 1;
    }else{
        return 1 + strlen_end(str+1, end);
    }
}

/* destroy_set */
static void destroy_set(SET* set){
    
    if( set->init ){
        /* Libero memoria del string */
        free(set->value.string);

        /* Libero memoria del key */
        free(set->key);
    }
    
}

/* is_empty_line */
static bool is_empty_line(char* str){
    if( *str == '\0' ){
        return true;
    }else if( *str == '\n' ){
        return is_empty_line( str+1 );
    }else if( *str == ' ' ){
        return is_empty_line( str+1 );
    }else{
        return false;
    }
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
/* CONSOLE_ARGS
 */

#include "console_args.h"

/*******************/
/* STATIC handlers */
/*******************/

/* is_number
 * Devuelve true si es numero el caracter
 *
 * str: Caracter
 */
static bool is_number(char str);

/* is_letter
 * Devuelve true si es letra el caracter
 *
 * str: Caracter
 */
static bool is_letter(char str);

/* is_numeric_string
 * Devuelve TRUE si el string es solamente numerico
 *
 * str: String 
 */
static bool is_numeric_string(char* str);

/* is_boolean_string
 * Devuelve TRUE si el string es un valor booleano
 *
 * str: String
 * bool: Te guarda el resultado
 */
static bool is_boolean_string(char* str, bool* value);

/********************/
/* ENTRIES handlers */
/********************/

/* entries_get_int */
bool entries_get_int(ENTRIES* entries, char* command, uint32_t* integer){    
    LENGTH i;
    
    /* Recorro las entradas */
    for(i = 0;i < entries->length;i++){
        /* Busco el comando */
        if( !( strcmp(entries->entryList[i].command, command) ) ){
            *integer = entries->entryList[i].value.integer;
            return true;
        }
    }
    return false;
}

/* entries_get_string */
char* entries_get_string(ENTRIES* entries, char* command){
    LENGTH i;
    
    /* Recorro las entradas */
    for(i = 0;i < entries->length;i++){
        /* Busco el comando */
        if( !( strcmp(entries->entryList[i].command, command) ) ){
            return entries->entryList[i].value.string;
        }
    }
    return NULL;
}

/* read_entries */
bool read_entries(ENTRIES* entries, char** str, LENGTH length){
    LENGTH i;
    
    /* Creo la instancia */
    *entries = entries_create();
    
    /* Inicializo */
    if( !entries_init(entries, length) ){
        return false;
    }
    
    /* Leo cada una */
    for(i = 0;i < length;i++){
        
        /* Verifico ok */
        if( !read_entry(&entries->entryList[i], str[i]) ){
            entries_destroy(entries);
            return false;
        }
    }
    
    /* Exito */
    return true;
}

/* entries_create */
ENTRIES entries_create(void){
    ENTRIES entries;
    
    /* Parametros por defecto */
    entries.entryList = NULL;
    entries.length = 0;
    entries.init = false;
    
    /* Devuelvo */
    return entries;
}

/* entries_init */
bool entries_init(ENTRIES* entries, LENGTH length){
    LENGTH i;
    
    /* Reservo memoria */
    entries->entryList = malloc( sizeof(ENTRY) * length );
    if( entries->entryList == NULL ){
        return false;
    }
    
    /* Set to NULL */
    for(i = 0;i < length;i++){
        entries->entryList[i] = entry_create();
    }
    
    /* Inicializado!! */
    entries->init = true;
    entries->length = length;
    
    /* OK! */
    return true;
}

/* entries_destroy */
void entries_destroy(ENTRIES* entries){
    LENGTH i;
    
    /* Verifico inicializacion */
    if( entries->init ){
        
        /* Verificacion */
        if( entries->entryList == NULL ){
            return;
        }
        
        /* Libero memoria de entries */
        for(i = 0;i < entries->length;i++){
            /* Libero y destruyo entradas */
            entry_destroy( &entries->entryList[i] );
        }
        
        /* Libero arreglo */
        free( entries->entryList );
    }
    
    /* Apago */
    entries->init = false;
}

/******************/
/* ENTRY handlers */
/******************/

/* read_entry */
bool read_entry(ENTRY* entry, char* str){
    READ_ENTRY_FSM  state = ENTRY_FSM_OPEN;
    
    STRING command = "";
    STRING value = "";
    LENGTH i = 0;
    
    /* Creo la instancia */
    *entry = entry_create();

    while( (state != ENTRY_FSM_OK) && (state != ENTRY_FSM_ERROR) ){
        /* Leo el caracter */
        switch( state ){
            case ENTRY_FSM_OPEN:
                if( *str == '-' ){
                    state = ENTRY_FSM_COMMAND;
                }else{
                    state = ENTRY_FSM_ERROR;
                }
                break;
            case ENTRY_FSM_COMMAND:
                if( is_number(*str) || is_letter(*str) ){
                    if( i >= MAX_STRING ){
                        state = ENTRY_FSM_ERROR;
                    }else{
                        command[i++] = *str;
                    }
                }else if( *str == '=' ){
                    command[i++] = '\0';
                    i = 0;
                    state = ENTRY_FSM_VALUE;
                }else if( *str == '\0' ){
                    state = ENTRY_FSM_OK;
                }else{
                    state = ENTRY_FSM_ERROR;
                }
                break;
            case ENTRY_FSM_VALUE:
                if( is_number(*str) || is_letter(*str) ){
                    if( i >= MAX_STRING ){
                        state = ENTRY_FSM_ERROR;
                    }else{
                        value[i++] = *str;
                    }
                }else if( *str == '\0' ){
                    value[i++] = '\0';
                    i = 0;                    
                    state = ENTRY_FSM_OK;
                }else{
                    state = ENTRY_FSM_ERROR;
                }
                break;
                
        }
        /* Muevo la cadena */
        str++;
    }
    
    /* Manejo resultados */
    if( state == ENTRY_FSM_OK ){
        if( !entry_init(entry, command) ){
            return false;
        }else{
            if( !entry_set_value(entry, value) ){
                entry_destroy(entry);
                return false;
            }
        }
        return true;
    }
    
    return false;
}

/* entry_create */
ENTRY entry_create(void){
    ENTRY entry;
    
    /* Parametros por defecto */
    entry.init = false;
    entry.command = NULL;
    entry.hasValue = false;
    
    /* Devuelvo */
    return entry;
}

/* entry_init */
bool entry_init(ENTRY* entry, char* command){
    LENGTH length;
    
    /* Calculo cantidad de memoria */
    length = strlen(command) + 1;
    
    /* Reservo memoria */
    entry->command = malloc( sizeof(char) * length );
    if( entry->command == NULL ){
        return false;
    }
    
    /* Guardo e inicializo */
    strcpy(entry->command, command);
    entry->init = true;
    
    /* Exito */
    return true;
}

/* entry_destroy */
void entry_destroy(ENTRY* entry){
    /* Verifico inicializacion */
    if( entry->init ){
        
        /* Libero memoria */
        if( entry->command != NULL ){
            free( entry->command );
        }
        
        /* Destruyo contenido */
        entry_value_destroy(&entry->value);
    }
    
    /* Apago */
    entry->init = false;
}

/* entry_set_value */
bool entry_set_value(ENTRY* entry, char* value){
    
    /* Verifico el entry */
    if( entry == NULL ){
        return false;
    }
    
    if( value == NULL ){
        return false;
    }
    
    /* Creo la instancia */
    entry->value = entry_value_create();
    
    /* Inicializo */
    if( !entry_value_init(&entry->value, value) ){
        return false;
    }
    
    if( strcmp(value, "") ){
        entry->hasValue = true;
    }
    
    /* Exito */
    return true;
}

/************************/
/* ENTRY_VALUE handlers */
/************************/

/* entry_value_create */
ENTRY_VALUE entry_value_create(void){
    ENTRY_VALUE entry;
    
    /* Parametros por defecto */
    entry.init = false;
    entry.string = NULL;
    
    /* Devuelvo instancia */
    return entry;
}

/* entry_value_init */
bool entry_value_init(ENTRY_VALUE* entry, char* value){
    LENGTH length;
    
    /* Calculo cantidad de memoria */
    length = strlen(value) + 1;
    
    /* Reservo memoria para la entry */
    entry->string = malloc( sizeof(char) * length );
    if( entry->string == NULL ){
        return false;
    }
    
    /* Guardo e inicializo */
    strcpy(entry->string, value);
    entry->init = true;
    
    /* Completo otros campos si es necesario */
    if( !is_boolean_string(value, &entry->boolean) ){
        if( is_numeric_string(value) ){
            entry->integer = atoi(value);
        }
    }
    
    /* Exito */
    return true;
}

/* entry_value_destroy */
void entry_value_destroy(ENTRY_VALUE* entry){
    /* Verifico inicializacion */
    if( entry->init ){
        /* Libero memoria */
        if( entry->string != NULL ){
            free(entry->string);
        }
    }
    
    /* Apago */
    entry->init = false;
}

/*******************/
/* STATIC handlers */
/*******************/

/* is_numeric_string */
static bool is_numeric_string(char* str){
    if( *str >= '0' && *str <= '9' ){
        return is_numeric_string(str + 1);
    }else if( *str == '\0' ){
        return true;
    }else{
        return false;
    }
}

/* is_boolean_string */
static bool is_boolean_string(char* str, bool* value){
    if( !( strcmp(str, "false") ) ){
        *value = false;
        return true;
    }else if( !( strcmp(str, "true") ) ){
        *value = true;
        return true;
    }
    return false;
}

/* is_number*/
static bool is_number(char str){
    if( str >= '0' && str<= '9' ){
        return true;
    }
    return false;
}

/* is_letter*/
static bool is_letter(char str){
    if( str < 'a' || str > 'z' ){
        if( str < 'A' || str > 'Z' ){
            return false;
        }
    }
    return true;
}
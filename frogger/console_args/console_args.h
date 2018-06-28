/* CONSOLE_ARGS
 * Interpreta comandos por consola con la siguiente sintaxis:
 * '-COMMAND' -> Ingresan un comando cualquiera
 * '-KEY=VALUE' -> Ingresa un comando/clave y su valor de ejecucion
 */

#ifndef CONSOLE_ARGS_H
#define CONSOLE_ARGS_H

/* Librerias utilizadas */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../gui/gui_types.h"

/* Tipos de datos */
 
typedef uint32_t LENGTH;

typedef struct{
    /* Tipos de datos almacenados */
    char* string;
    uint32_t integer;
    bool boolean;
    
    /* Inicializado */
    bool init;
} ENTRY_VALUE;

typedef struct{
    /* Entradas */
    char* command;
    ENTRY_VALUE value;
    
    /* Flags */
    bool hasValue;
    
    /* Inicializado */
    bool init;
} ENTRY;

typedef struct{
    /* Arreglo con entradas */
    ENTRY* entryList;
    LENGTH length;
    
    /* Inicializado */
    bool init;
} ENTRIES;

/* Constantes de FSM */

typedef enum{
    ENTRY_FSM_OPEN,
    ENTRY_FSM_COMMAND,
    ENTRY_FSM_VALUE,
    ENTRY_FSM_OK,
    ENTRY_FSM_ERROR
} READ_ENTRY_FSM;

/********************/
/* ENTRIES handlers */
/********************/

/* entries_get_int
 * Devuelve el valor int
 *
 * entries: Instancia
 * commmand: Comando
 * integer: Donde se guarda
 */
bool entries_get_int(ENTRIES* entries, char* command, uint32_t* integer);

/* entries_get_string
 * Devuelve el valor string
 *
 * entries: Instancia
 * command: Comando
 */
char* entries_get_string(ENTRIES* entries, char* command);

/* read_entries
 * Interpreta una secuencia de entradas
 *
 * entries: Instancia
 * str: Entradas
 * length: Cantidad de entradas
 */
bool read_entries(ENTRIES* entries, char** str, LENGTH length);

/* entries_create
 * Crea una instancia de entradas
 */
ENTRIES entries_create(void);

/* entries_init
 * Inicializa memoria para una instancia entries
 * 
 * entries: Instancia
 * length: Cantidad de entradas
 */
bool entries_init(ENTRIES* entries, LENGTH length);

/* entries_destroy
 * Destruye y libera instancia
 *
 * entries: Instancia
 */
void entries_destroy(ENTRIES* entries);

/******************/
/* ENTRY handlers */
/******************/

/* read_entry
 * Lee una entrada y la guarda en una instancia
 *
 * str: String de la entrada
 */
bool read_entry(ENTRY* entry, char* str);

/* entry_create
 * Crea una instancia de ENTRY
 */
ENTRY entry_create(void);

/* entry_init
 * Inicializa memoria para un entry 
 * 
 * entry: Instancia
 * command: Comando a contener
 */
bool entry_init(ENTRY* entry, char* command);

/* entry_destroy
 * Destruye la intancia y libera memoria
 *
 * entry: Instancia
 */
void entry_destroy(ENTRY* entry);

/* entry_set_value 
 * Carga un valor a una entry
 *
 * entry: Instancia
 * value: Valor de la entrada
 */
bool entry_set_value(ENTRY* entry, char* value);

/************************/
/* ENTRY_VALUE handlers */
/************************/

/* entry_value_create 
 * Crea una instancia de valor de una entrada de consola
 */
ENTRY_VALUE entry_value_create(void);

/* entry_value_init
 * Inicializa memoria para la instancia con el valor asignado
 *
 * entry: Instancia de la entrada
 * value: Valor a contener
 */
bool entry_value_init(ENTRY_VALUE* entry, char* value);

/* entry_value_destroy
 * Libera la memoria almacenada por el valor de entrada
 *
 * entry : Instancia 
 */
void entry_value_destroy(ENTRY_VALUE* entry);

#endif /* CONSOLE_ARGS_H */


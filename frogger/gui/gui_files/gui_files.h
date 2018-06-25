/* GUI_FILES
 * Este modulo permite interpretar archivos de configuracion
 * para los diferentes modulos de GUI
 */

#ifndef GUI_FILES_H
#define GUI_FILES_H

/* Librerias utilizadas */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../gui_animation/gui_animation.h"

#include "../gui_types.h"

/**************/
/* Constantes */
/**************/

/* Maximo largo de string */
#define MAX_LINE        256

#define SETTING_END     "END\n"

/******************/
/* Tipos de datos */
/******************/

typedef struct{
    uint32_t integer;
    char* string;
    bool boolean;
} VALUES;

typedef struct{
    char* key;
    VALUES value;
    bool init;
} SET;

typedef struct{
    char* name;
    SET* sets;
    uint32_t length;
    bool init;
} SECTION;

typedef struct{
    SECTION* sections;
    uint32_t length;
} SETTING;

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* gui_files_get_section_length 
 * Devuelve la cantidad de parametrosd entro de una seccion
 *
 * setting: Instancia setting
 * section: Nombre de la seccion
 */
LENGTH gui_files_get_section_length(SETTING* setting, char* section);

/* gui_files_get_bool
 * Devuelve el parametro booleano de la configuracion
 *
 * setting: Objeto de configuraciones
 * section: Seccion de configuracion
 * key: Clave del parametro
 * value: Donde se guarda
 */
bool gui_files_get_bool(SETTING* setting, char* section, char* key, bool* value);

/* gui_files_get_int
 * Devuelve el parametro int de la configuracion
 *
 * setting: Objeto de configuraciones
 * section: Seccion de configuracion
 * key: Clave del parametro
 * value: Donde se guarda
 */
bool gui_files_get_int(SETTING* setting, char* section, char* key, uint32_t* value);

/* gui_files_get_string
 * Devuelve el parametro string de la configuracion
 *
 * setting: Objeto de configuraciones
 * section: Seccion de configuracion
 * key: Clave del parametro
 */
char* gui_files_get_string(SETTING* setting, char* section, char* key);

/* gui_files_load_setting
 * Carga un archivo de configuracion
 * en un objeto
 *
 * filename: Nombre del archivo
 */
SETTING* gui_files_load_setting(char* filename);

/* gui_files_destroy_setting
 * Destruye o libera memoria 
 *
 * setting: Instancia
 */
void gui_files_destroy_setting(SETTING* setting);

#endif /* GUI_FILES_H */


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

/**************/
/* Constantes */
/**************/

/* Maximo largo de string */
#define MAX_STRING  100
#define MAX_LINE    200

/* Archivo objeto secciones y claves */
#define OBJFILE_ATTRIBUTES   "attributes"
#define OBJFILE_ORIENTATION  "orientation"
#define OBJFILE_FRAMES       "frames"
#define OBJFILE_TIMEDELTA    "timedelta"
#define OBJFILE_SPACEDELTA   "spacedelta"
#define OBJFILE_QUANTITY     "quantity"

/* Valores de orientacion */
#define OBJFILE_ORIENTATION_HORIZONTAL_LEFT     "HORIZONTAL_LEFT"
#define OBJFILE_ORIENTATION_HORIZONTAL_RIGHT    "HORIZONTAL_RIGHT"
#define OBJFILE_ORIENTATION_VERTICAL_UP         "VERTICAL_UP"
#define OBJFILE_ORIENTATION_VERTICAL_DOWN       "VERTICAL_DOWN"

/* Estados lectura de archivo objeto */
#define OBJFILE_STATE_ATTRIBUTE             0
#define OBJFILE_STATE_TIMEDELTA             1
#define OBJFILE_STATE_SPACEDELTA            2
#define OBJFILE_STATE_ORIENTATION           3
#define OBJFILE_STATE_QUANTITY              4
#define OBJFILE_STATE_FRAME_SECTION         5
#define OBJFILE_STATE_FRAMES                6

/******************/
/* Tipos de datos */
/******************/

typedef struct{
    /* Clave id del tipo de config */
    char key[MAX_STRING];
    /* Valor de la config */
    char value[MAX_STRING];
} SETTING;

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* gui_files_read_objfile 
 * Lee un object file y lo carga en una instancia, devuelve true
 * si el archivo pudo ser leido con exito
 *
 * objFile: Nombre del archivo objeto
 * object: Instancia del objeto
 */
bool gui_files_read_objfile(char* objFile, ANIMATED_OBJECT* object);

#endif /* GUI_FILES_H */


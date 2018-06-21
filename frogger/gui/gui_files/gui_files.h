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

/**************/
/* Constantes */
/**************/

/* Maximo largo de string */
#define MAX_STRING  50

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

#endif /* GUI_FILES_H */


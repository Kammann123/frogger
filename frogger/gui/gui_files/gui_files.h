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

/******************/
/* Tipos de datos */
/******************/

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* is_section
 * Devuelve true si el string tiene el formato de una seccion
 * del archivo de configuracion: '[seccion]'
 *
 * str: String a evaluar 
 */
bool is_section(char* str);

#endif /* GUI_FILES_H */


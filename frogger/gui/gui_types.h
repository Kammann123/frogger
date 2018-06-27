/* GUI_TYPES
 * Contiene los tipos de datos genericos utilizados
 * por igual en todas las interfaces con el usuario
 */

#ifndef GUI_TYPES_H
#define GUI_TYPES_H

/* Librerias utilizadas */
#include <stdint.h>

#define TESTING

/******************/
/* Tipos de datos */
/******************/

typedef struct{
    int32_t x;
    int32_t y;
} POSITION;

typedef uint32_t LENGTH;

/**************/
/* Prototipos */
/**************/

/* map_position
 * Crea un objeto posicion
 *
 * x: Posicion x
 * y: Posicion y
 */
POSITION map_position(int32_t x, int32_t y);

/* testing_msg
 * Muestra un mensaje en la consola para informar durante
 * el proceso de pruebas
 */
void testing_msg(char *str);

#endif /* GUI_TYPES_H */

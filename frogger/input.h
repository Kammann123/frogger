#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <stdint.h>

/**************/
/* Constantes */
/**************/

/* Valores del estado del evento */
#define MOVE_LEFT   0
#define MOVE_RIGHT  1
#define MOVE_UP     2
#define MOVE_DOWN   3
#define ENTER       4
#define NO_EVENT    5

/**********************/
/* Funciones publicas */
/**********************/

/* input_init 
 * Inicializa el hardware o software necesario para
 * tomar las entradas de las plataformas configuradas, 
 * asi como el thread
 */
bool input_init(void);

/* input_close 
 * Cierra los accesos de las entradas
 */
void input_close(void);

/* get_input_status
 * Devuelve el estado de la entrada del usuario
 */
uint16_t get_input_status(void);

#endif /* INPUT_H */


/* GUI_ANIMATION
 * Este modulo permite manejar animaciones para las interfaces
 * graficas que correspondan
 */

#ifndef GUI_ANIMATION_H
#define GUI_ANIMATION_H

/* Librerias utilizadas */
#include <stdint.h>
#include <stdbool.h>

#include "../gui_types.h"

/**************/
/* Constantes */
/**************/

/* Estados de la animacion */
#define GUI_ANIMATION_STATE_STATIC  0
#define GUI_ANIMATION_STATE_LOOP    1
#define GUI_ANIMATION_STATE_MOVE    2

/******************/
/* Tipos de datos */
/******************/

/* FRAME: Contiene un filename con el frame, interpretable segun
 * en que interfaz se ejecute el modulo
 */
typedef char* FRAME;

typedef struct{
    /* Posiciones del objeto */
    POSITION currentPos;
    POSITION finalPos;
    
    /* Estado de la animacion */
    uint16_t status;
    
    /* Indice frame actual */
    uint16_t frameIndex;
    
    /* Configuracion de la animacion */
    FRAME*  frames;
    uint32_t timeDelta;
    uint32_t spaceDelta;
} ANIMATED_OBJECT;

typedef struct{
    /* Lista de objetos animados ligados */
    ANIMATED_OBJECT** objects;
    
    /* Cantidad de objetos vinculados */
    uint32_t length;
    
    /* Habilitadores del motor */
    bool shutdown;
} ANIMATION_ENGINE;

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* gui_animation_create_engine
 * Instancia un motor de animaciones para manejar
 * objetos animados
 */
ANIMATION_ENGINE* gui_animation_create_engine(void);

/* gui_animation_create_object
 * Instancia un objeto animado, precargando su configuracion
 * dada en el archivo
 *
 * objFile: Nombre del archivo de la animacion
 * x: Posicion inicial en X
 * y: Posicion inicial en Y
 */
ANIMATED_OBJECT* gui_animation_create_object(char* objFile, int32_t x, int32_t y);

#endif /* GUI_ANIMATION_H */


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

/* Orientaciones de la animacion */
#define GUI_ANIMATION_HORIZONTAL_LEFT   0
#define GUI_ANIMATION_HORIZONTAL_RIGHT  1
#define GUI_ANIMATION_VERTICAL_UP       2
#define GUI_ANIMATION_VERTICAL_DOWN     3
#define GUI_ANIMATION_ORIENTATION_NULL  4

#define NUMBER_OF_ORIENTATIONS          4

/******************/
/* Tipos de datos */
/******************/

typedef char* FRAME;

typedef struct{
    /* Archivos de cada frame */
    FRAME* frames;
    
    /* Cantidad de frames de la animacion */
    uint16_t framesQty;
    
    /* Orientacion de los frames */
    uint16_t orientation;
    
    /* Parametros de desplazamiento */
    uint32_t timeDelta;
    uint32_t spaceDelta;
} ANIMATION;

typedef struct{
    /* Posiciones del objeto */
    POSITION currentPos;
    POSITION finalPos;
    
    /* Estado de la animacion */
    uint16_t status;
    uint16_t orientation;
    
    /* Indice frame actual */
    uint16_t frameIndex;
    
    /* Configuracion de la animacion */
    ANIMATION animations[NUMBER_OF_ORIENTATIONS];
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
 * x: Posicion inicial en X
 * y: Posicion inicial en Y
 * orientation: Orientacion inicial del objeto 
 */
ANIMATED_OBJECT* gui_animation_create_object(int32_t x, int32_t y, uint16_t orientation);

/* gui_animation_create_framelist
 * Asigna memoria para crear una lista de frames
 *
 * frameQty: Cantidad de frames
 */
FRAME* gui_animation_create_framelist(uint16_t frameQty);

/* gui_animation_destroy_framelist
 * Libera la memoria usada por la framelist
 *
 * framelist: Lista de frames
 * framesQty: Cantidad de frames
 */
void gui_animation_destroy_framelist(FRAME* framelist, uint16_t framesQty);

/* gui_animation_create_frame 
 * Asigna memoria y guarda el contenido del frame
 * en dicho bloque
 * 
 * str: Filename del frame
 */
FRAME gui_animation_create_frame(char* str);

/* gui_animation_destroy_frame
 * Destruye, libera la memoria usada por un frame
 *
 * frame: Frame a liberar
 */
void gui_animation_destroy_frame(FRAME frame);

#endif /* GUI_ANIMATION_H */


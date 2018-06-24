/* GUI_ANIMATION
 * Este modulo permite manejar animaciones para las interfaces
 * graficas que correspondan
 */

#ifndef GUI_ANIMATION_H
#define GUI_ANIMATION_H

/* Librerias utilizadas */
#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

#include "../gui_types.h"

/**************/
/* Constantes */
/**************/

/* Configuracion */
#define OBJFILE_ATTRIBUTES  "attributes"
#define OBJFILE_FRAMES      "frames"

#define OBJFILE_TIMEDELTA   "timedelta"
#define OBJFILE_SPACEDELTA  "spacedelta"
#define OBJFILE_ORIENTATION "orientation"
#define OBJFILE_QUANTITY    "quantity"

#define OBJFILE_VERTICAL_UP         "VERTICAL_UP"
#define OBJFILE_VERTICAL_DOWN       "VERTICAL_DOWN"
#define OBJFILE_HORIZONTAL_LEFT     "HORIZONTAL_LEFT"
#define OBJFILE_HORIZONTAL_RIGHT    "HORIZONTAL_RIGHT"

/* Estados de la animacion */
#define GUI_ANIMATION_STATE_STATIC      0
#define GUI_ANIMATION_STATE_LOOP        1
#define GUI_ANIMATION_STATE_MOVE        2
#define GUI_ANIMATION_STATE_STATIC_MOVE 3

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
    POSITION iCorner;
    POSITION fCorner;
} REGION;

typedef struct{
    uint32_t timeDelta;
    uint32_t spaceDelta;
} SPEED;

typedef struct{
    /* Archivos de cada frame */
    FRAME* frames;
    
    /* Cantidad de frames de la animacion */
    uint32_t framesQty;
    
    /* Orientacion de los frames */
    uint32_t orientation;
    
    /* Parametros de desplazamiento */
    SPEED speed;
    
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
    
    /* Contador de tiempo */
    uint32_t timeCounter;
    
    /* Configuracion de la animacion */
    ANIMATION animations[NUMBER_OF_ORIENTATIONS];

    /* Mutex */
    pthread_mutex_t objectMutex;
} ANIMATED_OBJECT;

typedef ANIMATED_OBJECT* OBJECT_POINTER;

typedef struct{
    /* Lista de objetos animados ligados */
    OBJECT_POINTER* objects;
    
    /* Cantidad de objetos vinculados */
    uint32_t length;
    
    /* Habilitadores del motor */
    bool shutdown;
    bool pause;
    
    /* Thread */
    pthread_t engineThread;
} ANIMATION_ENGINE;

/***********************************/
/* Prototipo de funciones publicas */
/***********************************/

/* map_region
 * Crea un objeto region
 *
 * iCorner: Esquina inicial
 * fCorner: Esquina final
 */
REGION map_region(POSITION iCorner, POSITION fCorner);

/* map_speed
 * Crea un objeto velocidad
 *
 * timeDelta: Tiempo
 * spaceDelta: Distancia
 */
SPEED map_speed(uint32_t timeDelta, uint32_t spaceDelta);

/* gui_animation_load_objfile
 * Carga un archivo con la definicion de la animacion
 * del objeto en la instancia
 *
 * objFile: Nombre del archivo
 * object: Instancia 
 */
bool gui_animation_load_objfile(char* objFile, ANIMATED_OBJECT* object);

/* gui_animation_in_region 
 * Devuelve true si el objeto se encuentra en la region
 * especificada
 * 
 * object: Objeto
 * region: Region
 */
bool gui_animation_in_region(ANIMATED_OBJECT* object, REGION region);

/* gui_animation_pause_engine
 * Pausa el motor de animaciones
 *
 * engine: Motor de animaciones
 */
void gui_animation_pause_engine(ANIMATION_ENGINE* engine);

/* gui_animation_continue_engine
 * Reanuda el motor de animaciones
 *
 * engine: Motor de animaciones 
 */
void gui_animation_continue_engine(ANIMATION_ENGINE* engine);

/* gui_animation_get_frame
 * Devuelve el frame actual de un objeto
 *
 * object: Objeto 
 */
FRAME gui_animation_get_frame(ANIMATED_OBJECT* object);

/* gui_animation_start_static_movement
 * Activa la animacion de movimiento con animacion estatica
 *
 * object: Objeto
 * x: Posicion final de X
 * y: Posicion final de Y
 */
void gui_animation_start_static_movement(ANIMATED_OBJECT* object, int32_t x, int32_t y);

/* gui_animation_start_movement
 * Activa la animacion de movimiento hacia una posicion final
 *
 * object: Objeto animado
 * orientation: Orientacion de la animacion
 * x: Posicion final X
 * y: Posicion final Y
 */
void gui_animation_start_movement(ANIMATED_OBJECT* object, uint16_t orientation, int32_t x, int32_t y);

/* gui_animation_stop_loop
 * Para la animacion del objeto 
 *
 * object: Objeto
 */
void gui_animation_stop_loop(ANIMATED_OBJECT* object);

/* gui_animation_start_loop 
 * Activa el funcionamiento de la animacion en modo bucle del objeto
 * 
 * object: Objeto
 * orientation: Orientacion de la animacion
 */
void gui_animation_start_loop(ANIMATED_OBJECT* object, uint16_t orientation);

/* gui_animation_start_engine 
 * Inicia el funcionamiento del motor de animaciones
 * 
 * engine: Motor de animaciones 
 */
void gui_animation_start_engine(ANIMATION_ENGINE* engine);

/* gui_animation_attach_object
 * Agrega un objeto a la lista de objetos del motor
 * de animaciones para controlarlos 
 *
 * engine: Motor de animaciones 
 * object: Objeto
 */
bool gui_animation_attach_object(ANIMATION_ENGINE* engine, ANIMATED_OBJECT* object);

/* gui_animation_destroy_engine
 * Libera la memoria ocupada por el engine
 *
 * engine: Motor de animaciones a liberar
 */
void gui_animation_destroy_engine(ANIMATION_ENGINE* engine);

/* gui_animation_create_engine
 * Instancia un motor de animaciones para manejar
 * objetos animados
 */
ANIMATION_ENGINE* gui_animation_create_engine(void);

/* gui_animation_destroy_object 
 * Libera objeto creado
 *
 * object: Objeto a destruir*/
void gui_animation_destroy_object(ANIMATED_OBJECT* object);

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


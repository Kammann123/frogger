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

/* Max values */
#define ANIMATION_ID_MAX_LENGTH     100

/* Formato animation file */
#define ANIMATION_ATTRIBUTES        "attributes"
#define ANIMATION_FRAMES            "frames"

#define ANIMATION_ATT_ID            "id"
#define ANIMATION_ATT_ORIENTATION   "orientation"

#define ANIMATION_HORIZONTAL_LEFT   "HORIZONTAL_LEFT"
#define ANIMATION_HORIZONTAL_RIGHT  "HORIZONTAL_RIGHT"
#define ANIMATION_VERTICAL_UP       "VERTICAL_UP"
#define ANIMATION_VERTICAL_DOWN     "VERTICAL_DOWN"
#define ANIMATION_ORIENTATION_NULL  "ORIENTATION_NULL"

/* Formato object file  */
#define OBJFILE_ATTRIBUTES  "attributes"
#define OBJFILE_ANIMATIONS  "animations"

#define OBJFILE_ATT_WIDTH       "width"
#define OBJFILE_ATT_HEIGHT      "height"
#define OBJFILE_ATT_TIME        "timedelta"
#define OBJFILE_ATT_SPACE       "spacedelta"

/* Estados de la animacion */
typedef enum {
    GUI_ANIMATION_STATE_STATIC,
    GUI_ANIMATION_STATE_LOOP,
    GUI_ANIMATION_STATE_MOVE,
    GUI_ANIMATION_STATE_STATIC_MOVE,
    GUI_ANIMATION_STATE_ONE_TIME
} GUI_ANIMATION_STATUS;

/* Orientaciones */
typedef enum {
    GUI_HORIZONTAL_LEFT,
    GUI_HORIZONTAL_RIGHT,
    GUI_VERTICAL_UP,
    GUI_VERTICAL_DOWN,
    GUI_ORIENTATION_NULL
} GUI_ANIMATION_ORIENTATION;

/******************/
/* Tipos de datos */
/******************/

typedef char* ANIMATION_ID;

typedef struct{
    /* Nombre del archivo */
    char* file;
    
    /* Inicializacion del objeto */
    bool init;
} FRAME;

typedef struct{
    POSITION iCorner;
    POSITION fCorner;
} REGION;

typedef struct{
    uint32_t timeDelta;
    uint32_t spaceDelta;
} SPEED;

typedef struct{
    /* Identificador de animacion */
    ANIMATION_ID id;
    
    /* Frames */
    FRAME* frames;
    LENGTH framesQty;
    
    /* Orientacion */
    GUI_ANIMATION_ORIENTATION orientation;
    
    /* Inicializacion del objeto */
    bool init;    
} ANIMATION;

typedef struct{
    /* Posicion del objeto */
    POSITION pos;
    
    /* Tamaño del objeto */
    uint32_t width;
    uint32_t height;
    
    /* Velocidad */
    SPEED speed;
    
    /* Id de animacion en proceso */
    ANIMATION_ID currentAnimation;
    
    /* Estado de la animacion */
    GUI_ANIMATION_STATUS status;
    uint16_t frameIndex;
    int32_t distance;
    uint32_t timeCounter;
    
    /* Animaciones */
    ANIMATION* animations;
    LENGTH animQty;

    /* Object file */
    char* file;
    
    /* Mutex */
    pthread_mutex_t objectMutex;
    
    /* Inicializacion del objeto */
    bool init;
} ANIMATED_OBJECT;

typedef ANIMATED_OBJECT* OBJECT_POINTER;

typedef struct{
    /* Lista de objetos animados ligados */
    OBJECT_POINTER* objects;
    
    /* Cantidad de objetos vinculados */
    LENGTH length;
    
    /* Habilitadores del motor */
    bool shutdown;
    bool pause;
    
    /* Thread */
    pthread_t engineThread;
    
    /* Mutex */
    pthread_mutex_t engineMutex;
    
    /* Inicializacion del objeto */
    bool init;
} ANIMATION_ENGINE;

/****************************/
/* ANIMATED_OBJECT handlers */
/****************************/

/* gui_animation_reload_object 
 * Carga el archivo objeto devuelta
 * 
 * object: Objeto
 */
bool gui_animation_reload_object(ANIMATED_OBJECT* object);

/* gui_animation_get_frame_length 
 * Devuelve la cantidad de frames de la animacion 
 *
 * object: Objeto
 * id: Animacion
 */
LENGTH gui_animation_get_frame_length(ANIMATED_OBJECT* object, ANIMATION_ID id);

/* gui_animation_get_orientation 
 * Devuelve la orientacion de la animacion que esta usando
 *
 * object: Objeto
 */
GUI_ANIMATION_ORIENTATION gui_animation_get_orientation(ANIMATED_OBJECT* object);

/* gui_animation_seek_animation 
 * Busco la animacion con dicha orientacion
 *
 * object: Objeto
 * or: Orientacion
 */
ANIMATION_ID gui_animation_seek_animation(ANIMATED_OBJECT* object, GUI_ANIMATION_ORIENTATION or);

/* gui_animation_set_animation 
 * Configura una animacion a la fuerza
 *
 * object: Objeto
 * id: Animacion
 */
void gui_animation_set_animation(ANIMATED_OBJECT* object, ANIMATION_ID id);

/* gui_animation_is_animation 
 * Devuelve si es que esa es la animacion actual del objeto
 *
 * object: Objeto
 * id: Animacion
 */
bool gui_animation_is_animation(ANIMATED_OBJECT* object, ANIMATION_ID id);

/* gui_animation_get_frame
 * Devuelve el frame actual de un objeto
 *
 * object: Objeto 
 */
char* gui_animation_get_frame(ANIMATED_OBJECT* object);

/* gui_animation_destroy_object 
 * Destruye la instancia
 *
 * object: Instancia */
void gui_animation_destroy_object(OBJECT_POINTER object);

/* gui_animation_load_object 
 * Carga en memoria un objeto
 *
 * filename: Nombre del archivo del objeto
 * pos: Posicion inicial
 * id: Id animacion inicial 
 */
OBJECT_POINTER gui_animation_load_object(char* filename, POSITION pos, ANIMATION_ID id);

/****************************/
/* ANIMATION_ENGINE handler */
/****************************/

/* gui_animation_start_one_time
 * Inicia animacion bluce en posicion fija una vez
 * 
 * object: Objeto
 * id: Animacion 
 */
void gui_animation_start_one_time(ANIMATED_OBJECT* object, ANIMATION_ID id);

/* gui_animation_start_static_movement
 * Activa la animacion de movimiento con animacion estatica
 *
 * object: Objeto
 * distance: Distanica a recorrer
 */
void gui_animation_start_static_movement(ANIMATED_OBJECT* object, int32_t distance);

/* gui_animation_start_movement
 * Activa la animacion de movimiento hacia una posicion final
 *
 * object: Objeto animado
 * id: Id de animacion 
 * distance: Distancia a recorrer
 */
void gui_animation_start_movement(ANIMATED_OBJECT* object, ANIMATION_ID id, int32_t distance);

/* gui_animation_stop_movement 
 * Frena un movimiento
 *
 * object: Objeto a frenar
 */
void gui_animation_stop_movement(ANIMATED_OBJECT* object);

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
 * id: Id de animacion 
 */
void gui_animation_start_loop(ANIMATED_OBJECT* object, ANIMATION_ID id);

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
bool gui_animation_attach_engine(ANIMATION_ENGINE* engine, ANIMATED_OBJECT* object);

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

/* gui_animation_pause_engine
 * Pausa el motor de animaciones
 *
 * engine: Motor de animaciones
 */
void gui_animation_pause_engine(ANIMATION_ENGINE* engine);

/**********************/
/* Funciones publicas */
/**********************/

/* gui_animation_orientation_conv 
 * Convierte el dato en su valor
 *
 * str: String
 */
GUI_ANIMATION_ORIENTATION gui_animation_orientation_conv(char* str);

/* gui_animation_collision 
 * Detecta la colision de dos objetos
 * tomando como division el step
 *
 * objA: Objeto uno
 * objB: Objeto dos
 */
bool gui_animation_collision(ANIMATED_OBJECT* objA, ANIMATED_OBJECT* objB);

/* gui_animation_region_collision
 * Se fija si hubo colision de dos regiones
 * 
 * a: Region a
 * b: Region b 
 */
bool gui_animation_region_collision(REGION a, REGION b);

/* gui_animation_map_collision 
 * Se fija si hubo colision de una region y un objeto
 *
 * a: Region a
 * obj: Objeto
 */
bool gui_animation_map_collision(REGION a, ANIMATED_OBJECT* obj);

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

/* gui_animation_in_region 
 * Devuelve true si el objeto se encuentra en la region
 * especificada
 * 
 * position: Posicion del objeto
 * region: Region
 */
bool gui_animation_in_region(POSITION position, REGION region);

#endif /* GUI_ANIMATION_H */


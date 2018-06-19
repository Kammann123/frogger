#ifndef TIMER_H
#define TIMER_H

/************************/
/* Estructuras de datos */
/************************/

typedef struct{
    uint16_t id;
    uint32_t timerCounter;
    uint32_t timerMax;
    void* (*callback)(void*);
    void* args;
} TIMER_EVENT;

/**********************/
/* Funciones publicas */
/**********************/

#endif /* TIMER_H */


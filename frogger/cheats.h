

#ifndef CHEATS_H
#define CHEATS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Parametros */
#define MAX_LARGE_FOR_CHEATS 10
#define NOTHING_VALUE       -1      //COMO MOVE_LEFT TIENE VALOR 0, SE GENERARIAN ERRORES SI COMPARTEN EL VALOR.

/************************/
/* Estructuras de datos */
/************************/
typedef struct cheatT{
    uint8_t cheatLarge;
    uint32_t cheatArray[MAX_LARGE_FOR_CHEATS];
    void * action (void);
    struct cheatT * p2Next;
} cheat_t;

/*
 * Como podria usarse:
 *     cheat_t cheat1 = {4, {MOVE_RIGHT, MOVE_UP, MOVE_LEFT, MOVE_DOWN}, first_cheat_action, NULL};       //DECLARO UN CHEAT
 *     cheat_t cheat2 = {3, {MOVE_LEFT, MOVE_UP, MOVE_DOWN}, second_cheat_action, NULL};                  //DECLARO OTRO CHEAT
 *     cheat1.p2next = &cheat2;                                                                           //REFERENCIA AL PROXIMO CHEAT
 *     int32_t inputArray [MAX_LARGE_FOR_CHEATS];                                                         //DECLARO ARRAY DONDE SE GUARDARA LA SECUENCIA INGRESADA
 *     empty_array (inputArray, NOTHING_VALUE);                                                           //INICIO EL ARRAY EN VALORES NULOS
 *     while (state = CHEAT_CODE)                   
 *          scan_cheat (&inputArray, &cheat1);                                                            //BUSCO SI SE INGRESAN CHEATS
 */

/*********************************/
/* Prototipos funciones privadas */
/*********************************/


/*  get_array_large
 * Analiza cuantos valores distintos al nulo se encuentran en el array.
 * array:   Array a analizar.
 * noValue: Valor de nulidad (valor en el que se encuentran todos los elementos del array que no fueron ocupados).
 */
static uint8_t get_array_large (int32_t * array, int32_t noValue);


/*  add_to_array
 * 
 *  Agrega un elemento al array en la posicion que se desee.
 * 
 */
static void add_to_array (int32_t * array, uint8_t inputValue, uint8_t position);


/*  cmp_partial_array
 * 
 *  Analiza si el array coincide parcialmente con el array de alguna de las estructuras de cheat_t.
 *  Analiza parcialmente porque devuelve verdadero aunque el array de la estructura sea mas grande.
 *  Ejemplo:
 *      inArray = {UP, DOWN, LEFT};
 *      firstCheatArray = {UP, DOWN, LEFT, RIGHT};
 *      
 *      Devolveria verdadero. 
 *
 *  inArray:        Array a analizar
 *  firstCheat:     Puntero a la primera estructura de cheats.
 *  inArrayLarge:   Cantidad de valores distintos al nulo que se encuentran en el array.
 */
static bool cmp_partial_array (int32_t * inArray, cheat_t * firstCheat, uint8_t inArrayLarge);


/*  cmp_partial_array
 * 
 *  Analiza si el array coincide absolutamente con el array de alguna de las estructuras de cheat_t.
 *  Ejemplo:
 *      inArray = {UP, DOWN, LEFT};
 *      firstCheatArray = {UP, DOWN, LEFT, RIGHT};
 *      
 *      Devolveria falso. Pero con:
 * 
 *      firstCheatArray = {UP, DOWN, LEFT};
 *      Devolveria verdadero.
 *  
 *
 *  inArray:        Puntero al array a analizar
 *  firstCheat:     Puntero a la primera estructura de cheats.
 *  inArrayLarge:   Cantidad de valores distintos al nulo que se encuentran en el array.
 */
static cheat_t * cmp_absolute_array (int32_t * inArray, cheat_t * firstCheat, uint8_t inArrayLarge);

/*********************************/
/* Prototipos funciones publicas */
/*********************************/


/*  scan_cheat
 * 
 *  Analiza lo ingresado por el usuario y ejecuta acciones si las secuencias coinciden con algun cheatcode.
 *  
 *  inputArray: Puntero al array donde se almacena la secuencia ingresada por el usuario. Debe empezar con todos los elementos en su valor de nulidad.
 *  firstCheat: Punto a la primera estructura de los cheat codes.
 */
uint8_t scan_cheat (int32_t * inputArray, cheat_t * firstCheat);


/*  empty_array
 *   
 *  Pone todos los elementos del array en un valor de nulidad. Igualmente sirve para llenarlo con cualquier valor.
 *  array:      Puntero al array a llenar.
 *  emptyValue: Valor de nulidad
 */
void empty_array (int32_t * array, int32_t emptyValue);


#endif /* CHEATS_H */


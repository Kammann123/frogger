
#include "cheats.h"


/*  scan_cheat  */
uint8_t scan_cheat (int32_t * inputArray, cheat_t * firstCheat){

    cheat_t * cheatFound = NULL;

    uint8_t key;
    
    //FALTA MANEJAR EVENTOS (GOT_KEY SERIA EL BOOL DE SI SE INGRESO ALGO Y GET KET DE QUE SE INGRESO)
    if (got_key){
        
        key = get_key;
        add_to_array (inputArray, key, get_array_large (inputArray));

        if (cmp_partial_array (inputArray, firstCheat, get_array_large (inputArray))){

            if ( cheatFound = cmp_absolute_array(&inputArray, firstCheat, get_array_large (inputArray)) != NULL){
                *cheatFound->action();
            }

        } else{

            if (get_array_large (inputArray)>1){
                
                empty_array (inputArray, NOTHING_VALUE);
                inputArray[0]= key;
                
                if (cmp_partial_array (inputArray, firstCheat, get_array_large (inputArray))){

                    if ( cheatFound = cmp_absolute_array(inputArray, firstCheat, get_array_large (inputArray)) != NULL){

                        *cheatFound->action();
                    }
                } else{

                    empty_array (inputArray, NOTHING_VALUE);
                }  
            } else{
                
                empty_array (inputArray, NOTHING_VALUE); 
            }
        }    
    }
    
    return 0;
   
}

/*  empty_array  */
void empty_array (int32_t * array, int32_t emptyValue){
    
    uint8_t i;
    for (i=0; i<MAX_LARGE_FOR_CHEATS; i++){
        array[i] = emptyValue;
    }
    
}

/*  get_array_large  */
static uint8_t get_array_large (int32_t * array, int32_t noValue){
    
    uint8_t large = 0;
    
    while (array[large] != noValue){
        large++;
    }
    
    return large;
    
}

/*  add_to_array  */
static void add_to_array (int32_t * array, uint8_t inputValue, uint8_t position){
    
    array[position] = inputValue;
   
}

/*  cmp_partial_array  */
static bool cmp_partial_array (int32_t * inArray, cheat_t * firstCheat, uint8_t inArrayLarge){
    
    bool found = false;
    uint8_t i;
    cheat_t * currentCheat = firstCheat;
    
    while (currentCheat->p2Next != NULL && found==false){
        
        if (currentCheat->cheatLarge >= inArrayLarge){
            
            while ((currentCheat->cheatLarge[i] == inArray[i]) && (i<inArrayLarge)){
                i++;
            }
            
            if (i == inArrayLarge){
                return true;
            } else {
                i=0;  
            }  
        }
    
    currentCheat = currentCheat->p2Next;       
        
    } 
    return false;  
}

/*  cmp_absolute_array  */
static cheat_t * cmp_absolute_array (int32_t * inArray, cheat_t * firstCheat, uint8_t inArrayLarge){
    bool found = false;
    uint8_t i;
    cheat_t * currentCheat = firstCheat;
    
    while (currentCheat->p2Next != NULL && found==false){
        
        if (currentCheat->cheatLarge == inArrayLarge){
            
            while ((currentCheat->cheatLarge[i] == inArray[i]) && (i<inArrayLarge)){
                i++;
            }
            
            if (i == inArrayLarge){
                return currentCheat;
            } else {
                i=0;  
            }  
        }
    
    currentCheat = currentCheat->p2Next;       
        
    } 
    return NULL;    
}

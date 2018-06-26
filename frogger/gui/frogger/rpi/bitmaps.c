/* BITMAPS
 */

#include "bitmaps.h"

/*******************/
/* BITMAP Handlers */
/*******************/

/* read_bitmap */
BITMAP* read_bitmap(char* filename){
    /* Creo instancia de bitmap */
    BITMAP* bitmap;
    
    /* Creo instancia de archivo */
    FILE* file;
    
    /* Reservo memoria para el bitmap handler */
    bitmap = malloc( sizeof(BITMAP) );
    if( bitmap == NULL ){
        return NULL;
    }
    
    /* Abro el archivo */
    file = fopen(filename, "r");
    if( file == NULL ){
        free(bitmap);
        return NULL;
    }
    
    /* Leo el file header del BMP */
    fread(&bitmap->fileHeader.signature, sizeof(WORD), 1, file);
    
    /* Compruebo formato */
    if( bitmap->fileHeader.signature != BMP_SIGNATURE ){
        free(bitmap);
        fclose(file);
        return NULL;
    }
    
    fread(&bitmap->fileHeader.size, sizeof(DWORD), 1, file);
    fread(&bitmap->fileHeader.reserved1, sizeof(WORD), 1, file);
    fread(&bitmap->fileHeader.reserved2, sizeof(WORD), 1, file);
    fread(&bitmap->fileHeader.offset, sizeof(DWORD), 1, file);
    
    /* Leo el header del BMP */
    fread(&bitmap->header.size, sizeof(DWORD), 1, file);
    fread(&bitmap->header.width, sizeof(DWORD), 1, file);
    fread(&bitmap->header.height, sizeof(DWORD), 1, file);
    fread(&bitmap->header.planes, sizeof(WORD), 1, file);
    fread(&bitmap->header.bitsPerPixel, sizeof(WORD), 1, file);
    fread(&bitmap->header.compression, sizeof(DWORD), 1, file);
    fread(&bitmap->header.imageSize, sizeof(DWORD), 1, file);
    fread(&bitmap->header.xResolution, sizeof(DWORD), 1, file);
    fread(&bitmap->header.yResolution, sizeof(DWORD), 1, file);
    fread(&bitmap->header.paletteSize, sizeof(DWORD), 1, file);
    fread(&bitmap->header.importantColorsQty, sizeof(DWORD), 1, file);
    
    /* Reservo memoria para el bloque de pixels */
    bitmap->pixels = malloc( bitmap->header.imageSize );
    if( bitmap->pixels == NULL ){
        free(bitmap);
        fclose(file);
        return NULL;
    }
    
    /* Muevo al bloque de pixels */
    fseek(file, bitmap->fileHeader.offset, SEEK_SET);
    
    /* Leo */
    fread(bitmap->pixels, sizeof(BYTE), bitmap->header.imageSize, file);
    
    /* Cierro el archivo */
    fclose(file);
    
    return bitmap;
            
}

/* destroy_bitmap */
void destroy_bitmap(BITMAP* bitmap){
    
    if( bitmap != NULL ){
        if( bitmap->pixels != NULL ){
            free( bitmap->pixels ); 
        }
        free( bitmap );
    }
}
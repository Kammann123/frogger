/* BITMAPS
 * Maneja la lectura de archivos de formato .BMP
 */

#ifndef BITMAPS_H
#define BITMAPS_H

/* Librerias utilizadas */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

/* Constantes */
#define BMP_SIGNATURE 0x4D42

/* Tipo de datos */

typedef char BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;

typedef struct{
    WORD signature;
    DWORD size;
    WORD reserved1;
    WORD reserved2;
    DWORD offset;
} BITMAP_FILEHEADER;

typedef struct{
    DWORD size;
    DWORD width;
    DWORD height;
    WORD planes;
    WORD bitsPerPixel;
    DWORD compression;
    DWORD imageSize;
    DWORD xResolution;
    DWORD yResolution;
    DWORD paletteSize;
    DWORD importantColorsQty;
} BITMAP_HEADER;

typedef DWORD COLOR;

typedef struct{
    BITMAP_FILEHEADER fileHeader;
    BITMAP_HEADER header;
    COLOR* palette;
    BYTE* pixels;
} BITMAP;

/*******************/
/* BITMAP Handlers */
/*******************/

/* read_bitmap 
 * Lee un bitmap y lo guarda en memoria 
 *
 * filename: Nombre del archivo 
 */
BITMAP* read_bitmap(char* filename);

/* destroy_bitmap
 * Libera la memoria usada por un bitmap
 *
 * bitmap: Bitmap cargado en memoria
 */
void destroy_bitmap(BITMAP* bitmap);

#endif /* BITMAPS_H */


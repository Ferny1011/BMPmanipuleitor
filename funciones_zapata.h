#ifndef FUNCIONES_ZAPATA_H
#define FUNCIONES_ZAPATA_H

#include "funciones_grupo.h"

typedef void(*printStruct)(const void*);
void _printMatInt(const void* elem);
void _printPixelRGB(const void* elem);
int leerImagen(const char *fileName, PixelRGB **pixels, BmpHeader *imgHeader);
void WriteImage(const char *fileName, PixelRGB *pixels, BmpHeader imgHeader);
void convertToGrayscalePixels(PixelRGB *pixels, int width, int height);
int leerHeader(BmpHeader* h, FILE* arch);
void testHeader(BmpHeader* h);

#endif // FUNCIONES_ZAPATA_H
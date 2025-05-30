#ifndef FUNCIONES_ZAPATA_H
#define FUNCIONES_ZAPATA_H

#include "funciones_grupo.h"

typedef void(*printStruct)(const void*);
int leerImagen(const char *fileName, PixelRGB **pixels, BmpHeader *imgHeader);
void WriteImage(const char *fileName, PixelRGB *pixels, BmpHeader imgHeader);
void convertirEscalaDeGrises(PixelRGB *pixels, const BmpHeader *imgHeader);
void cambioTonalidad(PixelRGB *imagenOriginal, const BmpHeader *imgHeader, float rojo, float verde, float azul);
void testHeader(BmpHeader* h);
void parse_argv(int argc, char* argv[], OpcionesImagen *opciones);

#endif // FUNCIONES_ZAPATA_H

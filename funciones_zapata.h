#ifndef FUNCIONES_ZAPATA_H
#define FUNCIONES_ZAPATA_H

#include "funciones_grupo.h"

#define NOMBRE_GRUPO "LUMEN"

typedef void(*printStruct)(const void*);

int guardarImagen(const char *nombreArch, TDA_ImagenBMP *imagen);
void convertirEscalaDeGrises(TDA_ImagenBMP *imagen);
void cambioTonalidad(PixelRGB *imagenOriginal, const BmpHeader *imgHeader, float rojo, float verde, float azul);
void cambioNegativo(PixelRGB *pixels, const BmpHeader *imgHeader);
void testHeader(BmpHeader* h);
void parse_argv(int argc, char* argv[], OpcionesImagen *opciones);
void mostrarAyuda();
void agregarOperacion(OpcionesImagen *opciones, TipoOperacion op, int valor);
bool requiereSegundaImagen(TipoOperacion operacion);
void generarNombreArchivo(const char* archivoOriginal, TipoOperacion operacion, char* nombreSalida);
const char* obtenerNombreOperacion(TipoOperacion operacion);
void cambioContraste(PixelRGB *pixels, const BmpHeader *imgHeader, int contraste);
int _adjustContrast(uint8_t pixel, float factor);
void espejarVertical(PixelRGB *pixels, const BmpHeader *imgHeader);
void espejarHorizontal(PixelRGB *pixels, const BmpHeader *imgHeader);
void rotarDerecha(PixelRGB *pixels, BmpHeader *imgHeader);



#endif // FUNCIONES_ZAPATA_H

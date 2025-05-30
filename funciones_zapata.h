#ifndef FUNCIONES_ZAPATA_H
#define FUNCIONES_ZAPATA_H

#include "funciones_grupo.h"

#define NOMBRE_GRUPO "LUMEN"

typedef void(*printStruct)(const void*);
int leerImagen(const char *fileName, PixelRGB **pixels, BmpHeader *imgHeader);
void WriteImage(const char *fileName, PixelRGB *pixels, BmpHeader imgHeader);
void convertirEscalaDeGrises(PixelRGB *pixels, const BmpHeader *imgHeader);
void cambioTonalidad(PixelRGB *imagenOriginal, const BmpHeader *imgHeader, float rojo, float verde, float azul);
void testHeader(BmpHeader* h);
void parse_argv(int argc, char* argv[], OpcionesImagen *opciones);
void mostrarAyuda();
void agregarOperacion(OpcionesImagen *opciones, TipoOperacion op, int valor);
bool requiereSegundaImagen(TipoOperacion operacion);
void generarNombreArchivo(const char* archivoOriginal, TipoOperacion operacion, char* nombreSalida);
const char* obtenerNombreOperacion(TipoOperacion operacion);
void cambioContraste(PixelRGB *pixels, const BmpHeader *imgHeader, int contraste);
uint8_t _adjustContrast(uint8_t pixel, float factor);



#endif // FUNCIONES_ZAPATA_H

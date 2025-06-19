#ifndef FUNCIONES_ZAPATA_H
#define FUNCIONES_ZAPATA_H

#include "funciones_grupo.h"

#define NOMBRE_GRUPO "LUMEN"
#define DERECHA 90
#define IZQUIERDA -90

typedef void(*printStruct)(const void*);

int guardarImagen(const char *nombreArch, TDA_ImagenBMP *imagen);
void convertirEscalaDeGrises(TDA_ImagenBMP *imagen);
void cambioTonalidad(TDA_ImagenBMP *imagen, float rojo, float verde, float azul);
void cambioNegativo(TDA_ImagenBMP *imagen);
void testHeader(BmpHeader* h);
void parse_argv(int argc, char* argv[], OpcionesImagen *opciones);
void mostrarAyuda();
void agregarOperacion(OpcionesImagen *opciones, TipoOperacion op, int valor);
bool requiereSegundaImagen(TipoOperacion operacion);
void generarNombreArchivo(const char* archivoOriginal, TipoOperacion operacion, char* nombreSalida);
const char* obtenerNombreOperacion(TipoOperacion operacion);
void cambioContraste(TDA_ImagenBMP *imagen, int contraste);
void imagenTrianguloRGB(TDA_ImagenBMP *imagen, float rojo, float verde, float azul);
void espejarVertical(TDA_ImagenBMP *imagen);
void espejarHorizontal(TDA_ImagenBMP *imagen);
void _rotarMatriz(TDA_ImagenBMP *imagen, int angulo);
void rotarDerecha(TDA_ImagenBMP *imagen);
void rotarIzquierda(TDA_ImagenBMP *imagen);



#endif // FUNCIONES_ZAPATA_H

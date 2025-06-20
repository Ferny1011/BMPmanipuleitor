#ifndef FUNCIONES_QUIROGA_H_INCLUDED
#define FUNCIONES_QUIROGA_H_INCLUDED

#include "funciones_grupo.h"
#include "funciones_zapata.h"
#include "funciones_lazaro.h"

void ejecutarOperaciones(OpcionesImagen *opciones);

TDA_ImagenBMP *concatenarImagenVertical (TDA_ImagenBMP *imagenSuperior, TDA_ImagenBMP *imagenInferior);

TDA_ImagenBMP *concatenarImagenHorizontal(TDA_ImagenBMP *imagenIzquierda, TDA_ImagenBMP *imagenDerecha);

void aplicarFiltrosEnCadena(TDA_ImagenBMP *imagen, OperacionImagen *operaciones, int cantidadOperaciones, TDA_ImagenBMP *imagen2);

#endif // FUNCIONES_QUIROGA_H_INCLUDED

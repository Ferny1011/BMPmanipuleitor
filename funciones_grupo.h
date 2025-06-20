#ifndef FUNCIONES_GRUPO_H
#define FUNCIONES_GRUPO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "lib/TDAImagenBMP.h"

typedef enum {
    OP_NONE = 0,
    OP_ACHICAR,
    OP_ROTAR_DERECHA,
    OP_ROTAR_IZQUIERDA,
    OP_RECORTAR,
    OP_ESPEJAR_HORIZONTAL,
    OP_ESPEJAR_VERTICAL,
    OP_CONCATENAR_HORIZONTAL,
    OP_CONCATENAR_VERTICAL,
    OP_ESCALA_GRISES,
    OP_TONALIDAD_AZUL,
    OP_TONALIDAD_ROJA,
    OP_TONALIDAD_VERDE,
    OP_AUMENTAR_CONTRASTE,
    OP_DISMINUIR_CONTRASTE,
    OP_NEGATIVO,
    OP_FUNCIONALIDAD_EXTRA,
    OP_TRIANGULO_RGB
}TipoOperacion;

typedef struct {
    TipoOperacion operacion;
    int valor;
}OperacionImagen;

typedef struct {
    OperacionImagen operaciones[16];
    int numOperaciones;
    char* imgFiles[2];
    int cantImg;
}OpcionesImagen;

#endif // FUNCIONES_GRUPO_H

#ifndef FUNCIONES_GRUPO_H
#define FUNCIONES_GRUPO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>

typedef struct __attribute__((packed)) {
    char firma[2];
    uint32_t tamArchivo;
    uint32_t reservado;
    uint32_t dataOffset;
    uint32_t headerSize;
    uint32_t anchura;
    uint32_t altura;
    uint16_t planos;
    uint16_t profundidad;
    uint32_t compresion;
    uint32_t tamImagen;
    uint32_t resH;
    uint32_t resV;
    uint32_t tamTablaColores;
    uint32_t contadorColores;
}BmpHeader;

typedef struct {
    uint8_t r, g, b;
}PixelRGB;

#endif // FUNCIONES_GRUPO_H
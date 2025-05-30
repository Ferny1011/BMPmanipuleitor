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

typedef struct{
    uint8_t b, g, r;
}PixelRGB;

typedef enum{
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
    OP_NEGATIVO
}TipoOperacion;

typedef struct{
    TipoOperacion operacion;
    int valor;
    bool activo;
}OperacionImagen;

typedef struct{
    OperacionImagen operaciones[16];
    int numOperaciones;
    char* imgFiles[2];
    int cantImg;
}OpcionesImagen;

#endif // FUNCIONES_GRUPO_H

#ifndef TDA_IMAGEN_BMP_H
#define TDA_IMAGEN_BMP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

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
    uint8_t b, g, r;
}PixelRGB;

typedef struct{
    PixelRGB** data;
    size_t tamElem, columnas, filas;
}TDA_Matriz;

typedef struct {
    BmpHeader cabecera;
    TDA_Matriz *matrizDePixeles;
    char *nombreArchivo;
}TDA_ImagenBMP;


TDA_ImagenBMP *crearImagenBMP(const char *nombreArchivo);
int crearMatPixeles(TDA_Matriz *mat, int columnas, int filas);
void liberarImagenBMP(TDA_ImagenBMP *imagen);
void freeMat(TDA_Matriz *mat);



#endif // TDA_IMAGEN_BMP_H
#ifndef TDA_IMAGEN_BMP_H
#define TDA_IMAGEN_BMP_H

#include "TDAPixel.h"

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
    BmpHeader cabecera;
    PixelRGB *matrizDePixeles;
    char *nombreArchivo;
}TDA_ImagenBMP;


int obtenerAlturaImagen(const TDA_ImagenBMP *imagen);
int obtenerAnchuraImagen(const TDA_ImagenBMP *imagen);
TDA_ImagenBMP* crearImagenBMP(const char *nombreArchivo);
void destruirImagenBMP(TDA_ImagenBMP **imagen);


#endif // TDA_IMAGEN_BMP_H
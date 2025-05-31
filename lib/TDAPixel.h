#ifndef TDA_PIXEL_H
#define TDA_PIXEL_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>


typedef struct {
    uint8_t b, g, r;
}PixelRGB;

/**
 * @brief Reserva un bloque contiguo de memoria del tamaño ancho * alto.
 * @param PixelRGB Puntero a la matriz de pixeles
 * @param ancho Ancho del bloque requerido
 * @param alto Alto del bloque requerido
 * @return Tamaño total reservado o 0 si no se pudo reservar el bloque.
 */
int reservarPixels(PixelRGB **pixels, int ancho, int alto);
void liberarPixels(PixelRGB **pixels);

#endif // TDA_PIXEL_H
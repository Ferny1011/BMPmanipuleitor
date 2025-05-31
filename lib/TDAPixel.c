#include "TDAPixel.h"



int reservarPixels(PixelRGB **pixels, int ancho, int alto) {
    if (ancho <= 0 || alto <= 0) {
        *pixels = NULL;
        return 0;
    }
    
    int tamTotal = ancho * alto;
    *pixels = (PixelRGB *)malloc(tamTotal * sizeof(PixelRGB));
    
    if (*pixels == NULL) {
        puts("Error al asignar memoria para los píxeles");
        return 0;
    }
    return tamTotal;
}

void liberarPixels(PixelRGB **pixels) {
    if (pixels && *pixels) {
        free(*pixels);
        *pixels = NULL;
    }
}
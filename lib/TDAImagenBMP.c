#include "TDAImagenBMP.h"


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

int createMat(TDA_Matriz* mat, size_t tamElem, int columnas, int filas){
    int row, j;
    mat->data = malloc(sizeof(void*) * filas);
    if ( !mat->data ) return 0;

    for ( row = 0; row < filas; row++ ){
        (mat->data)[row] = malloc(tamElem * columnas);
        if ( (mat->data)[row] == NULL ) {
            for ( j = 0; j < row; j++ )
                free( mat->data[j] );
            free(mat->data);
            return 0;
        }
    }
    mat->columnas = columnas;
    mat->filas = filas;
    mat->tamElem = tamElem;
    return 1;
}

TDA_ImagenBMP* crearImagenBMP(const char *nombreArchivo){
    TDA_Matriz* matImagen = NULL;
    TDA_ImagenBMP* imagen = NULL;
    if( createMat(matImagen, sizeof(PixelRGB), matImagen->columnas, matImagen->filas) ){
        puts("Error al reservar memoria para la matriz de pixeles");
        return NULL;
    }
    imagen->matrizDePixeles = matImagen;

    return imagen;
}

void liberarPixels(PixelRGB **pixels) {
    if (pixels && *pixels) {
        free(*pixels);
        *pixels = NULL;
    }
}

void freeMat(tMat* mat){
    int row;
    if( !mat || !mat->data ) return;
    for( row = 0; row < mat->filas; row++ )
        free( mat->data[row] );
    free( mat->data );
}

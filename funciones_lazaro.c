#include "funciones_lazaro.h"

TDA_ImagenBMP* recortar(TDA_ImagenBMP* original, int porcentaje) {
    if (!original || porcentaje <= 0 || porcentaje > 100) {
        printf("Error: Imagen nula o porcentaje invalido.\n");
        return NULL;
    }

    int nuevoAncho = (int)(original->cabecera.anchura * (porcentaje / 100.0));
    int nuevoAlto  = (int)(original->cabecera.altura  * (porcentaje / 100.0));

    if (nuevoAncho <= 0 || nuevoAlto <= 0) {
        printf("Error: Las nuevas dimensiones del recorte son inválidas.\n");
        return NULL;
    }


    TDA_ImagenBMP *recortada = (TDA_ImagenBMP *)malloc(sizeof(TDA_ImagenBMP));
    if (!recortada) {
        printf("Error: No se pudo reservar memoria para la imagen recortada.\n");
        return NULL;
    }

    recortada->cabecera = original->cabecera;
    recortada->cabecera.anchura = nuevoAncho;
    recortada->cabecera.altura = nuevoAlto;


    int padding = (4 - (nuevoAncho * sizeof(PixelRGB)) % 4) % 4;
    int tamImagen = (nuevoAncho * sizeof(PixelRGB) + padding) * nuevoAlto;

    recortada->cabecera.tamImagen = tamImagen;
    recortada->cabecera.tamArchivo = recortada->cabecera.dataOffset + tamImagen;


    recortada->matrizDePixeles = (TDA_Matriz *)malloc(sizeof(TDA_Matriz));
    if (!recortada->matrizDePixeles) {
        printf("Error: No se pudo reservar memoria para la matriz de píxeles.\n");
        free(recortada);
        return NULL;
    }

    if (!crearMatPixeles(recortada->matrizDePixeles, nuevoAncho, nuevoAlto)) {
        printf("Error al crear la matriz de pixeles para la imagen recortada.\n");
        free(recortada->matrizDePixeles);
        free(recortada);
        return NULL;
    }


    for (int i = 0; i < nuevoAlto; i++) {
        for (int j = 0; j < nuevoAncho; j++) {
            recortada->matrizDePixeles->data[i][j] = original->matrizDePixeles->data[i][j];
        }
    }

    return recortada;
}


TDA_ImagenBMP* achicar(TDA_ImagenBMP* original, int porcentaje) {

    if (!original || porcentaje <= 0 || porcentaje > 100) {
        printf("Error: Imagen nula o porcentaje inválido.\n");
        return NULL;
    }


    int anchoOrig = original->cabecera.anchura;
    int altoOrig  = original->cabecera.altura;


    int nuevoAncho = (int)(anchoOrig * (porcentaje / 100.0));
    int nuevoAlto  = (int)(altoOrig  * (porcentaje / 100.0));


    if (nuevoAncho <= 0 || nuevoAlto <= 0) {
        printf("Error: Dimensiones finales inválidas.\n");
        return NULL;
    }


    TDA_ImagenBMP *achicada = malloc(sizeof(TDA_ImagenBMP));
    if (!achicada) {
        printf("Error: No se pudo reservar memoria para la imagen achicada.\n");
        return NULL;
    }


    achicada->cabecera = original->cabecera;
    achicada->cabecera.anchura = nuevoAncho;
    achicada->cabecera.altura = nuevoAlto;


    int padding = (4 - (nuevoAncho * sizeof(PixelRGB)) % 4) % 4;


    int tamImagen = (nuevoAncho * sizeof(PixelRGB) + padding) * nuevoAlto;
    achicada->cabecera.tamImagen = tamImagen;
    achicada->cabecera.tamArchivo = achicada->cabecera.dataOffset + tamImagen;


    achicada->matrizDePixeles = malloc(sizeof(TDA_Matriz));
    if (!achicada->matrizDePixeles) {
        printf("Error: No se pudo reservar memoria para la matriz de píxeles.\n");
        free(achicada);
        return NULL;
    }


    if (!crearMatPixeles(achicada->matrizDePixeles, nuevoAncho, nuevoAlto)) {
        printf("Error al crear la matriz de píxeles achicada.\n");
        free(achicada->matrizDePixeles);
        free(achicada);
        return NULL;
    }


    float escalaX = (float)anchoOrig / nuevoAncho;
    float escalaY = (float)altoOrig  / nuevoAlto;


    for (int i = 0; i < nuevoAlto; i++) {
        for (int j = 0; j < nuevoAncho; j++) {

            int origI = (int)(i * escalaY);
            int origJ = (int)(j * escalaX);


            achicada->matrizDePixeles->data[i][j] = original->matrizDePixeles->data[origI][origJ];
        }
    }


    return achicada;
}


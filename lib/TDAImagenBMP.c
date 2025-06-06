#include "TDAImagenBMP.h"


int crearMatPixeles(TDA_Matriz* mat, int columnas, int filas){
    int filaAct, j;
    if(columnas <= 0 || filas <= 0) return 0;
    mat->data = malloc(sizeof(void*) * filas);
    if ( !mat->data ) return 0;
    mat->tamElem = sizeof(PixelRGB);
    for ( filaAct = 0; filaAct < filas; filaAct++ ){
        (mat->data)[filaAct] = malloc(mat->tamElem * columnas);
        //memset((mat->data)[filaAct], 120, mat->tamElem * columnas);
        if ( (mat->data)[filaAct] == NULL ) {
            for ( j = 0; j < filaAct; j++ )
                free( mat->data[j] );
            free(mat->data);
            return 0;
        }
    }
    mat->columnas = columnas;
    mat->filas = filas;

    return 1;
}

TDA_ImagenBMP* crearImagenBMP(const char *nombreArchivo){
    TDA_ImagenBMP* imagen = NULL;
    PixelRGB *filaSinRelleno = NULL;
    FILE *imgFile = NULL;
    int i, j, padding, tamfilaSinRelleno;

    imagen = (TDA_ImagenBMP*)malloc(sizeof(TDA_ImagenBMP));
    if ( !imagen ) {
        return NULL;
    }
    imgFile = fopen(nombreArchivo, "rb");
    if ( !imgFile ) {
        puts("Error al abrir el archivo de imagen");
        free(imagen);
        return NULL;
    }
    fread(&imagen->cabecera, sizeof(BmpHeader), 1, imgFile);
    //chequear la firma BM, cheaquear 24bits, prof = 1, compresion = 0
    if ( imagen->cabecera.firma[0] != 'B' || imagen->cabecera.firma[1] != 'M' ||
         imagen->cabecera.profundidad != 24 || imagen->cabecera.compresion != 0 ) {
        puts("El archivo no es un BMP valido");
        fclose(imgFile);
        free(imagen);
        return NULL;
    }
    // reservar memoria para la matriz de pixeles
    imagen->matrizDePixeles = (TDA_Matriz*)malloc(sizeof(TDA_Matriz));
    if ( !imagen->matrizDePixeles ) {
        puts("Error al reservar memoria para la matriz de pixeles");
        fclose(imgFile);
        free(imagen);
        return NULL;
    }
    // crear la matriz de pixeles
    if ( !crearMatPixeles(imagen->matrizDePixeles, imagen->cabecera.anchura, imagen->cabecera.altura) ) {
        puts("Error al crear la matriz de pixeles");
        fclose(imgFile);
        free(imagen);
        return NULL;
    }
    // calcular el padding de la imagen
    padding = (4 - (imagen->cabecera.anchura * sizeof(PixelRGB) % 4)) % 4;
    tamfilaSinRelleno = (imagen->cabecera.anchura * sizeof(PixelRGB));
    // reservar memoria para la fila con padding
    filaSinRelleno = (PixelRGB*)malloc(tamfilaSinRelleno);
    if ( !filaSinRelleno ) {
        puts("Error al reservar memoria para la fila con padding");
        fclose(imgFile);
        free(imagen);
        freeMat(imagen->matrizDePixeles);
        return NULL;
    }

    fseek(imgFile, imagen->cabecera.dataOffset, SEEK_SET);
    for (i = 0; i < imagen->cabecera.altura; i++) {
        fread(filaSinRelleno, sizeof(PixelRGB), imagen->cabecera.anchura, imgFile);
        // copiar los pixeles a la matriz de pixeles
        for (j = 0; j < imagen->cabecera.anchura; j++) {
            imagen->matrizDePixeles->data[i][j].b = filaSinRelleno[j].b;
            imagen->matrizDePixeles->data[i][j].g = filaSinRelleno[j].g;
            imagen->matrizDePixeles->data[i][j].r = filaSinRelleno[j].r;
        }
        fseek(imgFile, padding, SEEK_CUR);
    }
    free(filaSinRelleno);
    fclose(imgFile);
    return imagen;
}

int duplicarMatPixeles(TDA_ImagenBMP *original, TDA_ImagenBMP *copia) {
    if (!original || !copia || !original->matrizDePixeles || !copia->matrizDePixeles) {
        puts("Error: Imagen original o copia no inicializadas correctamente.");
        return 0;
    }

    copia->cabecera = original->cabecera;
    if (!crearMatPixeles(copia->matrizDePixeles, original->matrizDePixeles->columnas, original->matrizDePixeles->filas)) {
        puts("Error al crear la matriz de pixeles para la copia.");
        return 0;
    }

    for (int i = 0; i < original->matrizDePixeles->filas; i++) {
        memcpy(copia->matrizDePixeles->data[i], original->matrizDePixeles->data[i],
               original->matrizDePixeles->columnas * sizeof(PixelRGB));
    }

    return 1;
}

void liberarPixels(PixelRGB **pixels) {
    if (pixels && *pixels) {
        free(*pixels);
        *pixels = NULL;
    }
}

void freeMat(TDA_Matriz *mat){
    int filaAct;
    if( !mat || !mat->data ) return;
    for( filaAct = 0; filaAct < mat->filas; filaAct++ )
        free( mat->data[filaAct] );
    free( mat->data );
}

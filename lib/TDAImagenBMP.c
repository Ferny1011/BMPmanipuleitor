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
    FILE *imgFile = NULL;
    int i, padding;

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
    if ( imagen->cabecera.firma[0] != 'B' || imagen->cabecera.firma[1] != 'M' ||
         imagen->cabecera.profundidad != 24 || imagen->cabecera.compresion != 0 ) {
        puts("El archivo no es un BMP soportado (24 bits sin compresion)");
        fclose(imgFile);
        free(imagen);
        return NULL;
    }

    imagen->matrizDePixeles = (TDA_Matriz*)malloc(sizeof(TDA_Matriz));
    if ( !imagen->matrizDePixeles ) {
        puts("Error al reservar memoria para la matriz de pixeles");
        fclose(imgFile);
        free(imagen);
        return NULL;
    }

    if ( !crearMatPixeles(imagen->matrizDePixeles, imagen->cabecera.anchura, imagen->cabecera.altura) ) {
        puts("Error al crear la matriz de pixeles");
        fclose(imgFile);
        liberarImagenBMP(imagen);
        return NULL;
    }

    padding = (4 - (imagen->cabecera.anchura * sizeof(PixelRGB) % 4)) % 4;
    fseek(imgFile, imagen->cabecera.dataOffset, SEEK_SET);
    for (i = 0; i < imagen->cabecera.altura; i++) {
        fread(imagen->matrizDePixeles->data[i], sizeof(PixelRGB), imagen->cabecera.anchura, imgFile);
        fseek(imgFile, padding, SEEK_CUR);
    }

    fclose(imgFile);
    return imagen;
}

void liberarImagenBMP(TDA_ImagenBMP *imagen){
    if( !imagen ) return;
    if( imagen->matrizDePixeles ) {
        freeMat(imagen->matrizDePixeles);
        free(imagen->matrizDePixeles);
    }
    free(imagen);
}

void freeMat(TDA_Matriz *mat){
    int filaAct;
    if( !mat || !mat->data ) return;
    for( filaAct = 0; filaAct < mat->filas; filaAct++ )
        free( mat->data[filaAct] );
    free( mat->data );
}

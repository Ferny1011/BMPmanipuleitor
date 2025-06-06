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
    int i, padding, tamfilaSinRelleno;

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
        puts("El archivo no es un BMP soportado (24 bits sin compresion)");
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
        free(imagen->matrizDePixeles);
        return NULL;
    }
    // calcular el padding de la imagen
    padding = (4 - (imagen->cabecera.anchura * sizeof(PixelRGB) % 4)) % 4;
    tamfilaSinRelleno = (imagen->cabecera.anchura * sizeof(PixelRGB));
    // reservar memoria para la fila sin tener en cuenta padding
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
        fread(imagen->matrizDePixeles->data[i], sizeof(PixelRGB), imagen->cabecera.anchura, imgFile);
        fseek(imgFile, padding, SEEK_CUR);
    }
    free(filaSinRelleno);
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

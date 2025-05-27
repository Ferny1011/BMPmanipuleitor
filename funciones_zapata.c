#include "funciones_zapata.h"

void _printMatInt(const void* elem){
    printf("%2d|", *(int*)elem);
}

int createMat(Imagen* mat, size_t tamElem, int cols, int rows){
    int row, j;
    mat->data = malloc(sizeof(void*) * rows);
    if ( !mat->data ) return 0;

    for ( row = 0; row < rows; row++ ){
        (mat->data)[row] = malloc(tamElem * cols);
        if ( (mat->data)[row] == NULL ) {
            for ( j = 0; j < row; j++ )
                free( mat->data[j] );
            free(mat->data);
            return 0;
        }
    }
    mat->cols = cols;
    mat->rows = rows;
    mat->tamElem = tamElem;
    return 1;
}

int createMatBloque(Imagen* mat, size_t tamElem, int cols, int rows) {
    int i;
    mat->data = malloc(rows * sizeof(void*));
    if (!mat->data) return 0;

    void* bloque = malloc(rows * cols * tamElem);
    if (!bloque) {
        free(mat->data);
        return 0;
    }

    for (i = 0; i < rows; i++)
        mat->data[i] = (char*)bloque + i * cols * tamElem;
    mat->tamElem = tamElem;
    mat->cols = cols;
    mat->rows = rows;
    return 1;
}


void printMat(Imagen* mat, printStruct printM){
    int row, col;
    if (!mat || !mat->data) return;
    for( row = 0; row < mat->rows; row++ ){
        putchar('|');
        for( col = 0; col < mat->cols; col++ ){
            printM( (mat->data)[row] + (col * mat->tamElem) );
        }
        putchar('\n');
    }
}

int leerHeader(BmpHeader* h, FILE* arch){
    return fread(h, sizeof(BmpHeader), 1, arch);
}

void testHeader(BmpHeader *cabecera){
    printf("Cabecera Tam: %d\n", sizeof(BmpHeader));
    printf("Cabecera firma: %c%c\n", cabecera->firma[0], cabecera->firma[1]);
    printf("Alto: %d\n", cabecera->altura);
    printf("Ancho: %d\n", cabecera->anchura);
    printf("Profundidad: %d\n", cabecera->profundidad);
    printf("Compresion: %d\n", cabecera->compresion);
    printf("Tam Imagen: %d\n", cabecera->tamImagen);
}

int leerImagen(Imagen* img, FILE* archImg, BmpHeader* imgHeader){
    if(imgHeader->firma[0] != 'B' || imgHeader->firma[1] != 'M') {
        fprintf(stderr, "Archivo no es BMP\n");
        fclose(archImg);
        return 0;
    }
    if(imgHeader->profundidad != 24 || imgHeader->compresion != 0){
        fprintf(stderr, "Solo se soportan BMPs de 24 bits sin compresion\n");
        fclose(archImg);
        return 0;
    }

    size_t filaConPadding = ((imgHeader->anchura * 3 + 3) / 4) * 4;
    size_t totalBytes = filaConPadding * (imgHeader->altura > 0 ? imgHeader->altura : -imgHeader->altura);

    uint8_t* buffer = malloc(totalBytes);
    if (!buffer) {
        //freeMat(img);
        fclose(archImg);
        return 0;
    }

    fseek(archImg, imgHeader->dataOffset, SEEK_SET);
    fread(buffer, 1, imgHeader->tamImagen, archImg);

    for (int i = 0; i < (imgHeader->altura > 0 ? imgHeader->altura : -imgHeader->altura); i++) {
        int fila = imgHeader->altura > 0 ? imgHeader->altura - 1 - i : i;
        memcpy(img->data[i], buffer + fila * filaConPadding, imgHeader->anchura * sizeof(PixelRGB));
    }

    free(buffer);

    return 1;
}

int guardarImagen(Imagen* img, const char* ruta, BmpHeader* imgHeader) {
    FILE* f = fopen(ruta, "wb");
    if (!f) {
        perror("No se pudo abrir archivo para escritura");
        return 0;
    }
    fwrite(imgHeader, sizeof(BmpHeader), 1, f);
    int filaPadding = (4 - (img->cols * 3) % 4) % 4;
    uint8_t pad[3] = {0, 0, 0};
    for (int i = (int)img->rows - 1; i >= 0; --i) {
        fwrite(img->data[i], sizeof(PixelRGB), img->cols, f);
        fwrite(pad, 1, filaPadding, f);
    }

    fclose(f);
    return 1;
}



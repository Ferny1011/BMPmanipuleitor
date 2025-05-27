#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#pragma pack(push, 1)
typedef struct{
    char firma[2];
    uint32_t tamaArchivo;
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
#pragma pack(pop)

typedef struct{
    void** data;
    size_t tamElem, cols, rows;
}Imagen;

typedef struct{
    uint8_t b, g, r;
}PixelRGB;

typedef void(*printStruct)(const void*);
void _printMatInt(const void* elem);
void printMat(Imagen* mat, printStruct printM);
int createMat(Imagen* mat, size_t tamElem, int cols, int rows);
int createMatBloque(Imagen* mat, size_t tamElem, int cols, int rows);
void _printMatInt(const void* elem);
int leerImagen(Imagen* img, FILE* arch, BmpHeader* imgHeader);
int guardarImagen(Imagen* img, const char* nombreArch, BmpHeader* imgHeader);

int leerHeader(BmpHeader* h, FILE* arch);
void testHeader(BmpHeader* h);

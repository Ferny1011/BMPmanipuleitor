#include "funciones_zapata.h"


int _min(int a, int b){
    return (a < b) ? a : b;
}

void testHeader(BmpHeader *cabecera){
    printf("Cabecera Tam: %d\n", (int)sizeof(BmpHeader));
    printf("Cabecera firma: %c%c\n", cabecera->firma[0], cabecera->firma[1]);
    printf("Alto: %d\n", cabecera->altura);
    printf("Ancho: %d\n", cabecera->anchura);
    printf("Profundidad: %d\n", cabecera->profundidad);
    printf("Compresion: %d\n", cabecera->compresion);
    printf("Tam Imagen: %d\n", cabecera->tamImagen);
}

// TODO (Santiago#1#05/29/25): pasar los pixels ya reservados, evitar memory leaks

int leerImagen(const char *fileName, PixelRGB **pixels, BmpHeader *imgHeader){
	FILE *imgFile = fopen(fileName, "rb");
	if (!imgFile){
		printf("Error al leer el archivo %s", fileName);
		return 0;
	}

	fread(imgHeader, 54, 1, imgFile);
	if (imgHeader->firma[0] != 'B' || imgHeader->firma[1] != 'M') {
		puts("No es un archivo BMP valido\n");
		fclose(imgFile);
		return 0;
	}
	int tamFilaConPadding = (int)(4 * ceil((float)(imgHeader->anchura) / 4.0f))*(imgHeader->profundidad / 8);
	int totalPixels = imgHeader->anchura * imgHeader->altura;
	*pixels = (PixelRGB*)malloc(totalPixels * sizeof(PixelRGB));
	if (*pixels == NULL) {
		puts("Memory allocation failed\n");
		fclose(imgFile);
		return 0;
	}
	int i = 0;
	PixelRGB *currentRowPointer = *pixels+((imgHeader->altura - 1) * imgHeader->anchura);
	for (i = 0; i < imgHeader->altura; i++) {
		fseek(imgFile, imgHeader->dataOffset + (i * tamFilaConPadding), SEEK_SET);
		fread(currentRowPointer, sizeof(PixelRGB), imgHeader->anchura, imgFile);
		currentRowPointer -= imgHeader->anchura;
	}
	fclose(imgFile);
	return 1;
}

void WriteImage(const char *fileName, PixelRGB *pixels, BmpHeader imgHeader){
	FILE *outputFile = fopen(fileName, "wb");
	int paddedRowSize = (int)(4 * ceil((float)(imgHeader.anchura) / 4.0f)) * (imgHeader.profundidad / 8);
	fwrite(&imgHeader, sizeof(BmpHeader), 1, outputFile);
	fseek(outputFile, imgHeader.dataOffset, SEEK_SET);

	char *paddingBytes = (char*)calloc(paddedRowSize, 1);
	for (int i = 0; i < imgHeader.altura; i++) {
		int pixelOffset = ((imgHeader.altura - i) - 1) * imgHeader.anchura;
		memcpy(paddingBytes, &pixels[pixelOffset], imgHeader.anchura * sizeof(PixelRGB));
		fwrite(paddingBytes, 1, paddedRowSize, outputFile);
	}
	free(paddingBytes);
	fclose(outputFile);
}

void convertirEscalaDeGrises(PixelRGB *pixels, const BmpHeader *imgHeader){
    for ( int i = 0; i < imgHeader->anchura * imgHeader->altura; i++ ){
        uint8_t gray = (uint8_t)(0.299 * pixels[i].r + 0.587 * pixels[i].g + 0.114 * pixels[i].b);
        pixels[i].r = gray;
        pixels[i].g = gray;
        pixels[i].b = gray;
    }
}

void cambioTonalidad(PixelRGB *pixels, const BmpHeader *imgHeader, float rojo, float verde, float azul){
    for ( int i = 0; i < imgHeader->anchura * imgHeader->altura; i++ ){
        pixels[i].r = _min((int)(pixels[i].r * rojo), 255);
        pixels[i].g = _min((int)(pixels[i].g * verde), 255);
        pixels[i].b = _min((int)(pixels[i].b * azul), 255);
    }
}

uint8_t _adjustContrast(uint8_t pixel, float factor) {
    int newPixel = (int)(factor * (pixel - 128) + 128);
    if (newPixel < 0) newPixel = 0;
    if (newPixel > 255) newPixel = 255;
    return (uint8_t)newPixel;
}

void cambioContraste(PixelRGB *pixels, const BmpHeader *imgHeader, int contraste){
    float factor = (259.0 * (contraste + 255.0)) / (255.0 * (259.0 - contraste));
    for ( int i = 0; i < imgHeader->anchura * imgHeader->altura; i++ ){
        pixels[i].r = _adjustContrast(pixels[i].r, factor);
        pixels[i].g = _adjustContrast(pixels[i].g, factor);
        pixels[i].b = _adjustContrast(pixels[i].b, factor);
    }
}

void agregarOperacion(OpcionesImagen *opciones, TipoOperacion op, int valor){
    if ( opciones->numOperaciones < 16 ){
        opciones->operaciones[opciones->numOperaciones].operacion = op;
        opciones->operaciones[opciones->numOperaciones].valor = valor;
        opciones->operaciones[opciones->numOperaciones].activo = true;
        opciones->numOperaciones++;
    }
}

void parse_argv(int argc, char* argv[], OpcionesImagen *opciones){
    opciones->numOperaciones = 0;
    opciones->cantImg = 0;
    int valor;
    char *currArgv, *equalPos;

    for(int i = 1; i < argc; i++) {
        currArgv = argv[i];

        equalPos = strchr(currArgv, '=');
        valor = -1;

        if(equalPos != NULL) {
            *equalPos = '\0';
            valor = atoi(equalPos + 1);
        }

        if(!strcmp(currArgv,"--escala-de-grises")) {
            agregarOperacion(opciones, OP_ESCALA_GRISES, valor);
        }
        else if(!strcmp(currArgv,"--rotar-derecha")) {
            agregarOperacion(opciones, OP_ROTAR_DERECHA, valor);
        }
        else if(!strcmp(currArgv,"--rotar-izquierda")) {
            agregarOperacion(opciones, OP_ROTAR_IZQUIERDA, valor);
        }
        else if(!strcmp(currArgv, "--tonalidad-azul")) {
            agregarOperacion(opciones, OP_TONALIDAD_AZUL, valor);
        }
        else if(!strcmp(currArgv, "--aumentar-contraste")) {
            agregarOperacion(opciones, OP_AUMENTAR_CONTRASTE, valor);
        }
        else if(!strcmp(currArgv, "--disminuir-contraste")) {
            agregarOperacion(opciones, OP_DISMINUIR_CONTRASTE, valor);
        }
        else if(!strcmp(currArgv, "--tonalidad-roja")) {
            agregarOperacion(opciones, OP_TONALIDAD_ROJA, valor);
        }
        else if(!strcmp(currArgv, "--tonalidad-verde")) {
            agregarOperacion(opciones, OP_TONALIDAD_VERDE, valor);
        }
        else if(!strcmp(currArgv, "--recortar")) {
            agregarOperacion(opciones, OP_RECORTAR, valor);
        }
        else if(!strcmp(currArgv, "--negativo")) {
            agregarOperacion(opciones, OP_NEGATIVO, valor);
        }
        else if(!strcmp(currArgv, "--achicar")) {
            agregarOperacion(opciones, OP_ACHICAR, valor);
        }
        else if(!strcmp(currArgv, "--espejar-horizontal")) {
            agregarOperacion(opciones, OP_ESPEJAR_HORIZONTAL, valor);
        }
        else if(!strcmp(currArgv, "--espejar-vertical")) {
            agregarOperacion(opciones, OP_ESPEJAR_VERTICAL, valor);
        }
        else if(!strcmp(currArgv, "--concatenar-horizontal")) {
            agregarOperacion(opciones, OP_CONCATENAR_HORIZONTAL, valor);
        }
        else if(!strcmp(currArgv, "--concatenar-vertical")) {
            agregarOperacion(opciones, OP_CONCATENAR_VERTICAL, valor);
        }
        else if(*(currArgv)!='-') {
            if(opciones->cantImg < 2) {
                opciones->imgFiles[opciones->cantImg] = currArgv;
                opciones->cantImg++;
            } else {
                fprintf(stderr, "Error: Too many image files specified.\n");
                exit(EXIT_FAILURE);
            }
        }
//        if(equalPos != NULL)
//            *equalPos = '=';
    }
}
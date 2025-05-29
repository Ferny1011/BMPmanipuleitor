#include "funciones_zapata.h"

void _printMatInt(const void* elem){
    printf("%2d|", *(int*)elem);
}

void _printPixelRGB(const void* elem){
    PixelRGB* pixel = (PixelRGB*)elem;
    printf("(%3d,%3d,%3d)|", pixel->r, pixel->g, pixel->b);
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

int leerImagen(const char *fileName, PixelRGB **pixels, BmpHeader *imgHeader)
{
	FILE *imgFile = fopen(fileName, "rb");
	if (!imgFile){
		perror("Error opening file");
		return 0;
	}
	
	fread(imgHeader, 54, 1, imgFile);
	if (imgHeader->firma[0] != 'B' || imgHeader->firma[1] != 'M') {
		puts("Not a valid BMP file\n");
		fclose(imgFile);
		return 0;
	}
	int tamFilaConPadding = (int)(4 * ceil((float)(imgHeader->anchura) / 4.0f))*(imgHeader->profundidad / 8);
	int tamFilaSinPadding = imgHeader->anchura * (imgHeader->profundidad / 8);
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

void WriteImage(const char *fileName, PixelRGB *pixels, BmpHeader imgHeader)
{
	FILE *outputFile = fopen(fileName, "wb");
	int unpaddedRowSize = imgHeader.anchura * (imgHeader.profundidad / 8);
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

void convertToGrayscalePixels(PixelRGB *pixels, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        uint8_t gray = (uint8_t)(0.299 * pixels[i].r + 0.587 * pixels[i].g + 0.114 * pixels[i].b);
        pixels[i].r = gray;
        pixels[i].g = gray;
        pixels[i].b = gray;
    }
}



#include "funciones_quiroga.h"


void concatenarImagenVertical (PixelRGB *imagenSuperior, const BmpHeader *imgSuperiorHeader,PixelRGB *imagenInferior, const BmpHeader *imgInferiorHeader){
    FILE *outputFile = fopen("resources/concatenada.bmp", "wb");
    char *paddingBytes = NULL;
    char *paddingBytes2 = NULL;
    int i, paddedRowSize, pixelOffset, relleno;
    BmpHeader imgHeader;

    if(paddingBytes){
        puts("Error al reservar memoria");
        return;
    }


    imgHeader = imgSuperiorHeader->anchura > imgInferiorHeader->anchura ? *imgSuperiorHeader : *imgInferiorHeader;
    imgHeader.altura = imgSuperiorHeader->altura + imgInferiorHeader->altura;
    imgHeader.tamImagen = imgHeader.anchura * imgHeader.altura * imgHeader.profundidad;
    imgHeader.tamArchivo = imgHeader.headerSize + imgHeader.tamImagen;
    paddedRowSize = (int)(4 * ceil((float)(imgHeader.anchura) / 4.0f)) * (imgHeader.profundidad / 8);
    relleno = abs(imgSuperiorHeader->anchura - imgInferiorHeader->anchura) + paddedRowSize;
    paddingBytes = (char*)calloc(paddedRowSize, 1);
    paddingBytes2 = (char*)calloc(relleno, 1);

	fwrite(&imgHeader, sizeof(BmpHeader), 1, outputFile);
	fseek(outputFile, imgHeader.dataOffset, SEEK_SET);


	for (i = 0; i < imgSuperiorHeader->altura; i++) {
	    if(imgSuperiorHeader->anchura < imgInferiorHeader->anchura){
            pixelOffset = ((imgSuperiorHeader->altura - i) - 1) * imgSuperiorHeader->anchura;
            memcpy(paddingBytes2, &imagenSuperior[pixelOffset], imgSuperiorHeader->anchura * sizeof(PixelRGB));
            fwrite(paddingBytes2, 1, paddedRowSize, outputFile);
	    }else{
	        pixelOffset = ((imgSuperiorHeader->altura - i) - 1) * imgHeader.anchura;
            memcpy(paddingBytes, &imagenSuperior[pixelOffset], imgHeader.anchura * sizeof(PixelRGB));
            fwrite(paddingBytes, 1, paddedRowSize, outputFile);
	    }
	}

	for(i = 0; i < imgInferiorHeader->altura; i++){
        if(imgSuperiorHeader->anchura > imgInferiorHeader->anchura){
            pixelOffset = ((imgInferiorHeader->altura - i) - 1) * imgInferiorHeader->anchura;
            memcpy(paddingBytes2, &imagenInferior[pixelOffset], imgInferiorHeader->anchura * sizeof(PixelRGB));
            fwrite(paddingBytes2, 1, paddedRowSize, outputFile);
	    }else{
            pixelOffset = ((imgInferiorHeader->altura - i) - 1) * imgHeader.anchura;
            memcpy(paddingBytes, &imagenInferior[pixelOffset], imgHeader.anchura * sizeof(PixelRGB));
            fwrite(paddingBytes, 1, paddedRowSize, outputFile);
	    }
	}

	free(paddingBytes);
	fclose(outputFile);
}

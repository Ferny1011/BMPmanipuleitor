#include "funciones_grupo.h"
#include "funciones_zapata.h"



/**
* Apellido(s), nombre(s):
* DNI: (s�lo n�meros, sin puntos)
* Entrega: S�/No.
*
* Apellido(s), nombre(s): Zapata Santiago
* DNI: 44525943
* Entrega: S�/No.
*
* Apellido(s), nombre(s):
* DNI: (s�lo n�meros, sin puntos)
* Entrega: S�/No.
*/

int main(){
    PixelRGB *pixels;
	BmpHeader imgHeader;
	leerImagen("resources/imagen.bmp", &pixels, &imgHeader);
	convertToGrayscalePixels(pixels, imgHeader.anchura, imgHeader.altura);
	WriteImage("resources/img_gray.bmp", pixels, imgHeader);
	free(pixels);
	return 0;
}

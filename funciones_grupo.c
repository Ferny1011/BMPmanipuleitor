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

int main()
{
    Imagen img;
    BmpHeader cabecera;
    FILE *f = fopen("imagen.bmp", "rb");
    if (!f) {
        perror("No se pudo abrir el archivo");
        return 1;
    }
    leerHeader(&cabecera, f);
    testHeader(&cabecera);
    createMatBloque(&img, sizeof(PixelRGB), cabecera.anchura, cabecera.altura);
    leerImagen(&img, f, &cabecera);
    guardarImagen(&img, "guardada.bmp", &cabecera);

    fclose(f);
    return 0;
}

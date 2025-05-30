#include "funciones_grupo.h"
#include "funciones_zapata.h"


/**
* Apellido(s), nombre(s):
* DNI: (s�lo n�meros, sin puntos)
* Entrega: S�/No.
*
* Apellido(s), nombre(s): Zapata Santiago
* DNI: 44525943
* Entrega: Si.
*
* Apellido(s), nombre(s):
* DNI: (s�lo n�meros, sin puntos)
* Entrega: S�/No.
*/

int main(int argc, char* argv[]){
    OpcionesImagen opciones;
    parse_argv(argc, argv, &opciones);
    ejecutarOperaciones(&opciones);
	return 0;
}

void ejecutarOperaciones(OpcionesImagen *opciones){
    PixelRGB *imagen1 = NULL;
    BmpHeader imgHeader1;
    PixelRGB *imagen2 = NULL;
    BmpHeader imgHeader2;
    if( opciones->numOperaciones == 0 ) {
        puts("No se especificaron operaciones validas.\n");
        return;
    }
    if( opciones->cantImg > 0 ) {
        if(!leerImagen(opciones->imgFiles[0], &imagen1, &imgHeader1)) {
            puts("Error al leer la imagen 1.\n");
            return;
        }
        if( opciones->cantImg > 1 ) {
            if(!leerImagen(opciones->imgFiles[1], &imagen2, &imgHeader2)) {
                puts("Error al leer la imagen 2.\n");
                free(imagen1);
                return;
            }
        }
    } else {
        puts("No se especificaron archivos de imagen.\n");
        return;
    }
    for( int i = 0; i < opciones->numOperaciones; i++ ) {
        OperacionImagen *op = &opciones->operaciones[i];
        if(op->activo) {
            switch(op->operacion) {
                case OP_ESCALA_GRISES:
                    convertirEscalaDeGrises(imagen1, &imgHeader1);
                    break;
                case OP_ROTAR_DERECHA:
                    // Implementar rotacion a la derecha
                    break;
                case OP_ROTAR_IZQUIERDA:
                    // Implementar rotacion a la izquierda
                    break;
                case OP_TONALIDAD_AZUL:
                    cambioTonalidad(imagen1, &imgHeader1, 1, 1, 1 + ((float)op->valor / 100));
                    break;
                case OP_AUMENTAR_CONTRASTE:
                    // Implementar aumento de contraste
                    break;
                case OP_DISMINUIR_CONTRASTE:
                    // Implementar disminucion de contraste
                    break;
                case OP_TONALIDAD_ROJA:
                    cambioTonalidad(imagen1, &imgHeader1, 1 + ((float)op->valor / 100), 1, 1);
                    break;
                case OP_TONALIDAD_VERDE:
                    cambioTonalidad(imagen1, &imgHeader1, 1, 1 + ((float)op->valor / 100), 1);
                    break;
                case OP_RECORTAR:
                    // Implementar recorte
                    break;
                case OP_NEGATIVO:
                    // Implementar negativo
                    break;
                case OP_ACHICAR:
                    // Implementar achicamiento
                    break;
                case OP_ESPEJAR_HORIZONTAL:
                    // Implementar espejado horizontal
                    break;
                case OP_ESPEJAR_VERTICAL:
                    // Implementar espejado vertical
                    break;
                case OP_CONCATENAR_HORIZONTAL:
                    // Implementar concatenacion horizontal
                    break;
                case OP_CONCATENAR_VERTICAL:
                    concatenarImagenVertical(imagen1, &imgHeader1, imagen2, &imgHeader2);
                    break;
                default:
                    printf("Operacion no reconocida.\n");
            }
        }
    }

    if(opciones->cantImg > 0) {
        WriteImage("resources/resultado.bmp", imagen1, imgHeader1);
        free(imagen1);
        if(opciones->cantImg > 1) {
            WriteImage("resources/resultado2.bmp", imagen2, imgHeader2);
            free(imagen2);
        }
    }
}

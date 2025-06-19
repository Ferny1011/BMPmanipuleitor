#include "funciones_grupo.h"
#include "funciones_zapata.h"
#include "funciones_quiroga.h"

/**
* Apellido(s), nombre(s):
* DNI: (sólo números, sin puntos)
* Entrega: Sí/No.
*
* Apellido(s), nombre(s): Zapata Santiago
* DNI: 44525943
* Entrega: Si.
*
* Apellido(s), nombre(s):
* DNI: (sólo números, sin puntos)
* Entrega: Sí/No.
*/

int main(int argc, char* argv[]){
    OpcionesImagen opciones;
    opciones.numOperaciones = 0;
    opciones.cantImg = 0;

    //mostrar ayuda si se recibe el argumento --help o -h
    if(argc == 1 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))) {
        mostrarAyuda();
        return 0;
    }
    parse_argv(argc, argv, &opciones);
    ejecutarOperaciones(&opciones);
    return 0;
}

void ejecutarOperaciones(OpcionesImagen *opciones) {
    char nombreSalida[256];
    TDA_ImagenBMP *imagenOriginal = NULL, *imagenCopia = NULL, *imagen2 = NULL;
    int i;
    int modoExtra = 0;

    if (opciones->cantImg == 0) {
        printf("Error: No se especifico ningun archivo de imagen.\n");
        return;
    }

    if (opciones->numOperaciones == 0) {
        printf("Advertencia: No se especificaron operaciones validas.\n");
        return;
    }

    imagenOriginal = crearImagenBMP(opciones->imgFiles[0]);
    if (!imagenOriginal) {
        printf("Error: No se pudo cargar la imagen '%s'.\n", opciones->imgFiles[0]);
        return;
    }

    printf("Imagen cargada: %s (%dx%d)\n", opciones->imgFiles[0],
           imagenOriginal->cabecera.anchura, imagenOriginal->cabecera.altura);

    if (opciones->cantImg > 1) {
        imagen2 = crearImagenBMP(opciones->imgFiles[1]);
        if (!imagen2) {
            printf("Error: No se pudo cargar la segunda imagen '%s'.\n", opciones->imgFiles[1]);
            liberarImagenBMP(imagenOriginal);
            return;
        }
        printf("Segunda imagen cargada: %s (%dx%d)\n", opciones->imgFiles[1],
               imagen2->cabecera.anchura, imagen2->cabecera.altura);
    }

    for (i = 0; i < opciones->numOperaciones; i++) {
        if (opciones->operaciones[i].operacion == OP_FUNCIONALIDAD_EXTRA) {
            modoExtra = 1;
        }
    }

    if (modoExtra) {
        aplicarFiltrosEnCadena(imagenOriginal, opciones->operaciones, opciones->numOperaciones, imagen2);

        generarNombreArchivo(opciones->imgFiles[0], OP_FUNCIONALIDAD_EXTRA, nombreSalida);
        if (guardarImagen(nombreSalida, imagenOriginal)) {
            printf("\n\t-> Imagen guardada como: %s\n", nombreSalida);
        } else {
            printf("\nError al guardar la imagen procesada.\n");
        }

        liberarImagenBMP(imagenOriginal);
        liberarImagenBMP(imagen2);
        return;
    }

    for (i = 0; i < opciones->numOperaciones; i++) {
        imagenCopia = crearImagenBMP(opciones->imgFiles[0]);
        if (!imagenCopia) {
            printf("Error: No se pudo crear una copia de la imagen original para la operacion '%s'.\n",
                   obtenerNombreOperacion(opciones->operaciones[i].operacion));
            continue;
        }

        printf("Aplicando operacion: %s", obtenerNombreOperacion(opciones->operaciones[i].operacion));
        if (opciones->operaciones[i].valor != -1) {
            printf(" (valor: %d)", opciones->operaciones[i].valor);
            if (opciones->operaciones[i].valor > 100 || opciones->operaciones[i].valor < 0) {
                puts("Error: El valor de la operacion debe estar en el rango [0, 100]. No se generara la imagen.");
                liberarImagenBMP(imagenCopia);
                continue;
            }
        }

        if (requiereSegundaImagen(opciones->operaciones[i].operacion) && !imagen2) {
            printf("\nError: La operacion '%s' requiere una segunda imagen, pero no se proporciono.\n",
                   obtenerNombreOperacion(opciones->operaciones[i].operacion));
            liberarImagenBMP(imagenCopia);
            continue;
        }

        switch (opciones->operaciones[i].operacion) {
            case OP_ESCALA_GRISES:
                convertirEscalaDeGrises(imagenCopia);
                break;
            case OP_TONALIDAD_AZUL:
                cambioTonalidad(imagenCopia, 1, 1, 1 + ((float) opciones->operaciones[i].valor / 100));
                break;
            case OP_TONALIDAD_ROJA:
                cambioTonalidad(imagenCopia, 1 + ((float) opciones->operaciones[i].valor / 100), 1, 1);
                break;
            case OP_TONALIDAD_VERDE:
                cambioTonalidad(imagenCopia, 1, 1 + ((float) opciones->operaciones[i].valor / 100), 1);
                break;
            case OP_AUMENTAR_CONTRASTE:
                cambioContraste(imagenCopia, opciones->operaciones[i].valor);
                break;
            case OP_DISMINUIR_CONTRASTE:
                cambioContraste(imagenCopia, opciones->operaciones[i].valor * -1);
                break;
            case OP_NEGATIVO:
                cambioNegativo(imagenCopia);
                break;
            case OP_CONCATENAR_VERTICAL:
                imagenCopia = concatenarImagenVertical(imagenOriginal, imagen2);
                break;
            case OP_CONCATENAR_HORIZONTAL:
                imagenCopia = concatenarImagenHorizontal(imagenOriginal, imagen2);
                break;
            case OP_ESPEJAR_VERTICAL:
                espejarVertical(imagenCopia);
                break;
            case OP_ESPEJAR_HORIZONTAL:
                espejarHorizontal(imagenCopia);
                break;
            case OP_ROTAR_DERECHA:
                rotarDerecha(imagenCopia);
                break;
            case OP_ROTAR_IZQUIERDA:
                rotarIzquierda(imagenCopia);
                break;
            default:
                printf("Error: Operacion desconocida.\n");
                break;
        }

        generarNombreArchivo(opciones->imgFiles[0], opciones->operaciones[i].operacion, nombreSalida);
        if (guardarImagen(nombreSalida, imagenCopia)) {
            printf("\n\t-> Imagen guardada como: %s\n", nombreSalida);
        } else {
            printf("\nError al guardar la imagen procesada.\n");
        }

        liberarImagenBMP(imagenCopia);
    }

    liberarImagenBMP(imagenOriginal);
    liberarImagenBMP(imagen2);
    printf("\nProcesamiento completado.\n");
}



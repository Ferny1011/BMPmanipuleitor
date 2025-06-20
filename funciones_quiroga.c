#include "funciones_quiroga.h"

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
            case OP_RECORTAR:
                imagenCopia= recortar(imagenOriginal, opciones->operaciones[i].valor);
                break;
            case OP_ACHICAR:
                imagenCopia= achicar(imagenOriginal, opciones->operaciones[i].valor);
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


TDA_ImagenBMP *concatenarImagenVertical (TDA_ImagenBMP *imagenSuperior, TDA_ImagenBMP *imagenInferior) {
    TDA_ImagenBMP *imagenConcatenada = NULL;
    int i, j;
    if (!imagenSuperior || !imagenInferior) {
        puts("Error: Una o ambas imagenes no estan inicializadas.");
        return NULL;
    }
    imagenConcatenada = (TDA_ImagenBMP*)malloc(sizeof(TDA_ImagenBMP));
    if (!imagenConcatenada) {
        puts("Error al reservar memoria para la imagen concatenada.");
        return NULL;
    }
    imagenConcatenada->cabecera = imagenSuperior->cabecera;
    imagenConcatenada->cabecera.altura = imagenSuperior->cabecera.altura + imagenInferior->cabecera.altura;
    imagenConcatenada->cabecera.anchura = imagenSuperior->cabecera.anchura > imagenInferior->cabecera.anchura ?
        imagenSuperior->cabecera.anchura : imagenInferior->cabecera.anchura;
    imagenConcatenada->cabecera.tamImagen = imagenConcatenada->cabecera.anchura * imagenConcatenada->cabecera.altura * sizeof(PixelRGB);
    imagenConcatenada->cabecera.tamArchivo = sizeof(BmpHeader) + imagenConcatenada->cabecera.tamImagen;

    imagenConcatenada->matrizDePixeles = (TDA_Matriz*)malloc(sizeof(TDA_Matriz));
    if (!imagenConcatenada->matrizDePixeles) {
        puts("Error al reservar memoria para la matriz de pixeles de la imagen concatenada.");
        free(imagenConcatenada);
        return NULL;
    }
    if (!crearMatPixeles(imagenConcatenada->matrizDePixeles, imagenConcatenada->cabecera.anchura, imagenConcatenada->cabecera.altura)) {
        puts("Error al crear la matriz de pixeles para la imagen concatenada.");
        free(imagenConcatenada);
        free(imagenConcatenada->matrizDePixeles);
        return NULL;
    }
    PixelRGB pixelVerde = {0, 255, 0};
    for (i = 0; i < imagenInferior->cabecera.altura; i++) {
        for (j = 0; j < imagenConcatenada->cabecera.anchura; j++) {
            if (j < imagenInferior->cabecera.anchura)
                imagenConcatenada->matrizDePixeles->data[i][j] = imagenInferior->matrizDePixeles->data[i][j];
            else
                imagenConcatenada->matrizDePixeles->data[i][j] = pixelVerde;
        }
    }
    for (i = 0; i < imagenSuperior->cabecera.altura; i++) {
        for (j = 0; j < imagenConcatenada->cabecera.anchura; j++) {
            if (j < imagenSuperior->cabecera.anchura)
                imagenConcatenada->matrizDePixeles->data[i + imagenInferior->cabecera.altura][j] = imagenSuperior->matrizDePixeles->data[i][j];
            else
                imagenConcatenada->matrizDePixeles->data[i + imagenInferior->cabecera.altura][j] = pixelVerde;
        }
    }
    return imagenConcatenada;
}

TDA_ImagenBMP *concatenarImagenHorizontal(TDA_ImagenBMP *imagenIzquierda, TDA_ImagenBMP *imagenDerecha) {
    TDA_ImagenBMP *imagenConcatenada;
    PixelRGB pixelVerde;
    int i, j;

    if (!imagenIzquierda || !imagenDerecha) {
        puts("Error Una o ambas imágenes no están inicializadas.");
        return NULL;
    }

    imagenConcatenada = (TDA_ImagenBMP*)malloc(sizeof(TDA_ImagenBMP));
    if (!imagenConcatenada) {
        puts("Error al reservar memoria para la imagen concatenada.");
        return NULL;
    }


    imagenConcatenada->cabecera = imagenIzquierda->cabecera;
    imagenConcatenada->cabecera.anchura = imagenIzquierda->cabecera.anchura + imagenDerecha->cabecera.anchura;
    imagenConcatenada->cabecera.altura = imagenIzquierda->cabecera.altura > imagenDerecha->cabecera.altura ?
                                          imagenIzquierda->cabecera.altura : imagenDerecha->cabecera.altura;

    imagenConcatenada->cabecera.tamImagen = imagenConcatenada->cabecera.anchura * imagenConcatenada->cabecera.altura * sizeof(PixelRGB);
    imagenConcatenada->cabecera.tamArchivo = sizeof(BmpHeader) + imagenConcatenada->cabecera.tamImagen;

    imagenConcatenada->matrizDePixeles = (TDA_Matriz*)malloc(sizeof(TDA_Matriz));
    if (!imagenConcatenada->matrizDePixeles) {
        puts("Error al reservar memoria para la matriz de píxeles.");
        free(imagenConcatenada);
        return NULL;
    }

    if (!crearMatPixeles(imagenConcatenada->matrizDePixeles, imagenConcatenada->cabecera.anchura, imagenConcatenada->cabecera.altura)) {
        puts("Error al crear la matriz de píxeles.");
        free(imagenConcatenada->matrizDePixeles);
        free(imagenConcatenada);
        return NULL;
    }

    pixelVerde.r = 0;
    pixelVerde.g = 255;
    pixelVerde.b = 0;

    for (i = 0; i < imagenConcatenada->cabecera.altura; i++) {
        for (j = 0; j < imagenIzquierda->cabecera.anchura; j++) {
            if (i < imagenIzquierda->cabecera.altura) {
                imagenConcatenada->matrizDePixeles->data[i][j] = imagenIzquierda->matrizDePixeles->data[i][j];
            } else {
                imagenConcatenada->matrizDePixeles->data[i][j] = pixelVerde;
            }
        }

        for (j = 0; j < imagenDerecha->cabecera.anchura; j++) {
            if (i < imagenDerecha->cabecera.altura) {
                imagenConcatenada->matrizDePixeles->data[i][j + imagenIzquierda->cabecera.anchura] =
                    imagenDerecha->matrizDePixeles->data[i][j];
            } else {
                imagenConcatenada->matrizDePixeles->data[i][j + imagenIzquierda->cabecera.anchura] = pixelVerde;
            }
        }
    }

    return imagenConcatenada;
}
void aplicarFiltrosEnCadena(TDA_ImagenBMP *imagen, OperacionImagen *operaciones, int cantidadOperaciones, TDA_ImagenBMP *imagen2) {
    int i;
    TipoOperacion op;

    for (i = 0; i < cantidadOperaciones; i++) {
    op = operaciones[i].operacion;

    if (op != OP_FUNCIONALIDAD_EXTRA) {
        if (op == OP_ESCALA_GRISES) {
            convertirEscalaDeGrises(imagen);
        } else if (op == OP_TONALIDAD_AZUL) {
            cambioTonalidad(imagen, 1.0f, 1.0f, 1.0f + ((float) operaciones[i].valor / 100.0f));
        } else if (op == OP_TONALIDAD_ROJA) {
            cambioTonalidad(imagen, 1.0f + ((float) operaciones[i].valor / 100.0f), 1.0f, 1.0f);
        } else if (op == OP_TONALIDAD_VERDE) {
            cambioTonalidad(imagen, 1.0f, 1.0f + ((float) operaciones[i].valor / 100.0f), 1.0f);
        } else if (op == OP_AUMENTAR_CONTRASTE) {
            cambioContraste(imagen, operaciones[i].valor);
        } else if (op == OP_DISMINUIR_CONTRASTE) {
            cambioContraste(imagen, -operaciones[i].valor);
        } else if (op == OP_NEGATIVO) {
            cambioNegativo(imagen);
        } else if (op == OP_ESPEJAR_VERTICAL) {
            espejarVertical(imagen);
        } else if (op == OP_ESPEJAR_HORIZONTAL) {
            espejarHorizontal(imagen);
        } else if (op == OP_ROTAR_DERECHA) {
            rotarDerecha(imagen);
        } else if (op == OP_ROTAR_IZQUIERDA) {
            rotarIzquierda(imagen);
        } else if (op == OP_CONCATENAR_VERTICAL && imagen2 != NULL) {
            TDA_ImagenBMP *tmp = concatenarImagenVertical(imagen, imagen2);
            if (tmp != NULL) {
                freeMat(imagen->matrizDePixeles);
                imagen->cabecera = tmp->cabecera;
                imagen->matrizDePixeles = tmp->matrizDePixeles;
                tmp->matrizDePixeles = NULL;
                free(tmp);
            }
        } else if (op == OP_CONCATENAR_HORIZONTAL && imagen2 != NULL) {
            TDA_ImagenBMP *tmp = concatenarImagenHorizontal(imagen, imagen2);
            if (tmp != NULL) {
                freeMat(imagen->matrizDePixeles);
                imagen->cabecera = tmp->cabecera;
                imagen->matrizDePixeles = tmp->matrizDePixeles;
                tmp->matrizDePixeles = NULL;
                free(tmp);
            }
        } else {
            printf("Operacion '%d' no soportada o imagen2 faltante.\n", (int)op);
        }
    }
}
}

#include "funciones_quiroga.h"


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

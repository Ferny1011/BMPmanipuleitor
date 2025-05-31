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

void ejecutarOperaciones(OpcionesImagen *opciones){
    char nombreSalida[256];
    int totalPixelsBlock;
    PixelRGB *imagenOriginal = NULL, *imagenCopia = NULL, *imagen2 = NULL;
    BmpHeader headerOriginal, header2;
    if(opciones->cantImg == 0) {
        printf("Error: No se especifico ningun archivo de imagen.\n");
        return;
    }
    
    if(opciones->numOperaciones == 0) {
        printf("Advertencia: No se especificaron operaciones validas.\n");
        return;
    }
    //TODO (Santiago#1#05/29/25): Crear funcion para reservar memoria para los bloques de pixels
    if(!leerImagen(opciones->imgFiles[0], &imagenOriginal, &headerOriginal)) {
        printf("Error: No se pudo leer el archivo '%s'.\n", opciones->imgFiles[0]);
        return;
    }
    
    printf("Imagen cargada: %s (%dx%d)\n", opciones->imgFiles[0], 
           headerOriginal.anchura, headerOriginal.altura);
    totalPixelsBlock = headerOriginal.anchura * headerOriginal.altura * sizeof(PixelRGB);
    // Verificar si hay una segunda imagen
    if(opciones->cantImg > 1) {
        if(!leerImagen(opciones->imgFiles[1], &imagen2, &header2)) {
            printf("Error: No se pudo leer el archivo '%s'.\n", opciones->imgFiles[1]);
            free(imagenOriginal);
            return;
        }
        printf("Segunda imagen cargada: %s (%dx%d)\n", opciones->imgFiles[1], 
               header2.anchura, header2.altura);
    }
    
    // Crear copia de la imagen original para cada operación, evitando sobrescribir la original
    imagenCopia = (PixelRGB*)malloc(totalPixelsBlock);
    if(imagenCopia == NULL) {
        printf("Error: No se pudo asignar memoria para la operacion.\n");
        free(imagenOriginal);
        if(imagen2) 
            free(imagen2);
        exit(1);
    }

    for(int i = 0; i < opciones->numOperaciones; i++) {
        //if(!opciones->operaciones[i].activo) continue;
        memcpy(imagenCopia, imagenOriginal, totalPixelsBlock);
        
        printf("Aplicando operacion: %s", obtenerNombreOperacion(opciones->operaciones[i].operacion));
        if(opciones->operaciones[i].valor != -1){
            printf(" (valor: %d)\n", opciones->operaciones[i].valor);
            if(opciones->operaciones[i].valor > 100 || opciones->operaciones[i].valor < 0) {
                puts("Error: El valor de la operacion debe estar en el rango [0, 100]. No se generara la imagen.");
                continue;
            }
        }
        switch(opciones->operaciones[i].operacion) {
            case OP_ESCALA_GRISES:
                convertirEscalaDeGrises(imagenCopia, &headerOriginal);
                break;
            case OP_TONALIDAD_AZUL:
                cambioTonalidad(imagenCopia, &headerOriginal, 1, 1, 1 + ((float) opciones->operaciones[i].valor / 100));
                break;
            case OP_TONALIDAD_ROJA:
                cambioTonalidad(imagenCopia, &headerOriginal, 1 + ((float) opciones->operaciones[i].valor / 100), 1, 1);
                break;
            case OP_TONALIDAD_VERDE:
                cambioTonalidad(imagenCopia, &headerOriginal, 1, 1 + ((float) opciones->operaciones[i].valor / 100), 1);
                break;
            case OP_AUMENTAR_CONTRASTE:
                cambioContraste(imagenCopia, &headerOriginal, opciones->operaciones[i].valor);
                break;
            case OP_DISMINUIR_CONTRASTE:
                cambioContraste(imagenCopia, &headerOriginal, opciones->operaciones[i].valor * -1);
                break;            
            case OP_NEGATIVO:
                cambioNegativo(imagenCopia, &headerOriginal);
                break;
            case OP_CONCATENAR_VERTICAL:
                if(opciones->cantImg < 2) {
                    puts("Error: Se requiere una segunda imagen para la operación de concatenación vertical.");
                    continue;
                }
                concatenarImagenVertical(imagenCopia, &headerOriginal, imagen2, &header2);
                break;
            case OP_ROTAR_DERECHA:
            case OP_ROTAR_IZQUIERDA:
            case OP_RECORTAR:
            case OP_ACHICAR:
            case OP_ESPEJAR_HORIZONTAL:
            case OP_ESPEJAR_VERTICAL:
            case OP_CONCATENAR_HORIZONTAL:
                printf("Error: La operacion '%s' aún no esta implementada.\n", 
                obtenerNombreOperacion(opciones->operaciones[i].operacion));
                break;            
            default:
                printf("Error: Operacion desconocida.\n");
                break;
        }
        
        if(!requiereSegundaImagen(opciones->operaciones[i].operacion)) {
            generarNombreArchivo(opciones->imgFiles[0], opciones->operaciones[i].operacion, nombreSalida);
            WriteImage(nombreSalida, imagenCopia, headerOriginal);
            printf(" -> Imagen guardada como: %s\n", nombreSalida);
        } else {
            // Para operaciones que requieren una segunda imagen, la imagen se guarda en otro momento
            continue;
        }
    }
    free(imagenCopia);
    free(imagenOriginal);
    if(imagen2) {
        free(imagen2);
    }
    printf("\nProcesamiento completado.\n");
}


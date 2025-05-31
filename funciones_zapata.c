#include "funciones_zapata.h"

int _min(int a, int b)
{
    return (a < b) ? a : b;
}

void testHeader(BmpHeader *cabecera)
{
    printf("Cabecera Tam: %d\n", (int)sizeof(BmpHeader));
    printf("Cabecera firma: %c%c\n", cabecera->firma[0], cabecera->firma[1]);
    printf("Alto: %d\n", cabecera->altura);
    printf("Ancho: %d\n", cabecera->anchura);
    printf("Profundidad: %d\n", cabecera->profundidad);
    printf("Compresion: %d\n", cabecera->compresion);
    printf("Tam Imagen: %d\n", cabecera->tamImagen);
}

// TODO (Santiago#1#05/29/25): pasar los pixels ya reservados, evitar memory leaks

int leerImagen(const char *fileName, PixelRGB **pixels, BmpHeader *imgHeader)
{
    uint32_t i = 0;
    FILE *imgFile = fopen(fileName, "rb");
    if (!imgFile)
    {
        printf("Error al abrir el archivo %s", fileName);
        return 0;
    }

    fread(imgHeader, 54, 1, imgFile);
    if ((imgHeader->firma[0] != 'B' || imgHeader->firma[1] != 'M') || imgHeader->profundidad != 24 || imgHeader->compresion != 0)
    {
        printf("'%s' no es un archivo BMP valido\n", fileName);
        fclose(imgFile);
        return 0;
    }
    int tamFilaConPadding = (int)(4 * ceil((float)(imgHeader->anchura) / 4.0f)) * (imgHeader->profundidad / 8);
    int totalPixels = imgHeader->anchura * imgHeader->altura;
    *pixels = (PixelRGB *)malloc(totalPixels * sizeof(PixelRGB));
    if (*pixels == NULL)
    {
        puts("Error al asignar memoria para cargar imagen\n");
        fclose(imgFile);
        return 0;
    }

    // PixelRGB *currentRowPointer = *pixels+((imgHeader->altura - 1) * imgHeader->anchura);
    PixelRGB *currentRowPointer = *pixels;
    for (i = 0; i < imgHeader->altura; i++)
    {
        fseek(imgFile, imgHeader->dataOffset + (i * tamFilaConPadding), SEEK_SET);
        fread(currentRowPointer, sizeof(PixelRGB), imgHeader->anchura, imgFile);
        currentRowPointer += imgHeader->anchura;
    }
    fclose(imgFile);
    return 1;
}

void WriteImage(const char *fileName, PixelRGB *pixels, BmpHeader imgHeader)
{
    int i;
    FILE *outputFile = fopen(fileName, "wb");
    int paddedRowSize = (int)(4 * ceil((float)(imgHeader.anchura) / 4.0f)) * (imgHeader.profundidad / 8);
    fwrite(&imgHeader, sizeof(BmpHeader), 1, outputFile);
    fseek(outputFile, imgHeader.dataOffset, SEEK_SET);

    char *paddingBytes = (char *)calloc(paddedRowSize, 1);
    for (i = 0; i < imgHeader.altura; i++)
    {
        // int pixelOffset = ((imgHeader.altura - i) - 1) * imgHeader.anchura;  // leemos la imagen tal cual está en el archivo
        memcpy(paddingBytes, &pixels[/*pixelOffset*/ (i * imgHeader.anchura)], imgHeader.anchura * sizeof(PixelRGB));
        fwrite(paddingBytes, 1, paddedRowSize, outputFile);
    }
    free(paddingBytes);
    fclose(outputFile);
}

void espejarVertical(PixelRGB *pixels, const BmpHeader *imgHeader)
{
    // Espejar verticalmente la imagen, guardando las filas en orden inverso
    int i, tamLinea = imgHeader->anchura * sizeof(PixelRGB);
    PixelRGB *tempLinea = (PixelRGB *)malloc(tamLinea);
    if (tempLinea == NULL)
    {
        puts("Error al asignar memoria para el espejado vertical.");
        return;
    }
    for (i = 0; i < imgHeader->altura / 2; i++)
    {
        // Copiar la fila actual al buffer temporal
        memcpy(tempLinea, &pixels[i * imgHeader->anchura], tamLinea);
        // Intercambiar con la fila correspondiente desde el final
        memcpy(&pixels[i * imgHeader->anchura], &pixels[(imgHeader->altura - 1 - i) * imgHeader->anchura], tamLinea);
        memcpy(&pixels[(imgHeader->altura - 1 - i) * imgHeader->anchura], tempLinea, tamLinea);
    }
    free(tempLinea);
}

void espejarHorizontal(PixelRGB *pixels, const BmpHeader *imgHeader)
{
    PixelRGB temp, *inicioFila, *finFila;
    int fila;
    // Método optimizado: intercambio directo de píxeles sin buffer temporal
    for (fila = 0; fila < imgHeader->altura; fila++)
    {
        inicioFila = &pixels[fila * imgHeader->anchura];
        finFila = &pixels[fila * imgHeader->anchura + imgHeader->anchura - 1];
        while (inicioFila < finFila)
        {
            temp = *inicioFila;
            *inicioFila = *finFila;
            *finFila = temp;
            
            inicioFila++;
            finFila--;
        }
    }
}
// *--->               generica                   <---
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]



// *--->               rotacion                  <---
// *--->                                          
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//    [1f][2f][3f][4f][5f][6f][7][6][5][4][3][2][1]
//                                                <---


void convertirEscalaDeGrises(PixelRGB *pixels, const BmpHeader *imgHeader)
{
    int i, gray;
    for (i = 0; i < imgHeader->anchura * imgHeader->altura; i++)
    {
        gray = (int)(0.299 * pixels[i].r + 0.587 * pixels[i].g + 0.114 * pixels[i].b);
        pixels[i].r = gray;
        pixels[i].g = gray;
        pixels[i].b = gray;
    }
}

void cambioTonalidad(PixelRGB *pixels, const BmpHeader *imgHeader, float rojo, float verde, float azul)
{
    for (int i = 0; i < imgHeader->anchura * imgHeader->altura; i++)
    {
        pixels[i].r = _min((int)(pixels[i].r * rojo), 255);
        pixels[i].g = _min((int)(pixels[i].g * verde), 255);
        pixels[i].b = _min((int)(pixels[i].b * azul), 255);
    }
}

int _adjustContrast(uint8_t pixel, float factor)
{
    int newPixel = (int)(factor * (pixel - 128) + 128);
    return newPixel = (newPixel < 0 || newPixel > 255) ? (newPixel < 0 ? 0 : 255) : newPixel;
}

void cambioContraste(PixelRGB *pixels, const BmpHeader *imgHeader, int contraste)
{
    float factor = (259.0 * (contraste + 255.0)) / (255.0 * (259.0 - contraste));
    int i;
    for (i = 0; i < imgHeader->anchura * imgHeader->altura; i++)
    {
        pixels[i].r = _adjustContrast(pixels[i].r, factor);
        pixels[i].g = _adjustContrast(pixels[i].g, factor);
        pixels[i].b = _adjustContrast(pixels[i].b, factor);
    }
}

void cambioNegativo(PixelRGB *pixels, const BmpHeader *imgHeader)
{
    for (int i = 0; i < imgHeader->anchura * imgHeader->altura; i++)
    {
        pixels[i].r = 255 - pixels[i].r;
        pixels[i].g = 255 - pixels[i].g;
        pixels[i].b = 255 - pixels[i].b;
    }
}

void agregarOperacion(OpcionesImagen *opciones, TipoOperacion op, int valor)
{
    // Verificar si la operación ya está agregada
    for (int i = 0; i < opciones->numOperaciones; i++)
    {
        if (opciones->operaciones[i].operacion == op)
        {
            printf("Advertencia: La operacion '%s' ya fue especificada. Se ignora la duplicada.\n", obtenerNombreOperacion(op));
            return;
        }
    }

    if (opciones->numOperaciones < 16)
    {
        opciones->operaciones[opciones->numOperaciones].operacion = op;
        opciones->operaciones[opciones->numOperaciones].valor = valor;
        opciones->operaciones[opciones->numOperaciones].activo = true;
        opciones->numOperaciones++;
    }
    else
    {
        printf("Error: Se alcanzo el maximo numero de operaciones (16).\n");
    }
}

void mostrarAyuda()
{
    printf("=== BMP manipuleitor - Ayuda ===\n\n"
           "Uso: bmpmanipuleitor <imagen_entrada> [imagen_adicional] [opciones]\n\n"
           "Los argumentos pueden ser ingresados en cualquier orden.\n"
           "Archivos:\n"
           "  imagen_entrada     Archivo BMP de entrada (requerido, primer argumento)\n"
           "  imagen_adicional   Segunda imagen BMP para concatenacion (opcional)\n\n"
           "Opciones disponibles:\n"
           "  --escala-de-grises           Convierte a escala de grises\n"
           "  --rotar-derecha              Rota 90 grados a la derecha\n"
           "  --rotar-izquierda            Rota 90 grados a la izquierda\n"
           "  --negativo                   Invierte los colores\n"
           "  --espejar-horizontal         Espeja horizontalmente\n"
           "  --espejar-vertical           Espeja verticalmente\n"
           "  --concatenar-horizontal      Concatena dos imagenes horizontalmente (requiere 2 imagenes)\n"
           "  --concatenar-vertical        Concatena dos imagenes verticalmente (requiere 2 imagenes)\n\n"
           "Opciones con valores (formato: --opcion=valor):\n"
           "  --tonalidad-azul[=%%]        Ajusta tonalidad azul (defecto: 10)\n"
           "  --tonalidad-roja[=%%]        Ajusta tonalidad roja (defecto: 10)\n"
           "  --tonalidad-verde[=%%]       Ajusta tonalidad verde (defecto: 10)\n"
           "  --aumentar-contraste[=%%]    Aumenta el contraste (defecto: 20)\n"
           "  --disminuir-contraste[=%%]   Disminuye el contraste (defecto: 20)\n"
           "  --recortar[=%%]              Recorta la imagen (defecto: 10)\n"
           "  --achicar[=factor]           Achica la imagen (defecto: 2)\n\n"
           "  --help, -h                   Muestra esta ayuda\n\n"
           "Comportamiento:\n"
           "  - Cada operacion genera un archivo separado: LUMEN_<operacion>_<imagen>.bmp\n"
           "  - Las operaciones duplicadas se ejecutan solo una vez\n"
           "  - Los argumentos incorrectos se ignoran con advertencia\n"
           "  - Si una operacion falla, se continua con las demas\n"
           "  - Las operaciones se procesan en el orden especificado\n\n"
           "Ejemplos:\n"
           "  bmpmanipuleitor.exe unlam.bmp --negativo --escala-de-grises\n"
           "    Genera: LUMEN_negativo_unlam.bmp, LUMEN_escala-de-grises_unlam.bmp\n\n"
           "  bmpmanipuleitor.exe imagen.bmp --aumentar-contraste=18 --tonalidad-azul=5\n"
           "    Genera: LUMEN_aumentar-contraste_imagen.bmp, LUMEN_tonalidad-azul_imagen.bmp\n\n"
           "  bmpmanipuleitor.exe img1.bmp img2.bmp --concatenar-vertical\n"
           "    Genera: LUMEN_concatenar-vertical_img1.bmp\n\n");
}

void parse_argv(int argc, char *argv[], OpcionesImagen *opciones)
{
    int valor = -1;
    char *argumentoActual = NULL, *equalPos = NULL;
    for (int i = 1; i < argc; i++)
    {
        argumentoActual = argv[i];

        equalPos = strchr(argumentoActual, '=');
        valor = -1;

        if (equalPos != NULL)
        {
            *equalPos = '\0';
            valor = atoi(equalPos + 1);
        }

        if (!strcmp(argumentoActual, "--escala-de-grises"))
            agregarOperacion(opciones, OP_ESCALA_GRISES, valor);
        else if (!strcmp(argumentoActual, "--rotar-derecha"))
            agregarOperacion(opciones, OP_ROTAR_DERECHA, valor);
        else if (!strcmp(argumentoActual, "--rotar-izquierda"))
            agregarOperacion(opciones, OP_ROTAR_IZQUIERDA, valor);
        else if (!strcmp(argumentoActual, "--tonalidad-azul"))
            agregarOperacion(opciones, OP_TONALIDAD_AZUL, valor == -1 ? 10 : valor);
        else if (!strcmp(argumentoActual, "--tonalidad-roja"))
            agregarOperacion(opciones, OP_TONALIDAD_ROJA, valor == -1 ? 10 : valor);
        else if (!strcmp(argumentoActual, "--tonalidad-verde"))
            agregarOperacion(opciones, OP_TONALIDAD_VERDE, valor == -1 ? 10 : valor);
        else if (!strcmp(argumentoActual, "--aumentar-contraste"))
            agregarOperacion(opciones, OP_AUMENTAR_CONTRASTE, valor == -1 ? 20 : valor);
        else if (!strcmp(argumentoActual, "--disminuir-contraste"))
            agregarOperacion(opciones, OP_DISMINUIR_CONTRASTE, valor == -1 ? 20 : valor);
        else if (!strcmp(argumentoActual, "--recortar"))
            agregarOperacion(opciones, OP_RECORTAR, valor == -1 ? 100 : valor);
        else if (!strcmp(argumentoActual, "--negativo"))
            agregarOperacion(opciones, OP_NEGATIVO, valor);
        else if (!strcmp(argumentoActual, "--achicar"))
            agregarOperacion(opciones, OP_ACHICAR, valor == -1 ? 2 : valor);
        else if (!strcmp(argumentoActual, "--espejar-horizontal"))
            agregarOperacion(opciones, OP_ESPEJAR_HORIZONTAL, valor);
        else if (!strcmp(argumentoActual, "--espejar-vertical"))
            agregarOperacion(opciones, OP_ESPEJAR_VERTICAL, valor);
        else if (!strcmp(argumentoActual, "--concatenar-horizontal"))
            agregarOperacion(opciones, OP_CONCATENAR_HORIZONTAL, valor);
        else if (!strcmp(argumentoActual, "--concatenar-vertical"))
            agregarOperacion(opciones, OP_CONCATENAR_VERTICAL, valor);
        else if (argumentoActual[0] != '-')
        {
            if (opciones->cantImg < 2)
            {
                opciones->imgFiles[opciones->cantImg] = argumentoActual;
                opciones->cantImg++;
            }
            else
                printf("Advertencia: Se ignoro el archivo adicional '%s'. Maximo 2 archivos permitidos.\n", argumentoActual);
        }
        else
            printf("Advertencia: Argumento desconocido '%s' ignorado.\n", argumentoActual);
    }
}

bool requiereSegundaImagen(TipoOperacion operacion)
{
    switch (operacion)
    {
    case OP_CONCATENAR_VERTICAL:
    case OP_CONCATENAR_HORIZONTAL:
        return true;
    default:
        return false;
    }
}

void generarNombreArchivo(const char *archivoOriginal, TipoOperacion operacion, char *nombreSalida)
{
    char nombreBase[256];
    char *soloNombre = NULL, *ultimaBarra = NULL, *ultimoPunto = NULL;
    strcpy(nombreBase, archivoOriginal);
    ultimoPunto = strrchr(nombreBase, '.');
    ultimaBarra = strrchr(nombreBase, '\\');
    if (ultimoPunto != NULL)
        *ultimoPunto = '\0';

    if (ultimaBarra == NULL)
        ultimaBarra = strrchr(nombreBase, '/');

    soloNombre = (ultimaBarra != NULL) ? ultimaBarra + 1 : nombreBase;

    sprintf(nombreSalida, "%s_%s_%s.bmp", NOMBRE_GRUPO, obtenerNombreOperacion(operacion), soloNombre);
}

const char *obtenerNombreOperacion(TipoOperacion operacion)
{
    switch (operacion)
    {
    case OP_ESCALA_GRISES:
        return "escala-de-grises";
    case OP_NEGATIVO:
        return "negativo";
    case OP_TONALIDAD_AZUL:
        return "tonalidad-azul";
    case OP_TONALIDAD_ROJA:
        return "tonalidad-roja";
    case OP_TONALIDAD_VERDE:
        return "tonalidad-verde";
    case OP_AUMENTAR_CONTRASTE:
        return "aumentar-contraste";
    case OP_DISMINUIR_CONTRASTE:
        return "disminuir-contraste";
    case OP_ROTAR_DERECHA:
        return "rotar-derecha";
    case OP_ROTAR_IZQUIERDA:
        return "rotar-izquierda";
    case OP_RECORTAR:
        return "recortar";
    case OP_ACHICAR:
        return "achicar";
    case OP_ESPEJAR_HORIZONTAL:
        return "espejar-horizontal";
    case OP_ESPEJAR_VERTICAL:
        return "espejar-vertical";
    case OP_CONCATENAR_HORIZONTAL:
        return "concatenar-horizontal";
    case OP_CONCATENAR_VERTICAL:
        return "concatenar-vertical";
    default:
        return "desconocida";
    }
}
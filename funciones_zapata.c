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

int guardarImagen(const char *nombreArch, TDA_ImagenBMP *imagen)
{
    int i, padding;
    char paddingBytes[3] = {0};
    FILE *outputFile = fopen(nombreArch, "wb");
    if (!outputFile)
    {
        printf("Error al abrir el archivo %s para escritura.\n", nombreArch);
        return 0;
    }
    fwrite(&imagen->cabecera, sizeof(BmpHeader), 1, outputFile);
    fseek(outputFile, imagen->cabecera.dataOffset, SEEK_SET);
    for (i = 0; i < imagen->cabecera.altura; i++)
    {
        fwrite(imagen->matrizDePixeles->data[i], sizeof(PixelRGB), imagen->cabecera.anchura, outputFile);
        padding = (4 - (imagen->cabecera.anchura * sizeof(PixelRGB) % 4)) % 4;
        if (padding > 0)
            fwrite(paddingBytes, 1, padding, outputFile);
    }
    fclose(outputFile);
    return 1;
}

void espejarVertical(TDA_ImagenBMP *imagen)
{
    int i;
    void *temp;
    for (i = 0; i < imagen->cabecera.altura / 2; i++)
    {
        temp = imagen->matrizDePixeles->data[i];
        imagen->matrizDePixeles->data[i] = imagen->matrizDePixeles->data[imagen->cabecera.altura - 1 - i];
        imagen->matrizDePixeles->data[imagen->cabecera.altura - 1 - i] = temp;
    }
}

void espejarHorizontal(TDA_ImagenBMP *imagen)
{
    int i, j;
    PixelRGB temp, *fila;
    for (i = 0; i < imagen->cabecera.altura; i++)
    {
        fila = imagen->matrizDePixeles->data[i];
        for (j = 0; j < imagen->cabecera.anchura / 2; j++)
        {
            temp = fila[j];
            fila[j] = fila[imagen->cabecera.anchura - 1 - j];
            fila[imagen->cabecera.anchura - 1 - j] = temp;
        }
    }    
}

void rotarDerecha(TDA_ImagenBMP *imagen)
{
    _rotarMatriz(imagen, DERECHA);
}

void rotarIzquierda(TDA_ImagenBMP *imagen)
{
    _rotarMatriz(imagen, IZQUIERDA);
}

void _rotarMatriz(TDA_ImagenBMP *imagen, int angulo)
{
    TDA_ImagenBMP *imagenRotada = NULL;
    int i, j;

    imagenRotada = (TDA_ImagenBMP *)malloc(sizeof(TDA_ImagenBMP));
    if (!imagenRotada)
    {
        printf("Error al reservar memoria para la imagen rotada.\n");
        return;
    }
    imagenRotada->cabecera = imagen->cabecera;
    imagenRotada->cabecera.altura = imagen->cabecera.anchura;
    imagenRotada->cabecera.anchura = imagen->cabecera.altura;

    imagenRotada->matrizDePixeles = (TDA_Matriz *)malloc(sizeof(TDA_Matriz));
    if (!imagenRotada->matrizDePixeles)
    {
        printf("Error al reservar memoria para la matriz de pixeles de la imagen rotada.\n");
        liberarImagenBMP(imagenRotada);
        return;
    }
    if (!crearMatPixeles(imagenRotada->matrizDePixeles, imagenRotada->cabecera.anchura, imagenRotada->cabecera.altura))
    {
        printf("Error al crear la matriz de pixeles para la imagen rotada.\n");
        liberarImagenBMP(imagenRotada);
        return;
    }


    for(i = 0; i < imagen->cabecera.altura; i++)
    {
        for(j = 0; j < imagen->cabecera.anchura; j++)
        {
            if (angulo < 0)
                imagenRotada->matrizDePixeles->data[j][imagen->cabecera.altura - 1 - i] = imagen->matrizDePixeles->data[i][j];
            else
                imagenRotada->matrizDePixeles->data[imagen->cabecera.anchura - 1 - j][i] = imagen->matrizDePixeles->data[i][j];
        }
    }
    imagen->cabecera = imagenRotada->cabecera;
    free(imagen->matrizDePixeles->data);
    imagen->matrizDePixeles = imagenRotada->matrizDePixeles;
    imagenRotada->matrizDePixeles = NULL;
    liberarImagenBMP(imagenRotada);
    imagenRotada = NULL;
    return;
}

void convertirEscalaDeGrises(TDA_ImagenBMP *imagen)
{
    int i, j, gray;
    PixelRGB pixel;
    for (i = 0; i < imagen->cabecera.altura; i++)
        for (j = 0; j < imagen->cabecera.anchura; j++)
        {
            pixel = imagen->matrizDePixeles->data[i][j];
            gray = (pixel.r + pixel.g + pixel.b) / 3;
            imagen->matrizDePixeles->data[i][j].r = gray;
            imagen->matrizDePixeles->data[i][j].g = gray;
            imagen->matrizDePixeles->data[i][j].b = gray;
        }
}

void cambioTonalidad(TDA_ImagenBMP *imagen, float rojo, float verde, float azul)
{
    int i, j;
    for (i = 0; i < imagen->cabecera.altura; i++)
    {
        for (j = 0; j < imagen->cabecera.anchura; j++){
            imagen->matrizDePixeles->data[i][j].r = (uint8_t)_min(255, imagen->matrizDePixeles->data[i][j].r * rojo);
            imagen->matrizDePixeles->data[i][j].g = (uint8_t)_min(255, imagen->matrizDePixeles->data[i][j].g * verde);
            imagen->matrizDePixeles->data[i][j].b = (uint8_t)_min(255, imagen->matrizDePixeles->data[i][j].b * azul);
        }
    }
}

int _adjustContrast(uint8_t pixel, float factor)
{
    int newPixel = (int)(factor * (pixel - 128) + 128);
    return newPixel = (newPixel < 0 || newPixel > 255) ? (newPixel < 0 ? 0 : 255) : newPixel;
}

void cambioContraste(TDA_ImagenBMP *imagen, int contraste)
{
    float factor = (259.0 * (contraste + 255.0)) / (255.0 * (259.0 - contraste));
    int i, j;
    for (i = 0; i < imagen->cabecera.altura; i++)
        for (j = 0; j < imagen->cabecera.anchura; j++)
        {
            imagen->matrizDePixeles->data[i][j].r = _adjustContrast(imagen->matrizDePixeles->data[i][j].r, factor);
            imagen->matrizDePixeles->data[i][j].g = _adjustContrast(imagen->matrizDePixeles->data[i][j].g, factor);
            imagen->matrizDePixeles->data[i][j].b = _adjustContrast(imagen->matrizDePixeles->data[i][j].b, factor);
        }    
}

void cambioNegativo(TDA_ImagenBMP *imagen)
{
    int i, j;
    for (i = 0; i < imagen->cabecera.altura; i++)
        for (j = 0; j < imagen->cabecera.anchura; j++)
        {
            imagen->matrizDePixeles->data[i][j].r = 255 - imagen->matrizDePixeles->data[i][j].r;
            imagen->matrizDePixeles->data[i][j].g = 255 - imagen->matrizDePixeles->data[i][j].g;
            imagen->matrizDePixeles->data[i][j].b = 255 - imagen->matrizDePixeles->data[i][j].b;
        }
}

void imagenTrianguloRGB(TDA_ImagenBMP *imagen, float rojo, float verde, float azul)
{
    int i, j;
    int altura = imagen->cabecera.altura;
    int anchura = imagen->cabecera.anchura;
    PixelRGB pixelNegro = {0, 0, 0};
    for (i = 0; i < altura; i++)
        for (j = 0; j < anchura; j++)
        {
            // diagonales
            if(i == j)
                imagen->matrizDePixeles->data[i][j] = pixelNegro;
            if (anchura - 1 - i == j)
                imagen->matrizDePixeles->data[i][j] = pixelNegro;

            // triangulo izquierdo
            if (i >= j && i + j <= anchura - 1)
                imagen->matrizDePixeles->data[i][j].r = (uint8_t)_min(255, imagen->matrizDePixeles->data[i][j].r * rojo);

            // triangulo derecho
            if (i <= j && i + j <= anchura - 1)
                imagen->matrizDePixeles->data[i][j].g = (uint8_t)_min(255, imagen->matrizDePixeles->data[i][j].g * verde);

            // triangulo inferior
            if (i <= j && i + j >= anchura - 1)
                imagen->matrizDePixeles->data[i][j].b = (uint8_t)_min(255, imagen->matrizDePixeles->data[i][j].b * azul);
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
        opciones->numOperaciones++;
    }
    else
        printf("Error: Se alcanzo el maximo numero de operaciones (16).\n");
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
           "  --concatenar-vertical        Concatena dos imagenes verticalmente (requiere 2 imagenes)\n"
           "  --triangulo-rgb              Aplica filtros de color por regiones triangulares\n"
           "  --comodin        Aplica todas las operaciones especificadas a la misma imagen\n\n"
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
           "  - Si se incluye --comodin, todas las operaciones se aplican en cadena\n"
           "    sobre la misma imagen, y se genera un solo archivo de salida\n"
           "  - Los argumentos incorrectos se ignoran con advertencia\n"
           "  - Si una operacion falla, se continua con las demas\n"
           "  - Las operaciones se procesan en el orden especificado\n\n"
           "Ejemplos:\n"
           "  bmpmanipuleitor.exe unlam.bmp --negativo --escala-de-grises\n"
           "    Genera: LUMEN_negativo_unlam.bmp, LUMEN_escala-de-grises_unlam.bmp\n\n"
           "  bmpmanipuleitor.exe imagen.bmp --aumentar-contraste=18 --tonalidad-azul=5\n"
           "    Genera: LUMEN_aumentar-contraste_imagen.bmp, LUMEN_tonalidad-azul_imagen.bmp\n\n"
           "  bmpmanipuleitor.exe img1.bmp img2.bmp --concatenar-vertical\n"
           "    Genera: LUMEN_concatenar-vertical_img1.bmp\n\n"
           "  bmpmanipuleitor.exe imagen.bmp --comodin --tonalidad-roja=40 --tonalidad-azul=40 --rotar-derecha\n"
           "    Genera: LUMEN_comodin_imagen.bmp con todos los filtros aplicados en cadena\n\n");
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
            agregarOperacion(opciones, OP_RECORTAR, valor == -1 ? 10 : valor);
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
        else if (!strcmp(argumentoActual, "--comodin"))
            agregarOperacion(opciones, OP_FUNCIONALIDAD_EXTRA, valor);
        else if (!strcmp(argumentoActual, "--triangulo-rgb"))
            agregarOperacion(opciones, OP_TRIANGULO_RGB, valor);
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
    case OP_FUNCIONALIDAD_EXTRA:
        return "comodin";
    case OP_TRIANGULO_RGB:
        return "triangulo-rgb";
    default:
        return "desconocida";
    }
}
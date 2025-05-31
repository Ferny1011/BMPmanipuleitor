# BMP Manipulator - Grupo LUMEN

## 📖 Descripción

BMP Manipulator es una herramienta de línea de comandos desarrollada en C para modificar imágenes en formato BMP. Permite aplicar múltiples filtros y transformaciones a las imágenes, generando archivos de salida separados para cada operación.

## Características

- **Múltiples operaciones en una sola ejecución**: Aplica varios filtros simultáneamente
- **Archivos de salida independientes**: Cada operación genera su propio archivo
- **Manejo robusto de errores**: Continúa procesando aunque una operación falle
- **Detección de argumentos duplicados**: Evita procesar la misma operación múltiples veces
- **Soporte para valores personalizados**: Permite ajustar parámetros de los filtros

## Instalación y Compilación

### Prerrequisitos
- GCC (GNU Compiler Collection)
- Git Bash o terminal compatible con bash (para usar el script de compilación)

### Compilación

#### Opción 1: Script de Bash (Recomendado)
```bash
# Compilar solamente
./build.sh

# Compilar y ejecutar
./build.sh run

# Compilar y ejecutar con argumentos
./build.sh run imagen.bmp --escala-de-grises --tonalidad-azul=5
```

#### Opción 2: Compilación Manual
```bash
gcc -Wall -Wextra -std=c99 -g -O2 funciones_grupo.c funciones_zapata.c funciones_quiroga.c -o bin/bmp_manipuleitor.exe -lm
```

## Uso

### Sintaxis Básica
```
bmp_manipuleitor.exe <imagen_entrada> [imagen_adicional] [opciones]
```

### Parámetros
- `imagen_entrada`: Archivo BMP de entrada (requerido)
- `imagen_adicional`: Segunda imagen BMP para operaciones de concatenación (opcional)
- `opciones`: Una o más operaciones a aplicar

### Operaciones Disponibles

#### Operaciones Simples
| Operación | Descripción |
|-----------|-------------|
| `--escala-de-grises` | Convierte la imagen a escala de grises |
| `--negativo` | Invierte los colores de la imagen |
| `--rotar-derecha` | Rota la imagen 90° a la derecha |
| `--rotar-izquierda` | Rota la imagen 90° a la izquierda |
| `--espejar-horizontal` | Espeja la imagen horizontalmente |
| `--espejar-vertical` | Espeja la imagen verticalmente |

#### Operaciones con Valores
| Operación | Sintaxis | Valor por Defecto | Descripción |
|-----------|----------|-------------------|-------------|
| `--tonalidad-azul` | `--tonalidad-azul[=valor]` | 10 | Ajusta la tonalidad azul (%) |
| `--tonalidad-roja` | `--tonalidad-roja[=valor]` | 10 | Ajusta la tonalidad roja (%) |
| `--tonalidad-verde` | `--tonalidad-verde[=valor]` | 10 | Ajusta la tonalidad verde (%) |
| `--aumentar-contraste` | `--aumentar-contraste[=valor]` | 20 | Aumenta el contraste (%) |
| `--disminuir-contraste` | `--disminuir-contraste[=valor]` | 20 | Disminuye el contraste (%) |
| `--recortar` | `--recortar[=valor]` | 10 | Recorta la imagen (%) |
| `--achicar` | `--achicar[=factor]` | 10 | Reduce el tamaño de la imagen (%) |

#### Operaciones de Concatenación
| Operación | Descripción |
|-----------|-------------|
| `--concatenar-horizontal` | Une dos imágenes horizontalmente |
| `--concatenar-vertical` | Une dos imágenes verticalmente |

### Archivos de Salida

Los archivos de salida siguen el formato:
```
LUMEN_<operacion>_<nombre_imagen>.bmp
```

## 📋 Ejemplos de Uso

### Ejemplo 1: Operaciones múltiples
```bash
bmp_manipuleitor.exe unlam.bmp --negativo --escala-de-grises
```
**Genera:**
- `LUMEN_negativo_unlam.bmp`
- `LUMEN_escala-de-grises_unlam.bmp`

### Ejemplo 2: Operaciones con valores personalizados
```bash
bmp_manipuleitor.exe imagen.bmp --aumentar-contraste=18 --tonalidad-azul=5
```
**Genera:**
- `LUMEN_aumentar-contraste_imagen.bmp`
- `LUMEN_tonalidad-azul_imagen.bmp`

### Ejemplo 3: Concatenación de imágenes
```bash
bmp_manipuleitor.exe img1.bmp img2.bmp --concatenar-vertical
```
**Genera:**
- `LUMEN_concatenar-vertical_img1.bmp`

### Ejemplo 4: Comando complejo con argumentos incorrectos
```bash
bmp_manipuleitor.exe unlam.bmp --negativo --escala-de-grises --argumento-incorrecto --aumentar-contraste=18 --negativo
```
**Comportamiento:**
- Ignora `--argumento-incorrecto` con advertencia
- Ignora el segundo `--negativo` (duplicado)
- Genera: `LUMEN_negativo_unlam.bmp`, `LUMEN_escala-de-grises_unlam.bmp`, `LUMEN_aumentar-contraste_unlam.bmp`

## ⚙️ Comportamiento del Sistema

### Manejo de Errores
- **Argumentos incorrectos**: Se ignoran con advertencia en consola
- **Operaciones duplicadas**: Solo se ejecuta la primera ocurrencia
- **Archivos no encontrados**: Se reporta el error y continúa con otras operaciones
- **Operaciones que fallan**: Se reporta el error y continúa con las siguientes

### Limitaciones
- Máximo 16 operaciones por ejecución
- Máximo 2 archivos de imagen de entrada
- Solo soporta formato BMP de 24 bits
- Las operaciones de concatenación requieren exactamente 2 imágenes

## Estructura del Proyecto
```
BMPmanipuleitor/
├── funciones_grupo.c      # Lógica principal y ejecución de operaciones
├── funciones_grupo.h      # Definiciones y estructuras principales
├── funciones_zapata.c     # Funciones del alumno
├── funciones_zapata.h     # Headers del alumno
├── funciones_quiroga.c    # Funciones del alumno
├── funciones_quiroga.h    # Headers del alumno
├── funciones_lazaro.c     # Funciones del alumno
├── funciones_lazaro.h     # Headers del alumno
├── build.sh               # Script de compilación
├── README.md              # Este archivo
├── bin/                   # Ejecutables compilados
└── resources/             # Imágenes de ejemplo
```

## 👥 Autores

**Grupo LUMEN**
- Tópicos de Programación - Universidad Nacional de La Matanza
- Año: 2025

- Este proyecto fue desarrollado como trabajo práctico académico para la materia Tópicos de Programación de la Universidad Nacional de La Matanza.
---

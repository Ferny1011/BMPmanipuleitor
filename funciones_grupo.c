#include "funciones_grupo.h"
#include "funciones_zapata.h"
#include "funciones_quiroga.h"
#include "funciones_lazaro.h"

/**
* Apellido(s), nombre(s): Quiroga, Ferney
* DNI: 95933227
* Entrega: Sí
*
* Apellido(s), nombre(s): Zapata, Santiago
* DNI: 44525943
* Entrega: Sí
*
* Apellido(s), nombre(s): Lazaro, Fernando Ariel
* DNI: 40853793
* Entrega: Sí
*/

int main(int argc, char* argv[]){
    OpcionesImagen opciones;
    opciones.numOperaciones = 0;
    opciones.cantImg = 0;

    if(argc == 1 || (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))) {
        mostrarAyuda();
        return 0;
    }
    parse_argv(argc, argv, &opciones);
    ejecutarOperaciones(&opciones);
    return 0;
}


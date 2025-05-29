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

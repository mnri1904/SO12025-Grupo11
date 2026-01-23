/*
 * ayudas.c
 *
 *  Created on: 22 ene. 2026
 *      Author: Marcelo Romero
 */

#include "shellheader.h"

int ayudaShell (char *ruta) {
	/*
	 * Funcion que se encarga de imprimir archivos de ayuda y consejos
	 * Parametros:
	 * *ruta -> puntero a la cadena de caracteres que contiene la ruta del archivo
	 * Valor de retorno:
	 * 2 (SH_SYSERR) -> error
	 * 0 (SH_OK) -> Exito
	 */
	int nerror;
	FILE *a;
	a = fopen(ruta,"r");
	if (a == NULL) {
		return SH_SYSERR;
	}
	else {
        char buf[4096];
        while (fgets(buf, sizeof(buf), a) != NULL) {
            fputs(buf, stdout);
        }
        if (ferror(a)) {
        	perror("fgets guia.txt");
        	nerror = SH_SYSERR;
        }
        else {
        	nerror = SH_OK;
        }
	}
	fclose(a);
	return nerror;
}

int ayudaImpresion(unsigned int desde, unsigned int hasta) {
	/*
	 * Funcion que se encarga de imprimir el archivo guia.txt en un rango de lineas
	 * Parametros:
	 * *f -> puntero al archivo
	 * desde -> limite superior del archivo
	 * hasta -> limite inferior del archivo
	 */

	FILE *f;
	f = fopen("src/guia.txt", "r");

    if (!f) {
    	return -1;
    }
    if (desde < 1 || hasta < 1 || desde > hasta) {
        errno = EINVAL;
        return -1;
    }

    char buf[TAMBUFF];
    long linea = 0;

    while (fgets(buf, sizeof(buf), f) != NULL) {
        linea++;

        if (linea >= desde && linea <= hasta) {
            fputs(buf, stdout);
        }

        if (linea > hasta) {  // ya pasamos el rango, cortamos
            break;
        }
    }

    if (ferror(f)) {
    	return -1;
    }else {
    	return 0;
    }

}













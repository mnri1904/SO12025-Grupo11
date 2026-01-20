/*
 ============================================================================
 Name        : shell.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shellheader.h"

static int esLineaVacia(const char *linea) {
	/*
	 * Funcion que se encarga de detectar si una linea introducida por el usuario esta vacia
	 * Parametros
	 * *linea -> cadena de caracteres que almacena lo introducido por el usuario
	 * Valor de retorno:
	 * 0 -> si no esta vacia
	 * 1 -> si esta vacia
	 */
    while (*linea) {
        if (*linea != ' ' && *linea != '\t') return 0;
        linea++;
    }
    return 1;
}

int main() {
	int nerror = SH_OK;		// variable para distinguir errores por numeros (codigos) distintos
	while(1) {
		fprintf(stdout, "shell:~$ ");
		fflush(stdout);

		char *prompt = leerLinea();
		if (prompt == NULL) {
			break;
		}

        if (esLineaVacia(prompt)) {
            free(prompt);
            continue;
        }

		char *duplicado = strdup(prompt);		// se duplica prompt para poder modificarlo
		if (!duplicado) {
			perror("strdup");
			nerror = SH_SYSERR;
			free(prompt);
			continue;
		}

		char *cadena = NULL;									// puntero a la cadena que contendra lo que falta por separar (parametro de strtok_r)
		char *comando = strtok_r(duplicado, " \t", &cadena);

		if (comando == NULL) {									// si el usuario introdujo un espacio vacio
			nerror = SH_OK;
			free(duplicado);									// liberamos la memoria reservada
			free(prompt);
			continue;
		}

		else if ((strcmp(comando, "ls") == 0)) {
			nerror = ejecLs(prompt);
		}
		else if ((strcmp(comando, "cp") == 0)) {
			nerror = ejecCp(prompt);
		}
		else {
			fprintf(stderr, "ERROR. Comando no encontrado: %s\n", comando);
			nerror = SH_NOTFOUND;
		}
        free (duplicado);
        free (prompt);
	}
	return nerror;
}










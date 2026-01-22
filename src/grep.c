/*
 * grep.c
 *
 *  Created on: 21 ene. 2026
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include "shellheader.h"

char *patron;			// puntero a la cadena de caracteres que contendra el patron a buscar
char *arch;				// puntero a la cadena de caracteres que contendra la ruta del archivo

int contarEspacios (char *cadena) {
	/*
	 * Funcion que se encarga de contar si una cadena de caracteres tiene mas de un espacio en blanco para diferenciar de: grep "" archivo / grep " " archivo / grep "       " archivo
	 * Parametros:
	 * *cadena -> puntero a la cadena a evaluar
	 * Valor de retorno:
	 * contador -> espacios en blanco contados
	 */
	int i, contador = 0;
	for (i = 0; i < strlen(cadena); i++) {
		if (cadena[i] == ' ' || cadena[i] == '\t' ){
			contador++;
		}
	}
	return contador;
}

int parsingGrep(char *linea) {
	char *p = linea;					// puntero que apunta a linea
	patron = NULL;
	arch = NULL;

	int contadorComillas = 0;

	for (; *p != '\0' ; p++){			// incrementamos el puntero hasta encontrar una comilla doble
		if (*p == '"') {
			contadorComillas++;
		}
	}

	if (contadorComillas == 2) {		// si hay dos comillas dobles -> verificar si entre las comillas hay un espacio en blanco, varios, o un patron

		p = linea;						// reseteamos p

		for (; *p != '"' && *p != '\0'; p++);		// avanzamos el puntero hasta la primera comilla

		p++;										// incrementamos p para apuntar al caracter siguiente a la primera comilla

		patron = p;									// hacemos que el vector del patron apunte a lo mismo que p

		for (; *p != '"' && *p != '\0'; p++);		// buscamos la segunda comilla
		if (*p == '\0') {
			fprintf(stderr, "ERROR. Modo de empleo: grep [OPCIÓN]... PATRONES [FICHERO]...");
			return SH_USAGE;
		}

		*p= '\0';									// finalizamos la cadena en el lugar de la segunda comilla

		p++;										// p apunta al caracter siguiente a la ultima comilla doble

		for (; *p == ' ' || *p == '\t'; p++);		// skipeamos los espacios para buscar el nombre del archivo en la linea

		if (*p == '\0') {							// si se escribe grep "" se lee de stdin y se escribe en stdout
			return SH_OK;
		}

		arch = p;									// p avanzo hasta la primera letra que corresponderia al nombre del archivo

		for (; *p != '\0' && *p != ' ' && *p != '\t'; p++);		// p avanza para leer todo el nombre del archivo
		if (*p != '\0') {
			*p = '\0';											// finalizamos la cadena para tener el nombre/ubicacion del archivo
		}
		return SH_OK;

	}
	else if (contadorComillas ==  0) {			// si no hay comillas -> grep "patron" archivo
		char *cadena = NULL;
		char *token = strtok_r(linea, " \t", &cadena);					// grep
		if (token == NULL) {
			fprintf(stderr, "ERROR. Modo de empleo: grep [OPCIÓN]... PATRONES [FICHERO]...");
			return SH_USAGE;
		}

		token = strtok_r(NULL, " \t", &cadena);							// patron
		if (token == NULL) {
			fprintf(stderr, "ERROR. Modo de empleo: grep [OPCIÓN]... PATRONES [FICHERO]...");
			return SH_USAGE;
		}

		patron = token;													// hacemos que patron apunte al token porque lo que se va a leer es stdin

		token = strtok_r(NULL, " \t", &cadena);							// el nombre/ubicacion del archivo (no es necesario)
		arch = token;
		if (token != NULL) {
			arch = token;												// hacemos que arch apunte al nombre/ubicacion del archivo
		}
		return SH_OK;

	}else {
		fprintf(stderr, "ERROR. Modo de empleo: grep [OPCIÓN]... PATRONES [FICHERO]...");
		return SH_USAGE;												// error de uso si hay una sola comilla o mas de dos
	}
}

int ejecGrep (char *prompt) {
	int nerror = parsingGrep(prompt);
    if (nerror != SH_OK){
    	return nerror;
    }
    if (patron == NULL) return SH_USAGE;  // no hay patrón => uso inválido

    FILE *a = stdin;
    int fd = -1;

    if (arch != NULL) {
        fd = open(arch, O_RDONLY);
        if (fd == -1) { perror("open"); return SH_SYSERR; }

        a = fdopen(fd, "r");
        if (a == NULL) {
        	perror("fdopen");
        	close(fd);
        	return SH_SYSERR;
        }
    }

    char *linea = NULL;
    size_t tam = 0;
    ssize_t n;

    while ((n = getline(&linea, &tam, a)) != -1) {
        if (strstr(linea, patron) != NULL) {
            fputs(linea, stdout);
        }
    }

    free(linea);
    if (arch != NULL) fclose(a);

    return SH_OK;
}



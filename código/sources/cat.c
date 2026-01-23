/*
 * cat.c
 *
 *  Created on: 20 ene. 2026
 *      Author: Marcelo Romero
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "shellheader.h"

/*
 * Usos: cat o cat - (escribe stdin hasta EOF)
 * cat - archivo	(escribe stdin en stdout hasta EOF y escribe archivo en stdout)
 * cat archivo1 archivo2 ...	(escribe archivo1 ... en stdout)
 */

int parsingCat (char *linea) {
	/*
	 * Funcion que se encarga de parsear la linea si se ejecuta cat, ademas, dependiendo el uso, abre los archivos y muestra en pantalla el correspondiente
	 * Parametros:
	 * *linea -> puntero a la cadena de caracteres que contiene el prompt del usuario
	 * *salida -> puntero a la cadena de caracteres correspondiente al archivo de salida que se mostrara en pantalla
	 * *flag -> puntero a un numero usado como bandera para determinar si se uso cat con un - como argumento
	 * Valor de retorno:
	 * *salida -> puntero a la cadena de caracteres con el nombre del archivo de salida
	 *
	 */
	int banderaArgs = 0;							// variable para contar la cantidad de argumentos recibidos

	char *cadena = NULL;
	char *token = strtok_r(linea, " \t", &cadena);		// token = "cat"

	if(token == NULL) {							// si se escribio solamente cat
		return SH_OK;
	}

	while((token = strtok_r(NULL, " \t", &cadena)) != NULL ) {	// NULL

		banderaArgs = 1;					// si entramos al while hay al menos un argumento

		if (strcmp(token, "-") == 0) {			// si se escribio cat - se escribe stdin en stdout, si se escribio cat token es NULL dentro del while

			if (loopLectEscr(STDIN_FILENO, STDOUT_FILENO) != 0) {	// usamos la misma funcion para lectura/escritura de cp

				return SH_SYSERR;
			}
		}
		else {														// si se escribio cat archivo1 ...
			int fd = open(token, O_RDONLY);							// file descriptor del token, o sea del archivo que se va a escribir en pantalla
			if (fd == -1) {
				perror("open");										// error al abrir el archivo
				return SH_SYSERR;
			}
			int copiadoCorrecto = loopLectEscr(fd, STDOUT_FILENO);
			close (fd);
			if (copiadoCorrecto != 0) {
				return SH_SYSERR;
			}
		}
	}
	if (banderaArgs == 0) {
		if (loopLectEscr(STDIN_FILENO,STDOUT_FILENO) != 0) {
			return SH_SYSERR;
		}
	}
	return SH_OK;
}

int ejecCat(char *prompt) {
	int n;
	n = parsingCat(prompt);
	return n;
}

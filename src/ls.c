/*
 * ls.c
 *
 *  Created on: 17 ene. 2026
 *      Author: Marcelo Romero
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "shellheader.h"

/*
============================================================================
 DECLARACION DE VARIABLES GLOBALES
 ===========================================================================
 */
	/*
	 * Valores de error
	 * 0 -> no hay error, el while sigue
	 * 1 -> error de abrir/cerrar directorio, el while sigue
	 * (poner un numero para exit o crear otra variable)
	 *
	 */
char ls[] = "ls";
char *ruta = NULL;
char *leerLinea () {
	/*
	 * Funcion que lee la linea escrita por el usuario
	 * Valor de retorno: La linea escrita por el usuario
	 */
	char *linea = NULL; // se declara un puntero a char que apunta a NULL para que getline tenga un buffer
	size_t tam = 0;		// tamano del buffer que getline actualiza

	ssize_t n = getline(&linea, &tam, stdin);
	if (n == -1) { // getline retorna -1 si ocurrio un error o EOF
		free(linea);
		return NULL;
	}

	// si la linea termina en \n se reemplaza por \0
	if (n > 0 && linea[n-1] == '\n') {
		linea[n-1] = '\0';
	}
	return linea;
}

int parsingLs (char *linea) {

	/*
	 * Funcion que sirve para separar el prompt del usuario
	 * Parametros:
	 * *linea -> puntero a la cadena de caracteres que representa la entrada del usuario
	 * Valor de retorno
	 * hayArg -> bandera para distinguir si hay argumentos o no
	 * -1 -> si lo que almacena el token la primera vez es nulo
	 */
	ruta = ".";					// inicializamos ruta en . para considerar que la ruta es el directorio actual en caso que el usuario no haya especificado
	int hayArg = 0;

	char *cadena = NULL; 								// inicializamos la cadena que guardara el tercer parametro de strtok_r
	char *token = strtok_r(linea, " \t", &cadena);

	if (token == NULL) {
		return -1;
	}
	while  ((token = strtok_r(NULL, " \t", &cadena)) != NULL) {	// 1er parametro NULL para que no se reinicie el proceso

		if (token[0] == '-') {		// si el token actual es un argumento...
			if (*(token+1) == 'a') {	// si el argumento es -a (unico implementado)
				hayArg = 1;
			}
		}
		else {											// si token[0] no es un argumento, entonces es una ruta
			ruta = token;								// hacemos que ruta apunte a token
		}
	}
	return hayArg;
}

char *detectarComando (char *prompt) {
	/*
	 * Funcion que detecta el comando introducido por el usuario
	 * Parametros:
	 * *prompt -> cadena de caracteres que contiene lo que introdujo el usuario
	 * Valor de retorno
	 * *com -> cadena de caracteres con el comando entresacado
	 */

	char *com = prompt;

	int i = 0;
	if (com[i] == ' ' || com[i] == '\0') { // si escribe un espacio al comienzo o no se escribe nada
		printf("ERROR. Comando no existente\n");
		com[0] = '\0';						// asignamos null al primer elemento de la cadena para finalizarla
		return com;							// para salir antes de la funcion
	}
	for (; prompt[i] != ' ' ; i++) {
		com[i] = prompt[i];
	}
	com[i] = '\0';

	return com;
}

int ejecLs (char *linea) {

	int modo = 0;				// variable para definir el comportamiento de ls (si hay o no argumentos)
	/*
	while (errorNum == 0 || errorNum == 1) {
		printf("shell:~$ ");
		char *prompt = leerLinea();

		if (prompt == NULL) {
			break;
		}
		*/

	modo = parsingLs(linea);
	DIR *directorio;
	struct dirent *entrada;

	if (modo != -1) {
		directorio = opendir(ruta);
		if (directorio == NULL) {
			puts("Error al abrir en el archivo\n");
			return 1;
		}
		if (modo == 1) {				// si hay -a imprimir todos los elementos del directorio incluidos los ocultos
			while ((entrada = readdir(directorio)) != NULL) {
				printf("%s\n", entrada -> d_name);
			}
		}
		else {												// si no hay -a imprimir todos los elementos a excepcion de los ocultos
			while ((entrada = readdir(directorio)) != NULL) {
				if (entrada -> d_name[0] != '.') {			// si el nombre de lo apuntado por entrada es distinto de . (o sea no es un archivo oculto)
					printf("%s\n", entrada -> d_name);
				}
			}
		}
		if (closedir(directorio) == -1) {
			puts ("Error al cerrar el archivo\n");
			return 1;
		}
	}
	return 0;
}




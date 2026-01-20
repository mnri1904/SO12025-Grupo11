/*
 * cp.c
 *
 *  Created on: 19 ene. 2026
 *      Author: Marcelo Romero
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include "shellheader.h"
#define TAMBUFF 8192

char *fuente;							// puntero que contiene el nombre del archivo fuente
char *destino;							// puntero que contiene el nombre del archivo de destino

int parsingCp(char *linea, int *arg) {
	/*
	 * Funcion que se encarga de parsear el comando cp dividiendo por los parametros (solo se permiten dos)
	 * Parametros:
	 * *linea -> puntero a la cadena de caracteres que almacena lo introducido por el usuario
	 * *arg -> puntero al entero para saber si hay un argumento (v el unico soportado)
	 * Valor de retorno: la cantidad de parametros utilizados por el usuario
	 * -1 -> si lo que almacena el token es nulo
	 * 0 -> no hay parametros (error)
	 * 1 -> hay insuficientes parametros (error)
	 * 2 -> no hay error
	 * > 2 -> hay demasiados parametros
	 * Formato: cp fuente destino
	 *
	 */
	fuente = NULL;
	destino = NULL;

	int contador = 0;								// variable para contar la cantidad de parametros
	char *cadena = NULL;								// variable que almacena el resto de la linea spliteada por strtok_r

	char *token = strtok_r(linea, " \t", &cadena);
	//en este momento token[] = "cp" en teoria
	if (token == NULL) {
		return -1;
	}
	while ((token = strtok_r(NULL, " \t",&cadena)) != NULL) {

		if (token[0] == '-') {
			contador -= 1;
			if (*(token +1) == 'v') {
				*arg = 1;
			}
			else {
				printf("ERROR. Opcion no sportada\n");
				return 0;							// si el argumento es distinto de v consideramos que no hubieron parametros y todo el comando fue erroneo
			}
		}

		contador++;									//cada vez que se entre al while el contador se suma



		// 1ra llamada dentro del while: token[] = "fuente"
		if (contador == 1) {
			fuente = token;			// hacemos que fuente apunte al token
		}

		// 2da llamada dentro del while: token[] = "destino"
		else if (contador == 2){
			destino = token;		// hacemos que destino apunte al token
		}
	}
	return contador;
}

int verificarExistencia(char *ruta) {
	/*
	 * Funcion que se encarga de verificar el estado de las rutas brindadas por el usuario
	 * Parametros:
	 * *ruta -> puntero a la cadena de caracteres que corresponde a la ruta del archivo
	 * Valor de retorno:
	 * 0 -> si la ruta existe y es un archivo regular
	 * 1 -> si la ruta existe y es un directorio
	 * -1 -> si la ruta no existe
	 * -2 -> si el archivo existe pero no se puede acceder
	 * 2 -> si el archivo no es directorio ni archivo regular
	 * Obs: para fuente debe retornar 0, para destino, si retorna 1 crear archivo dentro del directorio especificado (comportamiento de cp en Linux)
	 *
	 */

	/*Verificamos la existencia de lo que hay en la ruta*/
	int existeRuta;				// 0 si si, -1 si no
	struct stat stRuta;			// struct que tiene los metadatos de un archivo
	existeRuta = stat(ruta, &stRuta);
	if (existeRuta == -1) {
		if (errno == ENOENT) return -1;
		return -2;
	}
	else {
		/*Verificamos si lo que hay en la ruta es un directorio*/
		if (S_ISDIR(stRuta.st_mode)) return 1;
		if (S_ISREG(stRuta.st_mode)) return 0;
		return 2;
	}
}

int verificarBarraFinal(char *cadena) {
	/*
	 * Funcion que sirve para verificar si el destino dado por el usuario termina en '/'
	 * Parametros
	 * *cadena -> cadena de destino
	 * Valor de retorno
	 * 0 -> si el destino no termina en '/'
	 * 1 -> si el destino termina en '/'
	 *
	 */
	for (; *cadena != '\0'; cadena++); // avanzamos el puntero hasta llegar a null
	if (*(cadena - 1) == '/') {
		return 1;
	}
	else {
		return 0;
	}
}

char *rutaFinal (char *src, char*dst) {
	/*
	 * Funcion que se encarga de crear un vector nuevo compuesto por la ruta del directorio y el archivo fuente en el caso de que destino sea un directorio
	 * Ej de uso: cp fuente /dir/subdir -> rutaFinal = /dir/subdir/fuente
	 * Parametros:
	 * *src -> puntero a la cadena de caracteres que contiene el nombre del archivo fuente
	 * *dst -> puntero a la cadena de caracteres que contiene la ruta del directorio de destino
	 * Valor de retorno
	 * rutaFinal -> puntero a la cadena de caracteres creada
	 *
	 */
	char *archivo = NULL;										// *archivo es un puntero al nombre del archivo. Ej: fuente[]="/dir/subdir/archivo.txt -> *archivo = "archivo.txt" es decir barra+1

	char *barra = strrchr(src, '/');							//  *barra es un puntero a la ultima '/' de la cadena fuente. Ej: fuente[]="/dir/subdir/archivo.txt -> barra[] = "/archivo.txt" y *barra = "/"

	if (barra == NULL) {										// si la cadena de fuente no lleva ninguna '/' quiere decir que directamente se hizo uso del archivo
		archivo = src;
	}
	else {														// si la cadena fuente lleva alguna '/' quiere deicr que se paso la ruta de ubicacion del archivo
		archivo = barra + 1;
	}

																// la razon de uso de esta variable es evitar que en el vector final se copien dos barras. Ej: cp /dir/subdir/archivo.txt /dir2/subdir2/

	char *rutaFinal = NULL;
	size_t tamFinal;											// variable para almacenar el tamano que tendra la ruta final

	if (!(verificarBarraFinal(dst) == 1)) {					// si el destino tiene la forma /dir/subdir
		tamFinal = strlen(dst) + 1 + strlen(archivo) + 1; 		// sumamos el tamano de la cadena de destino + 1 (para '/') + tamano de la cadena fuente + 1 (para '/0')
		rutaFinal = malloc(tamFinal);							// reservamos memoria dinamica para tener la cantidad exacta que necesita la ruta final
		if (rutaFinal != NULL) {
			snprintf(rutaFinal, tamFinal, "%s/%s", dst,archivo);	// copiamos en el vector nuevo el contenido de destino y archivo
		}
	}
	else {														// si el destino tiene la forma /dir/subdir/
		tamFinal = strlen(dst) + strlen(archivo) + 1;			// el tamano del vector nuevo debera ser el tamano del destino + el tam del vector restante desde la ultima '/' dentro del vector fuente + 1 para '\0'
		rutaFinal = malloc(tamFinal);
		if (rutaFinal != NULL) {
			snprintf(rutaFinal, tamFinal, "%s%s", dst,archivo);	// copiamos en el vector nuevo el contenido de destino y archivo
		}
	}
	return rutaFinal;
}

int loopLectEscr(int fd_entrada, int fd_salida) {
	/*
	 * Funcion que controla el loop de lectura y escritura
	 * Parametros":
	 * fd_entrada -> file descriptor del archivo de entrada/lectura
	 * fd_salida -> file descriptor del archivo de salida/escritura
	 * Valor de retorno:
	 * -1 -> si ocurrio un error intesperado durante el loop de lectura o escritura (perror dice en cual proceso ocurrio el error)
	 * 0 -> si no ocurrieron errores y el archivo se copio correctamente
	 *
	 */

	ssize_t bytesLeidos;							// variable para contar los bytes leidos de fuente
	char buffer[TAMBUFF];							// declaramos un vector que actua como buffer

	while ((bytesLeidos = read(fd_entrada, buffer, sizeof(buffer))) > 0) {	// mientras el archivo se siga leyendo

		ssize_t bytesEscritos = 0;						// variable para contar los bytes escritos

		ssize_t bytesAcumulados = 0;

		while (bytesEscritos < bytesLeidos) {		// mientras la cantidad que se escriba sea menor o igual que la que se lee vamos a seguir escribiendo

			bytesAcumulados = write(fd_salida, buffer + bytesEscritos, bytesLeidos - bytesEscritos);
				/*
				 * Explicacion: como write puede parar antes de escribir bytesLeidos, buffer siempre apuntara a la ultima posicion que se escribio, por eso buffer + bytesEscritos
				 * Entonces, siempre que el loop pare de escribir antes de llegar a la cantidad de bytesLeidos, faltaran por escribir bytesLeidos - bytesEscritos bytes
				 *
				 */
			if (bytesAcumulados == -1) {				// si hubo un error durante el loop
				perror("write");
				return -1;
			}
			bytesEscritos += bytesAcumulados;			// actualizamos la cantidad de bytesEscritos
		}
	}
	if (bytesLeidos == -1) {						// si hubo un error durante la lectura del archivo fuente
		perror("read");
		return -1;
	}
	return 0;
}

const char *tipoArchivo(mode_t m) {
	/*
	 * Funcion que devuelve el tipo de archivo que no es posible acceder
	 * Parametros:
	 * mode_t m -> puntero al campo st.mode del struct st
	 * Valor de retorno:
	 * Cadena de caracteres constante
	 */
    if (S_ISREG(m))  return "archivo regular";
    if (S_ISDIR(m))  return "directorio";
    if (S_ISCHR(m))  return "dispositivo de caracter";
    if (S_ISBLK(m))  return "dispositivo de bloque";
    if (S_ISFIFO(m)) return "FIFO/pipe";
    if (S_ISSOCK(m)) return "socket";
    return "tipo desconocido";
}


int ejecCp(char *prompt) {

	/*
	 * Funcion que se encarga de la ejecucion de cp
	 * Parametros:
	 * *prompt -> cadena de caracteres introducida por el usuario
	 * Valor de retorno:
	 * 0 -> exito o linea vacia
	 * 1 -> error de uso de parametros
	 * 2 -> error de sistema/archivos
	 * 127 -> comando no encontrado
	 *
	 */


	int validador = 0; 	// variable para validar el uso de cp (solo puede ser 2)
	int verbose = 0;			// entero para indicar si el usuario uso -v (unico argumento soportado)
	validador = parsingCp(prompt, &verbose);

	if (validador == -1){				// linea vacia/token NULL pasamos
		return SH_OK;
	}
	else if (validador < 2){
		printf("Error. Cantidad de parametros insuficientes(%d)\n", validador);
		return SH_USAGE;
	}
	else if(validador > 2) {
		printf("Error. Demasiados parametros (%d)\n", validador);
		return SH_USAGE;

	}
	else {
		/*si la cantidad de parametros recibidos es 2 continuamos con el proceso*/

		int veriFuente;			// variable para verificar si el archivo FUENTE EXISTE y ES un ARCHIVO REGULAR

		veriFuente = verificarExistencia(fuente);

		if (veriFuente == 0) {

			/*Si se cumple la condicion, verificamos que DESTINO EXISTA y que tipo es*/

			int fd_in = open(fuente, O_RDONLY);
			if (fd_in == -1) {
				fprintf(stderr, "ERROR. No se pudo abrir el archivo %s\n", fuente);
				perror("open");
				return SH_SYSERR;
			}

			int veriDestino;	// variable para verificar si el archivo DESTINO EXISTE y de que tipo es

			veriDestino = verificarExistencia(destino);

			if (veriDestino == 1) {

				/*Si DESTINO EXISTE y ademas es un DIRECTORIO debemos crear la nueva ruta*/

				char *rutaNueva = NULL;
				rutaNueva = rutaFinal(fuente, destino);

				if (rutaNueva == NULL) {
					close(fd_in);
					return SH_SYSERR;
				}

				/*Intentamos abrir el archivo*/

				int fd_out = open(rutaNueva, O_WRONLY | O_CREAT | O_TRUNC, 0644);

				if (fd_out == -1) {
					fprintf(stderr, "ERROR. No se pudo abrir el archivo %s\n", rutaNueva);
					perror("open");
					close(fd_in);
					free(rutaNueva);
					return SH_SYSERR;
				}
				else {

					/*Si el archivo abre podemos leer y escrbir*/

					int copiadoCorrecto = loopLectEscr(fd_in, fd_out);

					if (copiadoCorrecto == 0) {
						if (verbose == 1) {
							printf("'%s' -> '%s'\n",fuente,destino);
						}
						close (fd_in);
						close(fd_out);
						free(rutaNueva);
						return SH_OK;
					}
					else {
						close (fd_in);
						close(fd_out);
						free(rutaNueva);
						return SH_SYSERR;
					}

				}


			}
			else if (veriDestino == 0) {

				/*Si DESTINO EXISTE y ademas ES UN ARCHIVO REGULAR*/

				int fd_out = open(destino, O_WRONLY | O_CREAT | O_TRUNC, 0644);
				if (fd_out == -1) {
					fprintf(stderr, "ERROR. No se pudo abrir el archivo %s\n", destino);
					perror("open");
					close(fd_in);
					return SH_SYSERR;
				}
				else {
					int copiadoCorrecto = loopLectEscr(fd_in, fd_out);
					if (copiadoCorrecto == 0) {
						if (verbose == 1) {
							printf("'%s' -> '%s'\n",fuente,destino);
						}
						close(fd_in);
						close(fd_out);
						return SH_OK;
					}
					else {
						close(fd_in);
						close(fd_out);
						return SH_SYSERR;
					}
				}
			}
			else if (veriDestino == -1) {

				/*Si DESTINO NO EXISTE intentamos CREARLO*/

				int fd_out = open(destino, O_WRONLY | O_CREAT | O_TRUNC, 0644); // O_CREAT crea el archivo
				if (fd_out == -1) {
					fprintf(stderr, "ERROR. No se pudo crear/abrir el archivo %s\n", destino);
					perror("open");
					close(fd_in);
					return SH_SYSERR;
				}
				int copiadoCorrecto = loopLectEscr(fd_in, fd_out);
				if (copiadoCorrecto == 0) {
					if (verbose == 1) {
						printf("'%s' -> '%s'\n",fuente,destino);
					}
					close(fd_out);
					close(fd_in);
					return SH_OK;
				}
				else {
					close(fd_out);
					close(fd_in);
					return SH_SYSERR;
				}
			}
			else if (veriDestino == -2) {

				/*Si DESTINO EXISTE pero NO SE PUEDE ACCEDER*/

				fprintf(stderr, "ERROR. No se puede acceder al destino %s\n", destino);
				close(fd_in);
				return SH_SYSERR;
			}
			else {

				/*DESTINO NO ES un ARCHIVO REGULAR ni DIRECTORIO*/

				struct stat st;
				if (stat(destino, &st) == -1) {
					fprintf(stderr, "ERROR. No se pudo inspeccionar el destino %s\n", destino);
					perror("stat");
				}
				else {
					fprintf(stderr, "ERROR. Destino %s no soportado: es %s\n",
					destino, tipoArchivo(st.st_mode));
		    	}
					    	close(fd_in);
					    return SH_SYSERR;
			}
		}
		else if (veriFuente == 1) {

			/*Si FUENTE es un DIRECTORIO: ERROR*/

			fprintf(stderr, "ERROR. El archivo %s proporcionado como fuente es un directorio, no un archivo\n",fuente);
			return SH_USAGE;
		}

		else if (veriFuente == -1) {

			/*Si FUENTE NO EXISTE: ERROR*/

			fprintf(stderr, "ERROR. El archivo %s proporcionado como fuente no existe\n",fuente);
			return SH_USAGE;
		}

		else if (veriFuente == -2) {

			/*Si FUENTE EXISTE pero NO SE PUEDE ACCEDER: ERROR*/

			fprintf(stderr, "ERROR. No se puede acceder al archivo %s proporcionado como fuente\n",fuente);
			return SH_SYSERR;
		}
		else {

			/*Si FUENTE NO es DIRECTORIO ni ARCHIVO REGULAR: ERROR*/

			struct stat st;
			if (stat(fuente, &st) == -1) {
				fprintf(stderr, "ERROR. No se pudo inspeccionar la fuente %s\n", fuente);
				perror("stat");
			} else {
				fprintf(stderr, "ERROR. Fuente %s no soportada: es %s\n", fuente, tipoArchivo(st.st_mode));
			}
			return SH_SYSERR;
		}
	}
	return SH_OK;
}


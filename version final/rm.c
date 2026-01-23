/*
 * rm.c
 *
 *  Created on: 22 ene. 2026
 *      Author: lp1
 */

#include "shellheader.h"

char *rm_ruta = NULL;

int parsingRm(char *linea, int *arg) {
	/*
	 * Funcion que se encarga de parsear la linea cuando se ejecute rm
	 * Parametros:
	 * *linea -> puntero a la cadena de caracteres
	 * *arg -> puntero a la bandera para indicar si se usa verbose
	 * Valor de retorno:
	 * 0 (SH_OK_) -> No hay errores
	 * 1 (SH_USAGE) -> error de uso
	 */
    char *cadena = NULL;
    int contador = 0;									// contador de argumentos
    char *token = strtok_r(linea, " \t", &cadena);   // "rm"
    if (token == NULL) {
    	return SH_USAGE;
    }

    while ((token = strtok_r(NULL, " \t", &cadena)) != NULL) {

		if (token[0] == '?' && token[1] == '\0') {
			int n = ayudaImpresion(72,86);
			if (n != 0) {
				return SH_SYSERR;
			}
			return -2;
		}

    	contador++;
    	if (token[0] == '-') {
    		if (*(token + 1) == 'v' && token[2] == '\0') {

    			if (rm_ruta != NULL) {
    				fprintf(stderr, "ERROR. %s es un directorio. Si necesitas ayuda ejecuta rm ?\n", rm_ruta);
    				return SH_USAGE;
    			}
    			*arg = 1;
    			continue;						// no queremos que este token se almacene como ruta
    		}
    		else {
    			fprintf(stderr, "ERROR. %s es un directorio. Si necesitas ayuda ejecuta rm ?\n", rm_ruta);
    			return SH_USAGE;				// error de uso (solo es soportado -v)
    		}
    	}

    	if (rm_ruta == NULL) {		// si en este punto no hay ruta debemos guardar el token
    		rm_ruta = token;
    	}
    	else {
    		fprintf(stderr, "ERROR. %s es un directorio. Si necesitas ayuda ejecuta rm ?\n", rm_ruta);
    		return SH_USAGE;		// error de uso si hay mas de una ruta
    	}
    }
    if (rm_ruta == NULL) {
    	fprintf(stderr, "ERROR. %s es un directorio. Si necesitas ayuda ejecuta rm ?\n", rm_ruta);
    	return SH_USAGE;			// si se escribe rm o rm -v sin archivo
    }
    return SH_OK;
}

void log_sistema_error_min(char *msg) {
	/*
	 * Funcion que escribe en el archivo ubicado en /var/log/shell/sistema_error.log
	 * Parametros:
	 * *msg -> cadena de caracteres que contiene el mensaje que se va a escribir en el archivo
	 */
    FILE *f = fopen("/var/log/shell/sistema_error.log", "a");	// va a fallar si no se es usuario root
    if (!f) {
    	fprintf(stderr, "ERROR. No eres usuario root o no tienes permisos suficientes. Si necesitas ayuda ejecuta rm ?");
    	return; 										// si no existe el directorio
    }
    fprintf(f, "%s\n", msg);
    fclose(f);
}

int ejecRm(char *prompt) {
    rm_ruta = NULL;
    int verbose = 0;

    int st = parsingRm(prompt, &verbose);

    if (st == -2) {
    	return SH_OK;
    }

    if (st != SH_OK) {
    	return st;
    }

    struct stat sb;
    if (stat(rm_ruta, &sb) == -1) {											// si esa ruta no existe, escribimos en el archivo

        if (errno == ENOENT) {
            char buf[512];
            snprintf(buf, sizeof(buf), "rm: archivo inexistente: %s", rm_ruta);
            log_sistema_error_min(buf);
        } else {
            perror("stat");
        }
        return SH_SYSERR;
    }

    if (S_ISDIR(sb.st_mode)) {
        fprintf(stderr, "ERROR. %s es un directorio. Si necesitas ayuda ejecuta rm ?\n", rm_ruta);
        return SH_USAGE;
    }

	printf("Aviso de seguridad...\n");
	printf("Archivo encontrado. Confirmacion para eliminar [s/n]\n");
	fflush(stdout);
	char *respuesta = NULL;

    while (1) {
        respuesta = leerLinea();
        if (respuesta == NULL) {   // EOF/error
            return SH_SYSERR;
        }

        if (strcmp(respuesta, "s") == 0) {
            free(respuesta);
            break;                 // seguimos a unlink()
        }
        if (strcmp(respuesta, "n") == 0) {
            free(respuesta);
            return SH_OK;          // no borra, pero el comando termina bien
        }

        free(respuesta);
        printf("ERROR. Introduzca una respuesta valida [s/n]: ");
        fflush(stdout);
    }

    if (unlink(rm_ruta) == -1) {									// el archivo se elimina con la llamada a unlink
        perror("unlink");
        return SH_SYSERR;
    }
    else {
    	if (verbose == 1) {
    		printf("'%s' ha sido correctamente eliminado.\n", rm_ruta);
    	}
    }
    return SH_OK;
}



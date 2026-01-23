/*
 * shellheader.h
 *
 *  Created on: 18 ene. 2026
 *      Author: Marcelo Romero
 */

#ifndef SHELLHEADER_H_
#define SHELLHEADER_H_

#define _POSIX_C_SOURCE 200809L
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <pwd.h>


#define TAMBUFF 8192
#define MAX_PATH 1024


/*CODIGOS DE ERROR*/
enum {
    SH_OK       = 0,   // éxito / línea vacía
    SH_USAGE    = 1,   // error de uso (parámetros)
    SH_SYSERR   = 2,   // error del sistema/archivos
    SH_NOTFOUND = 127  // comando no encontrado
};

/***************************************************************ls*************************************************************************/
char *leerLinea();
int parsingLs(char *linea);
char *detectarComando (char *prompt);
int ejecLs(char *prompt);
/***************************************************************cp*************************************************************************/
int parsingCp(char *linea, int *arg);
int verificarExistencia(char *ruta);
int verificarBarraFinal(char *cadena);
char *rutaFinal (char *src, char*dst);
int loopLectEscr(int fd_entrada, int fd_salida);
char *tipoArchivo(mode_t m);
int ejecCp(char *prompt);
/***************************************************************cat*************************************************************************/
int ejecCat(char *prompt);
int parsingCat (char *linea);
/***************************************************************grep*************************************************************************/
int parsingGrep(char *linea);
int ejecGrep (char *prompt);
/***************************************************************rm*************************************************************************/
int parsingRm(char *linea, int *arg);
void log_sistema_error_min(char *msg);
int ejecRm(char *prompt);
/***************************************************************cd*************************************************************************/
int ejecCd(char *prompt);
/***************************************************************ayudas*************************************************************************/
int ayudaShell (char *ruta);
int ayudaImpresion(unsigned int desde, unsigned int hasta);

/***************************************************************pwd*************************************************************************/
int ejecPwd(char *prompt);
/***************************************************************echo*************************************************************************/
int ejecECHO(char *argumentos);
/***************************************************************mkdir*************************************************************************/
int ejecMKDIR(char *argumentos);
/***************************************************************logs*************************************************************************/
void logAccion(const char *usuario, const char *comando, int exito, const char *mensaje);
void logError(const char *usuario, const char *comando, const char *mensaje_error);
/***************************************************************clear*************************************************************************/
int ejecClear(char *prompt);
#endif /* SHELLHEADER_H_ */

/*
 * shellheader.h
 *
 *  Created on: 18 ene. 2026
 *      Author: Marcelo Romero
 */

#ifndef SHELLHEADER_H_
#define SHELLHEADER_H_

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

/***************************************************************cd*************************************************************************/
int ejecCd(char *directorio);

/***************************************************************cp*************************************************************************/
int parsingCp(char *linea, int *arg);
int verificarExistencia(char *ruta);
int verificarBarraFinal(char *cadena);
char *rutaFinal (char *src, char*dst);
int loopLectEscr(int fd_entrada, int fd_salida);
const char *tipoArchivo(mode_t m);
int ejecCp(char *prompt);

/***************************************************************mkdir*************************************************************************/
int ejecMKDIR(char *argumentos);

/***************************************************************ECHO*************************************************************************/
int ejecECHO(char *directorio);

/***************************************************************pwd*************************************************************************/
int ejecPWD();

#endif /* SHELLHEADER_H_ */

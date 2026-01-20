/*
 * shellheader.h
 *
 *  Created on: 18 ene. 2026
 *      Author: Marcelo Romero
 */

#ifndef SHELLHEADER_H_
#define SHELLHEADER_H_
/***************************************************************ls*************************************************************************/
char *leerLinea();
int parsingLs(char *linea);
char *detectarComando (char *prompt);
int ejecLs(char *prompt);


/***************************************************************cp*************************************************************************/
int parsingCp(char *linea);
int verificarExistencia(char *ruta);
int verificarBarraFinal(char *cadena);
char *rutaFinal (char *src, char*dst);
int loopLectEscr(int fd_entrada, int fd_salida);
const char *tipoArchivo(mode_t m);
int ejecCp(char *prompt);





#endif /* SHELLHEADER_H_ */

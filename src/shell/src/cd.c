#include "shellheader.h"
#define MAX_PATH 1024

int ejecCd(char *prompt){

	/*Funcion que se encarga de identificar el directorio al cual el usuario quiere trasladarse
	 * Parametros:
	 * *prompt -> puntero a la cadena de caracteres que contiene el prompt del usuario
	 * Valor de retorno:
	 * 0 (SH_OK) -> exito
	 * 1 -> error
	 *
	 */
    char *resto = NULL;
    char *cmd = strtok_r(prompt, " \t", &resto);     // "cd"
    if (cmd == NULL) return SH_OK;

    char *directorio = strtok_r(NULL, " \t", &resto);
    if (directorio == NULL) {
        fprintf(stderr, "ERROR. Falta el directorio. Si necesitas ayuda ejecuta cd ?\n");
        return SH_USAGE;
    }

    // si hay más de 1 argumento -> error de uso
    char *token;
    if ((token = strtok_r(NULL, " \t", &resto)) != NULL) {
        fprintf(stderr, "ERROR. Demasiados parametros. Si necesitas ayuda ejecuta cd ?\n");
        return SH_USAGE;
    }

    char cwd_actual[MAX_PATH];
    char cwd_nuevo[MAX_PATH];
    if (!getcwd(cwd_actual, sizeof(cwd_actual))){
        perror("cd");
        return 1;
    }

    if (!directorio){
        printf("ERROR. Se necesita de un directorio al cual ir. Si necesitas ayuda ejecuta cd ?\n");
        return 1;
    }

    if (getcwd(cwd_nuevo, sizeof(cwd_nuevo)) == NULL) {
        perror("cd");
        return 1;
    }

    if (chdir(directorio) == -1) {
        perror("cd");
        return 1;
    }

    setenv("OLDPWD", cwd_actual, 1);
    setenv("PWD", cwd_nuevo, 1);

    return 0;
}



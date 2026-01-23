
#include "shellheader.h"



int ejecPwd(char *prompt) {
	/*
	 * Funcion que obtiene el nombre del directorio de trabajo actual
	 * Parametros:
	 * *prompt -> puntero a la cadena de caracteres que contiene lo escrito por el usuario
	 * Valor de retorno:
	 * 2 (SH_SYSERR) -> si hay error
	 * 0 (SH_OK) -> si no hay error
	 */
    (void)prompt;

    char cwd[MAX_PATH];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return SH_SYSERR;
    }

    puts(cwd);
    return SH_OK;
}



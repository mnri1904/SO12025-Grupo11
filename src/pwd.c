#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#define MAX_PATH 1024

/*
 *Funcion que se encarga de la ejecucion de pwd. getcwd 
 *Parámetros:
 * Ninguno.
 *Valor de retorno:
 * 0 -> exito.
 * 1 -> error si errores EACCES O ERANGE.
*/

int ejecPWD (){
    char cwd[MAX_PATH];
    if (getcwd(cwd, sizeof(cwd))){
        printf("%s\n", cwd);
    } else {
        perror ("getcwd");
        return 1;
    }
    return 0;
}

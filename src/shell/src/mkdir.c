#include "shellheader.h"
/*
 Funcion que se encarga de la ejecucion de mkdir.
 *Parámetros:
 * argumentos -> puntero a char que apunta a todo lo que el usauario escribio despues del comando mkdir.
 *Valor de retorno:
 * 0 -> exito, se crearon los directorios especificados o los directorios ya exist.
 * 1 -> error si errores EACCES O ERANGE.
*/
int ejecMKDIR(char *argumentos){
    char *token, *resto;                                                            //token es el puntero a cada nombre de directorio que vamos a recibir y resto es

    if (!argumentos){
        printf("ERROR. Se necesita de al menos un (1) nombre de directorio. Si necesitas ayuda ejecuta mkdir ?\n");
        //mkdir requiere de argumentos, luego imprimimos un mensaje de error indicandolo y retornamos 1;
        return 1;
    }

    resto = argumentos;                                                             //inicializamos el vector que strtok va a actualzar
    token = strtok_r(resto, " \t\n", &resto);                                       //extrae el primer token hasta el primer espacio, tab, o enter

    while (token) {                                                                 //iteramos mientras el token sea valido
        if (mkdir(token, 0755) == -1) {                                             //intenta crear el direcotrio con permisos 0755
            perror("token");                                                        // si falla, mostramos error pero seguimos
        } else {
            printf("Directorio '%s' creado correctamente.\n", token);
        }

        token = strtok_r(NULL, " \t\n", &resto);                                    // siguiente token
    }

    return 0;
}


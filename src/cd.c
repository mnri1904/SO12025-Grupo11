// Hecho por: Alexandre Avalos

#include <stdlib.h>
#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#define MAX_PATH 1024

int ejecCd(char *directorio){
    char cwd_actual[MAX_PATH];
    char cwd_nuevo[MAX_PATH];
    if (!getcwd(cwd_actual, sizeof(cwd_actual))){
        perror("cd");
        return 1;
    }
    
    if (!directorio){
        printf("ERROR. Se necesita de un directorio al cual ir.\n");
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

#include "shellheader.h"
int ejecECHO(char *argumentos) {
    if (argumentos == NULL) {
        printf("\n");
        return 0;
    }

    printf("%s\n", argumentos);
    return 0;
}


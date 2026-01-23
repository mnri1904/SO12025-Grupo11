#include <stdio.h>
#include <unistd.h>
#include "shellheader.h"

int ejecClear(char *prompt) {
    (void)prompt;

    /* Si stdout es una terminal limpiamos pantalla y movemos cursor a (0,0) */
    if (isatty(STDOUT_FILENO)) {
        fputs("\033[H\033[2J", stdout);
        fflush(stdout);
    } else {
        for (int i = 0; i < 50; i++) putchar('\n');
    }

    return SH_OK;
}


#include "shellheader.h"



int ejecPwd(char *prompt) {
    (void)prompt;

    char cwd[MAX_PATH];
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        perror("getcwd");
        return SH_SYSERR;
    }

    puts(cwd);
    return SH_OK;
}



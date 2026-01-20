/*
 ============================================================================
 Name        : shell.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "shellheader.h"
int main() {

	while(1) {
		printf("shell:~$ ");
		char *prompt = leerLinea();
		if (prompt == NULL) {
			break;
		}

		else {
			char *comando = detectarComando(prompt);
			if ((strcmp(comando, "ls") == 0)) {
				ejecLs(prompt);
			}
			else if ((strcmp(comando, "cp") == 0)) {
				ejecCp(prompt);
			}
		}
	}
	return 0;
}










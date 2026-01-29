/*
 ============================================================================
 Name        : shell.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "shellheader.h"

char variableEnt[MAX_PATH+16];


static void imprimirBanner(void) {
    puts("==============================================================");
    puts("                 BIENVENIDO A SOSH (SO1 TP)                   ");
    puts("==============================================================");
    puts("Tip: usa `shell ?` para ver la guía completa.");
    puts("Si quieres leer 5 consejos de seguridad, ejecuta `seguridad`");
    puts("Si quieres aprender sobre uso general de Linux, ejecuta `aprender`");
    puts("Si quieres aprender un poco sobre la historia de Linux, ejecuta `historia`");
    puts("Si conocer cuales son las 5 distros de Linux mas utilizadas, ejecuta `distros`");
    puts("");
}


static const char *obtenerUsuario(void) {
	/*
	 * Funcion que obtiene el usuario correspondiente
	 * Valor de retorno:
	 * u -> el usuario encontrado (si esta registrado en la base de sesiones)
	 * "desconocido" -> cadena de caracteres a utilizar para nombrar a un usuario si este no es encontrado
	 */
    struct passwd *pw = getpwuid(getuid());
    if (pw && pw->pw_name) return pw->pw_name;

    const char *u = getenv("LOGNAME");
    if (!u) u = getenv("USER");
    return u ? u : "desconocido";
}

static void actualizarVariableEnt() {
	/*
	 * Funcion que actualiza la variable de entorno dependiendo del directorio donde nos encontramos
	 */
	char cwd[MAX_PATH];

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        snprintf(variableEnt, sizeof(variableEnt), "sosh:~$ ");
        return;
    }

    snprintf(variableEnt, sizeof(variableEnt), "sosh:%s$ ", cwd); // mostramos cwd completo
}

static int esLineaVacia(const char *linea) {
	/*
	 * Funcion que se encarga de detectar si una linea introducida por el usuario esta vacia
	 * Parametros
	 * *linea -> cadena de caracteres que almacena lo introducido por el usuario
	 * Valor de retorno:
	 * 0 -> si no esta vacia
	 * 1 -> si esta vacia
	 */
    while (*linea) {
        if (*linea != ' ' && *linea != '\t') return 0;
        linea++;
    }
    return 1;
}

static int horaActualLocal() {
	/*
	 * Funcion que se encarga de obtener la hora actual del sistema
	 */
    time_t t = time(NULL);
    struct tm *tmv = localtime(&t);
    return tmv ? tmv->tm_hour : -1;   // 0..23, o -1 si falla
}

static int obtenerEdadUsuario() {
	/*
	 * FUncion que obtiene la edad del usuario con una variable de entorno
	 * Valor de retorno:
	 * b -> la edad del usuario
	 */
    const char *env = getenv("SOSH_AGE");
    if (env && *env) return atoi(env);

    char b[8];
    printf("Introduce tu edad edad: ");
    fflush(stdout);
    if (!fgets(b, sizeof b, stdin)) return -1;
    return atoi(b);
}

static int accesoRestringidoPorEdadHorario(int edad) {
	/*
	 * Funcion que se encarga de definir si el usuario esta habilitado para usar la shell
	 */
    int h = horaActualLocal();
    if (h < 0) return 0;

    if (edad <= 0) return (h >= 22); // bloqueo desde 22:00
    return (edad < 18 && h >= 22);
}


int main(void) {
    int nerror = SH_OK;
    int exito = 0;

    const char *usuario = obtenerUsuario();
    if (!usuario) usuario = "desconocido";

    const char *usuario = obtenerUsuario();
    if (!usuario) usuario = "desconocido";

    /* ===== Restricción edad/horario ===== */
    int edad = obtenerEdadUsuario();
    if (accesoRestringidoPorEdadHorario(edad)) {
        fprintf(stderr, "ACCESO DENEGADO: menores de 18 no pueden usar SOSH después de las 22:00.\n");
        fprintf(stderr, "Opcional: definir SOSH_AGE para omitir la pregunta de edad (ej.: export SOSH_AGE=18)\n");
        logAccion(usuario, "ACCESO", 0, "restriccion edad/horario");
        return SH_USAGE;
    }

    imprimirBanner();

    while (1) {
    	actualizarVariableEnt();
    	fprintf(stdout, "%s",variableEnt);
        fflush(stdout);

        char *prompt = leerLinea();
        if (prompt == NULL) {
            break;
        }

        if (esLineaVacia(prompt)) {
            free(prompt);
            continue;
        }

        char *duplicado = strdup(prompt);
        if (!duplicado) {
            perror("strdup");
            nerror = SH_SYSERR;
            logError(usuario, prompt, strerror(errno));
            free(prompt);
            continue;
        }

        char *cadena = NULL;
        char *comando = strtok_r(duplicado, " \t", &cadena);

        if (comando == NULL) {
            free(duplicado);
            free(prompt);
            continue;
        }

        /* ====================== COMANDOS ====================== */

        if ((strcmp(comando, "ls") == 0) || (strcmp(comando, "listar") == 0)) {
            nerror = ejecLs(prompt);
        }
        else if ((strcmp(comando, "cp") == 0) || (strcmp(comando, "copiar") == 0)) {
            nerror = ejecCp(prompt);
            if (nerror != SH_OK) logError(usuario, prompt, strerror(errno));
        }
        else if ((strcmp(comando, "cat") == 0) || (strcmp(comando, "concatenar") == 0)) {
            nerror = ejecCat(prompt);
            if (nerror != SH_OK) logError(usuario, prompt, strerror(errno));
        }
        else if ((strcmp(comando, "grep") == 0) || (strcmp(comando, "buscar") == 0)) {
            nerror = ejecGrep(prompt);
            if (nerror != SH_OK) logError(usuario, prompt, strerror(errno));
        }
        else if ((strcmp(comando, "rm") == 0) || (strcmp(comando, "remover") == 0)) {
            nerror = ejecRm(prompt);
            if (nerror != SH_OK) logError(usuario, prompt, strerror(errno));
        }

        /* ---------------------- cd y cd ? ---------------------- */
        else if ((strcmp(comando, "cd") == 0) || (strcmp(comando, "cambiardir") == 0)) {
            char *arg1 = strtok_r(NULL, " \t", &cadena);
            char *arg2 = strtok_r(NULL, " \t", &cadena);

            if (arg1 && strcmp(arg1, "?") == 0 && arg2 == NULL) {
                nerror = ayudaImpresion(89, 104);
            } else if (arg1 == NULL || arg2 != NULL) {
                nerror = SH_USAGE;
            } else {
                nerror = ejecCd(prompt);
                if (nerror != SH_OK) logError(usuario, prompt, strerror(errno));
            }
        }

        /* ---------------------- pwd y pwd ? ---------------------- */
        else if ((strcmp(comando, "pwd") == 0) || (strcmp(comando, "diractual") == 0)) {
            char *arg1 = strtok_r(NULL, " \t", &cadena);
            char *arg2 = strtok_r(NULL, " \t", &cadena);

            if (arg1 && strcmp(arg1, "?") == 0 && arg2 == NULL) {
                nerror = ayudaImpresion(137, 147);
            } else if (arg1 != NULL) {   // pwd no lleva operandos
                nerror = SH_USAGE;
            } else {
                nerror = ejecPwd(prompt);
                if (nerror != SH_OK) logError(usuario, prompt, strerror(errno));
            }
        }

        /* ---------------------- echo y echo ? ---------------------- */
        else if ((strcmp(comando, "echo") == 0) || (strcmp(comando, "eco") == 0)) {

            if (cadena != NULL && strcmp(cadena, "?") == 0) {
                nerror = ayudaImpresion(123, 134);
            } else {
                nerror = ejecECHO(cadena);
                if (nerror != SH_OK) logError(usuario, prompt, strerror(errno));
            }
        }

        /* ---------------------- mkdir y mkdir ? ---------------------- */
        else if ((strcmp(comando, "mkdir") == 0) || (strcmp(comando, "creardir") == 0)) {
            char *arg1 = strtok_r(NULL, " \t", &cadena);
            char *arg2 = strtok_r(NULL, " \t", &cadena);

            if (arg1 && strcmp(arg1, "?") == 0 && arg2 == NULL) {
                nerror = ayudaImpresion(107, 120);
            } else if (arg1 == NULL || arg2 != NULL) {
                nerror = SH_USAGE;
            } else {
                nerror = ejecMKDIR(arg1);
                if (nerror != SH_OK) logError(usuario, prompt, strerror(errno));
            }
        }

        /* ---------------------- shell ? (ayuda general) ---------------------- */
        else if (strcmp(comando, "shell") == 0) {
            char *arg1 = strtok_r(NULL, " \t", &cadena);
            char *arg2 = strtok_r(NULL, " \t", &cadena);

            if (arg1 && strcmp(arg1, "?") == 0 && arg2 == NULL) {
                nerror = ayudaShell("src/guia.txt");
            } else {
                nerror = SH_USAGE;
            }
        }
        /* ---------------------- exit y exit ? ---------------------- */
        else if ((strcmp(comando, "exit") == 0) || (strcmp(comando, "salir") == 0)){
            char *arg1 = strtok_r(NULL, " \t", &cadena);
            char *arg2 = strtok_r(NULL, " \t", &cadena);

            if (arg1 && strcmp(arg1, "?") == 0 && arg2 == NULL) {
                nerror = ayudaImpresion(150, 161);
            } else if (arg1 == NULL) {
                logAccion(usuario, "exit", 1, "cierre limpio");
                free(duplicado);
                free(prompt);
                break;
            } else {
                nerror = SH_USAGE;
            }
        }
        else if ((strcmp(comando, "whoami") == 0) || (strcmp(comando, "quiensoy") == 0)) {

            if (cadena != NULL && strcmp(cadena, "?") == 0) {
                nerror = ayudaImpresion(164, 167);
            } else {
            	fprintf(stdout, "%s\n", usuario);
            }
        }
        else if (strcmp(comando, "seguridad") == 0) {
        	nerror = ayudaShell("src/seguridad.txt");
        }
        else if (strcmp(comando, "aprender") == 0) {
        	nerror = ayudaShell("src/usos.txt");
        }
        else if (strcmp(comando, "historia") == 0) {
        	nerror = ayudaShell("src/historia.txt");
        }
        else if (strcmp(comando, "distros") == 0) {
        	nerror = ayudaShell("src/distros.txt");
        }
        else if ((strcmp(comando, "clear") == 0) || (strcmp(comando, "limpiar") == 0)) {
            char *arg1 = strtok_r(NULL, " \t", &cadena);
            char *arg2 = strtok_r(NULL, " \t", &cadena);

            if (arg1 && strcmp(arg1, "?") == 0 && arg2 == NULL) {
                nerror = ayudaImpresion(170,173);
            } else if (arg1 != NULL) {   // clear no lleva operandos
                nerror = SH_USAGE;
            } else {
                nerror = ejecClear(prompt);
            }
        }

        /* ---------------------- comando desconocido ---------------------- */
        else {
            fprintf(stderr, "ERROR. Comando no encontrado: %s. Si necesitas ayuda ejecuta shell ?\n", comando);
            logError(usuario, prompt, "comando no encontrado");
            nerror = SH_NOTFOUND;
        }

        /* ====================== LOG ACCION ====================== */
        if (nerror == SH_OK) {
            exito = 1;
            logAccion(usuario, prompt, exito, "comando ejecutado");
        } else {
            exito = 0;
            logAccion(usuario, prompt, exito, "comando fallido");
        }

        free(duplicado);
        free(prompt);
    }
    return nerror;
}



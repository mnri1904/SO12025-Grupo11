#include "shellheader.h"
#define LOG_DIR "/var/log/shell"
#define LOG_ACCIONES "/var/log/shell/shell.log"
#define LOG_ERRORES "/var/log/shell/sistema_error.log"

static void crearDirectorioSiNoExiste() {
    struct stat st;

    if (stat(LOG_DIR, &st) == -1) {
        mkdir(LOG_DIR, 0755);
    }
}

static void obtenerTimestamp(char *buffer, size_t tam) {
    time_t ahora = time(NULL);
    struct tm *t = localtime(&ahora);

    strftime(buffer, tam, "%Y-%m-%d %H:%M:%S", t);
}

void logAccion(const char *usuario, const char *comando, int exito, const char *mensaje){
    FILE *f;
    char tiempo[64];

    crearDirectorioSiNoExiste();
    obtenerTimestamp(tiempo, sizeof(tiempo));

    f = fopen(LOG_ACCIONES, "a");
    if (f == NULL) return;

    fprintf(f, "[%s] user=%s comando=\"%s\" resultado=%s msg=\"%s\"\n", tiempo, usuario, comando, exito ? "SUCCESS" : "FAIL", mensaje);

    fclose(f);
}

void logError(const char *usuario, const char *comando, const char *mensaje_error){
    FILE *f;
    char tiempo[64];

    crearDirectorioSiNoExiste();
    obtenerTimestamp(tiempo, sizeof(tiempo));

    f = fopen(LOG_ERRORES, "a");
    if (f == NULL) return;
    fprintf(f, "[%s] user=%s comando=\"%s\" error=\"%s\"\n", tiempo, usuario, comando, mensaje_error);

    fclose(f);
}

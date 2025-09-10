// lanzador.c
// Parte 1 punto 2: usar exec para ejecutar el programa del punto 1 ("matrizsum").
// Este lanzador valida m y n, y luego invoca ./matrizsum m n 

//Compilar con: gcc -O2 -Wall -Wextra -o lanzador lanzador.c
//Ejecutar con: ./lanzador 4 2 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

static bool es_entero_positivo(const char *s) {
    if (!s || !*s) return false;
    for (const char *p = s; *p; ++p) {
        if (!isdigit((unsigned char)*p)) return false;
    }
    return strcmp(s, "0") != 0;
}

static void uso(const char *prog) {
    fprintf(stderr,
        "Uso: %s m n\n"
        "  m = tamaño de la matriz cuadrada (entero positivo)\n"
        "  n = número de procesos (entero positivo)\n",
        prog);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        uso(argv[0]);
        return EXIT_FAILURE;
    }

    const char *m_str = argv[1];
    const char *n_str = argv[2];

    if (!es_entero_positivo(m_str) || !es_entero_positivo(n_str)) {
        fprintf(stderr, "Error: m y n deben ser enteros positivos.\n");
        uso(argv[0]);
        return EXIT_FAILURE;
    }

    // argv para exec: nombre del programa + args + NULL
    char *const args[] = { "matrizsum", (char *)m_str, (char *)n_str, NULL };

    // 1) Intentar ejecutar desde el directorio actual
    if (access("./matrizsum", X_OK) == 0) {
        execv("./matrizsum", args);
        perror("execv(\"./matrizsum\")");
        return EXIT_FAILURE;
    }

    // 2) Si no está en el directorio actual, intentar buscar en el PATH
    execvp("matrizsum", args);
    perror("execvp(\"matrizsum\")");
    return EXIT_FAILURE;
}

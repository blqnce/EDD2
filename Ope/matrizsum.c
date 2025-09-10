// matrizsum.c
// Parte 1, Punto 1: crea n procesos, cada uno suma m/n filas y muestra los números, la suma y el PID.

//Compilar con: gcc -O2 -Wall -Wextra -o matrizsum matrizsum.c
//Ejecutar con: ./matrizsum 4 2 



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

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
        "  n = número de procesos (entero positivo y divisor de m)\n", prog);
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

    char *endp = NULL;
    long m_long = strtol(m_str, &endp, 10);
    if (*endp != '\0' || m_long <= 0) {
        fprintf(stderr, "Error: m inválido.\n");
        uso(argv[0]);
        return EXIT_FAILURE;
    }
    long n_long = strtol(n_str, &endp, 10);
    if (*endp != '\0' || n_long <= 0) {
        fprintf(stderr, "Error: n inválido.\n");
        uso(argv[0]);
        return EXIT_FAILURE;
    }

    int m = (int)m_long;
    int n = (int)n_long;

    if (m % n != 0) {
        fprintf(stderr, "Error: n (%d) debe ser divisor de m (%d).\n", n, m);
        uso(argv[0]);
        return EXIT_FAILURE;
    }

    // Reservar matriz m x m
    int *mat = (int *)malloc((size_t)m * (size_t)m * sizeof(int));
    if (!mat) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    // Llenar matriz con valores (0..9)
    srand((unsigned)time(NULL));
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < m; ++j) {
            mat[i * m + j] = rand() % 10;
        }
    }

    int filas_por_proceso = m / n;

    // Crear n procesos
    for (int p = 0; p < n; ++p) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            free(mat);
            return EXIT_FAILURE;
        } else if (pid == 0) {
            // Hijo: procesa su bloque de filas
            pid_t yo = getpid();
            int inicio = p * filas_por_proceso;
            int fin = inicio + filas_por_proceso; // no inclusive

            // Desactivar buffer para evitar intercalado raro (opcional)
            setvbuf(stdout, NULL, _IONBF, 0);

            for (int i = inicio; i < fin; ++i) {
                int suma = 0;
                // Imprimir la fila y acumular suma
                // Formato: "PID=xxxx | fila i: v1 v2 ... vk  => suma=S"
                printf("PID=%d | fila %d: ", (int)yo, i);
                for (int j = 0; j < m; ++j) {
                    int v = mat[i * m + j];
                    suma += v;
                    printf("%d", v);
                    if (j < m - 1) printf(" ");
                }
                printf("  => suma=%d\n", suma);
            }
            free(mat);
            _exit(EXIT_SUCCESS);
        }
        // Padre continúa creando más hijos
    }

    // Padre: esperar a todos los hijos
    int hijos_restantes = n;
    while (hijos_restantes > 0) {
        int status = 0;
        pid_t w = wait(&status);
        if (w == -1) {
            if (errno == EINTR) continue;
            perror("wait");
            break;
        }
        --hijos_restantes;
        if (WIFEXITED(status)) {
            int code = WEXITSTATUS(status);
            if (code != 0) {
                fprintf(stderr, "Hijo %d terminó con código %d\n", (int)w, code);
            }
        } else if (WIFSIGNALED(status)) {
            fprintf(stderr, "Hijo %d terminó por señal %d\n", (int)w, WTERMSIG(status));
        }
    }

    free(mat);
    return EXIT_SUCCESS;
}

/* Compilar: gcc matrizproc.c -o matrizproc -pthread -- Se especifica que el ejecutable se llame matrizproc*/

#include <stdio.h>      
#include <stdlib.h>    
#include <pthread.h>    
#include <string.h>   // Para usar la función strcmp para comparar strings
#include <time.h>     // Para generar numeros aleatorios diferentes cada vez (En el enunciado dice que se crea aleatoriamente)
 

#define NUM_GRUPO 4   // Somos grupo 4

// Estructura para tener todos los que necesita el hilo
// Se usa porque a la función de un hilo solo se le puede pasar un argumento (en teoria)
typedef struct {
    int fila_inicio;                // Fila donde el hilo comienza a trabajar
    int fila_fin;                   // Fila donde el hilo termina
    int **puntero_matriz;           // Puntero a la matriz compartida
    int tamano_matriz;              // El tamaño m de la matriz
    char *operacion_a_realizar;     // La operación (sumar o max)
} DatosHilo;

// -------------------------------------------------------------------------------------------------------------------------------
// LA FUNCION QUE EJECUTAN LOS HILOS
// Cada hilo que se crea ejecuta esta misma función, pero con sus propios datos (su propio rango de filas para procesar)
// El tipo de retorno es void* y recibe un argumento 'void*', porque asi lo exige pthread
 
void* procesar_filas_del_hilo(void* argumentos) {
    // Aqui se convierte el puntero al tipo de estructura
    DatosHilo* datos_recibidos = (DatosHilo*)argumentos;

    // El hilo itera sobre el rango de filas que se le dio
    for (int i = datos_recibidos->fila_inicio; i < datos_recibidos->fila_fin; i++) {
        
        // Primero se imprimen todos los elementos de la fila actual
        for (int j = 0; j < datos_recibidos->tamano_matriz; j++) {
            printf("%d ", datos_recibidos->puntero_matriz[i][j]);
        }

        // Luego se hace la operacion solicitada
        if (strcmp(datos_recibidos->operacion_a_realizar, "sumar") == 0) { 
            int suma_total_fila = 0;
            for (int j = 0; j < datos_recibidos->tamano_matriz; j++) {
                suma_total_fila += datos_recibidos->puntero_matriz[i][j];
            }
            printf("%d\n", suma_total_fila); //

        } else if (strcmp(datos_recibidos->operacion_a_realizar, "max") == 0) { 
            int valor_maximo_fila = datos_recibidos->puntero_matriz[i][0];
            for (int j = 1; j < datos_recibidos->tamano_matriz; j++) {
                if (datos_recibidos->puntero_matriz[i][j] > valor_maximo_fila) {
                    valor_maximo_fila = datos_recibidos->puntero_matriz[i][j];
                }
            }
            printf("%d\n", valor_maximo_fila); //
        }
    }
    
    // El hilo finaliza su ejecución.
    pthread_exit(NULL);
}
// -------------------------------------------------------------------------------------------------------------------------------

// EL MAIN

int main(int argc, char *argv[]) {

    // PASO 1: Validar los argumentos de la linea de comandos
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <m> <operacion>\n", argv[0]);
        fprintf(stderr, "<m> es el tamaño de la matriz\n");
        fprintf(stderr, "<operacion> es 'sumar' o 'max'\n");
        return 1;
    }

    int tamano_m = atoi(argv[1]);
    char *operacion_u = argv[2];

    if (strcmp(operacion_u, "sumar") != 0 && strcmp(operacion_u, "max") != 0) {
        fprintf(stderr, "Error: la operación debe ser 'sumar' o 'max'.\n");
        return 1;
    }
    
    if (tamano_m <= 0) {
        fprintf(stderr, "Error: el tamaño de la matriz <m> debe ser un numero positivo.\n");
        return 1;
    }

    // PASO 2: Crear y llenar la matriz dinamicamente
    printf("Matriz aleatoria de %d x %d\n", tamano_m, tamano_m);

    int **matriz_dinamica = (int **)malloc(tamano_m * sizeof(int *));
    for (int i = 0; i < tamano_m; i++) {
        matriz_dinamica[i] = (int *)malloc(tamano_m * sizeof(int));
    }

    srand(time(NULL));
    for (int i = 0; i < tamano_m; i++) {
        for (int j = 0; j < tamano_m; j++) {
            matriz_dinamica[i][j] = rand() % 10;
        }
    }

    // PASO 3: Preparar la distribución del trabajo para los hilos
    const int numero_de_hilos_p = NUM_GRUPO + 2; // La operacion que pide el profesor
    
    pthread_t identificadores_hilos[numero_de_hilos_p];
    DatosHilo argumentos_para_hilos[numero_de_hilos_p];

    int filas_base_por_hilo = tamano_m / numero_de_hilos_p;
    int filas_restantes = tamano_m % numero_de_hilos_p;
    int fila_actual_asignada = 0;

    printf("Se distribuyen %d filas entre %d hilos\n\n", tamano_m, numero_de_hilos_p);

    // PASO 4: Crear y lanzar los hilos
    for (int i = 0; i < numero_de_hilos_p; i++) {
        argumentos_para_hilos[i].puntero_matriz = matriz_dinamica;
        argumentos_para_hilos[i].tamano_matriz = tamano_m;
        argumentos_para_hilos[i].operacion_a_realizar = operacion_u;
        argumentos_para_hilos[i].fila_inicio = fila_actual_asignada;

        int cantidad_filas_para_este_hilo = filas_base_por_hilo + (i < filas_restantes ? 1 : 0);
        fila_actual_asignada += cantidad_filas_para_este_hilo;
        argumentos_para_hilos[i].fila_fin = fila_actual_asignada;

        pthread_create(&identificadores_hilos[i], NULL, procesar_filas_del_hilo, &argumentos_para_hilos[i]);
    }

    // PASO 5: Esperar a que todos los hilos terminen su ejecucion
    for (int i = 0; i < numero_de_hilos_p; i++) {
        pthread_join(identificadores_hilos[i], NULL);
    }
    
    // PASO 6: Liberar la memoria reservada (Buena practica?)
    for (int i = 0; i < tamano_m; i++) {
        free(matriz_dinamica[i]);
    }
    free(matriz_dinamica);

    printf("\nFin\n");
    return 0;
}
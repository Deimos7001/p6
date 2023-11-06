#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//INF-317 - 1ER EXAMEN PARCIAL
//NOMBRE: VICTOR GABRIEL CAPIA ALI
//CI: 4762494 LP
//PREGUNTA 6. La numeral 4 (PREGUNTA 4) con MPI.

// Función para calcular el valor en la serie
int calcular_valor(int posicion) {
    if (posicion % 2 == 1) {
        // Números impares
        return ((posicion / 2) + 1) * ((posicion / 2) + 1) + 1;
    } else {
        // Números pares
        return (posicion / 2) * 2;
    }
}

int main(int argc, char** argv) {
    int num_procesos, id_proceso;
    MPI_Init(&argc, &argv); // Inicializar MPI
    MPI_Comm_size(MPI_COMM_WORLD, &num_procesos); // Obtener el número de procesos
    MPI_Comm_rank(MPI_COMM_WORLD, &id_proceso); // Obtener el ID del proceso

    int num_posiciones = 10000;
    int *serie_local = (int*)malloc((num_posiciones / num_procesos) * sizeof(int)); // Crear un arreglo local para cada proceso
    int *serie_final = NULL;
    int i;

    // Calcular los valores locales en la serie
    for (i = id_proceso; i < num_posiciones; i += num_procesos) {
        serie_local[i / num_procesos] = calcular_valor(i + 1);
    }

    if (id_proceso == 0) {
        serie_final = (int*)malloc(num_posiciones * sizeof(int)); // Crear un arreglo para el proceso raíz
    }

    // Reunir los valores locales en el proceso raíz
    MPI_Gather(serie_local, num_posiciones / num_procesos, MPI_INT, serie_final, num_posiciones / num_procesos, MPI_INT, 0, MPI_COMM_WORLD);

    if (id_proceso == 0) {
        // Imprimir la serie final en el proceso raíz
        for (i = 0; i < num_posiciones; i++) {
            printf("%d, ", serie_final[i]);
        }
        free(serie_final); // Liberar memoria del arreglo final
    }

    MPI_Finalize(); // Finalizar MPI
    free(serie_local); // Liberar memoria del arreglo local

    return 0;
}


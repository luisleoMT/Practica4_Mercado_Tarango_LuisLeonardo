#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

typedef struct {
    double x, y;
} Punto;

// Función para calcular la distancia euclidiana entre dos puntos
double distancia(Punto p1, Punto p2) {
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// Función para encontrar el par de puntos más cercano
void encontrarParCercano(Punto puntos[], int n, Punto *p1Cercano, Punto *p2Cercano) {
    double minDist = DBL_MAX; // Iniciar con una distancia máxima
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = distancia(puntos[i], puntos[j]);
            if (dist < minDist) {
                minDist = dist;
                *p1Cercano = puntos[i];
                *p2Cercano = puntos[j];
            }
        }
    }
}

// Función para generar puntos aleatorios
void generarPuntos(Punto puntos[], int n) {
    for (int i = 0; i < n; i++) {
        puntos[i].x = ((double)rand() / RAND_MAX) * 1000.0;
        puntos[i].y = ((double)rand() / RAND_MAX) * 1000.0;
    }
}

int main() {
    srand(time(0)); // Inicializar semilla para generación aleatoria

    int n;
    Punto *puntos, p1Cercano, p2Cercano;
    clock_t inicio, fin;
    double tiempo;

    // Arreglo con los valores de n a probar
    int pruebas[] = {10, 100, 1000, 10000, 100000};
    int numPruebas = sizeof(pruebas) / sizeof(pruebas[0]);

    for (int k = 0; k < numPruebas; k++) {
        n = pruebas[k];
        puntos = (Punto *)malloc(n * sizeof(Punto));

        generarPuntos(puntos, n);

        inicio = clock(); // Empezar a medir tiempo
        encontrarParCercano(puntos, n, &p1Cercano, &p2Cercano);
        fin = clock(); // Terminar medición

        tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
        printf("Para n=%d, los puntos más cercanos son: (%.2f, %.2f) y (%.2f, %.2f)\n", n, p1Cercano.x, p1Cercano.y, p2Cercano.x, p2Cercano.y);
        printf("Tiempo de ejecución: %f segundos\n", tiempo);

        free(puntos); // Liberar memoria
    }

    return 0;
}

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

// Función de comparación para ordenar puntos por coordenada X
int compararX(const void *a, const void *b) {
    Punto *puntoA = (Punto *)a;
    Punto *puntoB = (Punto *)b;
    return (puntoA->x - puntoB->x);
}

// Función de comparación para ordenar puntos por coordenada Y
int compararY(const void *a, const void *b) {
    Punto *puntoA = (Punto *)a;
    Punto *puntoB = (Punto *)b;
    return (puntoA->y - puntoB->y);
}

// Función para encontrar la distancia mínima en un subconjunto de puntos
double distanciaMinima(Punto puntos[], int n) {
    double minDist = DBL_MAX;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double dist = distancia(puntos[i], puntos[j]);
            if (dist < minDist) {
                minDist = dist;
            }
        }
    }
    return minDist;
}

// Función para encontrar la distancia mínima en la franja (strip)
double distanciaStrip(Punto strip[], int size, double distMinima) {
    double minDist = distMinima;
    qsort(strip, size, sizeof(Punto), compararY); // Ordenar por coordenada Y

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < minDist; j++) {
            double dist = distancia(strip[i], strip[j]);
            if (dist < minDist) {
                minDist = dist;
            }
        }
    }
    return minDist;
}

// Función recursiva para encontrar la distancia mínima usando divide y vencerás
double encontrarParCercanoRec(Punto puntos[], Punto aux[], int izq, int der) {
    // Si hay pocos puntos, usar el método de fuerza bruta
    if (der - izq <= 3) {
        return distanciaMinima(puntos + izq, der - izq + 1);
    }

    int medio = (izq + der) / 2;
    Punto puntoMedio = puntos[medio];

    // Dividir y encontrar la distancia mínima en las dos mitades
    double distIzq = encontrarParCercanoRec(puntos, aux, izq, medio);
    double distDer = encontrarParCercanoRec(puntos, aux, medio + 1, der);

    // Encontrar la menor distancia de ambas mitades
    double distMinima = fmin(distIzq, distDer);

    // Crear el strip (franja) que contiene los puntos cercanos al corte medio
    int sizeStrip = 0;
    for (int i = izq; i <= der; i++) {
        if (fabs(puntos[i].x - puntoMedio.x) < distMinima) {
            aux[sizeStrip] = puntos[i];
            sizeStrip++;
        }
    }

    // Encontrar la distancia mínima en el strip
    return fmin(distMinima, distanciaStrip(aux, sizeStrip, distMinima));
}

// Función principal que ordena los puntos y llama a la recursiva
double encontrarParCercano(Punto puntos[], int n) {
    Punto *aux = (Punto *)malloc(n * sizeof(Punto));
    qsort(puntos, n, sizeof(Punto), compararX); // Ordenar puntos por coordenada X
    double resultado = encontrarParCercanoRec(puntos, aux, 0, n - 1);
    free(aux);
    return resultado;
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
    Punto *puntos;
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
        double distMinima = encontrarParCercano(puntos, n);
        fin = clock(); // Terminar medición

        tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
        printf("Para n=%d, la distancia mínima es: %f\n", n, distMinima);
        printf("Tiempo de ejecución: %f segundos\n", tiempo);

        free(puntos); // Liberar memoria
    }

    return 0;
}

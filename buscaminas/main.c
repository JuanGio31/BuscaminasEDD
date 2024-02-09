#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define MINA_ENCONTRADA 1
#define ESPACIO_YA_DESCUBIERTO 2
#define NA 3


#define FILAS 6
#define COLUMNAS 6
#define MINA  '*'
#define ESPACIO_SIN_DESCUBRIR  '.'
#define ESPACIO_DESCUBIERTO  ' '
#define NUMERO_DE_MINAS 9

#define DEBUG 0

void iniciarTablero(char matrix[FILAS][COLUMNAS]) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            matrix[i][j] = ESPACIO_SIN_DESCUBRIR;
        }
    }
}

int obtenerMinasCercanas(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
    int conteo = 0, filaInicio, filaFin, columnaInicio, columnaFin;
    if (fila <= 0) {
        filaInicio = 0;
    } else {
        filaInicio = fila - 1;
    }

    if (fila + 1 >= FILAS) {
        filaFin = FILAS - 1;
    } else {
        filaFin = fila + 1;
    }

    if (columna <= 0) {
        columnaInicio = 0;
    } else {
        columnaInicio = columna - 1;
    }
    if (columna + 1 >= COLUMNAS) {
        columnaFin = COLUMNAS - 1;
    } else {
        columnaFin = columna + 1;
    }
    int m;
    for (m = filaInicio; m <= filaFin; m++) {
        int l;
        for (l = columnaInicio; l <= columnaFin; l++) {
            if (tablero[m][l] == MINA) {
                conteo++;
            }
        }
    }
    return conteo;
}

int aleatorioEnRango(int minimo, int maximo) {
    return minimo + rand() / (RAND_MAX / (maximo - minimo + 1) + 1);
}

void colocarMina(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
    tablero[fila][columna] = MINA;
}

void colocarMinasAleatoriamente(char arr[FILAS][COLUMNAS]) {
    int l;
    for (l = 0; l < NUMERO_DE_MINAS; l++) {
        int fila = aleatorioEnRango(0, FILAS - 1);
        int columna = aleatorioEnRango(0, COLUMNAS - 1);
        colocarMina(fila, columna, arr);
    }
}

int abrirCasilla(int fila, int columna, char tablero[FILAS][COLUMNAS]) {
    fila--;
    columna--;

    assert(columna < COLUMNAS && columna >= 0);
    assert(fila < FILAS && fila >= 0);
    if (tablero[fila][columna] == MINA) {
        return MINA_ENCONTRADA;
    }
    if (tablero[fila][columna] == ESPACIO_DESCUBIERTO) {
        return ESPACIO_YA_DESCUBIERTO;
    }
    tablero[fila][columna] = ESPACIO_DESCUBIERTO;
    return NA;
}

int elUsuarioHaGanado(char tablero[FILAS][COLUMNAS]) {
    int l;
    for (l = 0; l < FILAS; l++) {
        int m;
        for (m = 0; m < COLUMNAS; m++) {
            char actual = tablero[l][m];
            if (actual == ESPACIO_SIN_DESCUBRIR) {
                return 0;
            }
        }
    }
    return 1;
}

void imprimirTablero(char tablero[FILAS][COLUMNAS], int deberiaMostrarMinas) {
    for (int i = 0; i < FILAS; i++) {
        if (i == 0) {
            printf("   ");
            for (int k = 0; k < 6; k++) {
                printf("%i ", k + 1);
            }
            printf("\n  ____________\n");
        }

        for (int j = 0; j < COLUMNAS; j++) {
            if (j == 0) {
                printf("%i| ", i + 1);
            }

            char verdaderaLetra = ESPACIO_SIN_DESCUBRIR;
            char letraActual = tablero[i][j];
            if (letraActual == MINA) {
                verdaderaLetra = ESPACIO_SIN_DESCUBRIR;
            } else if (letraActual == ESPACIO_DESCUBIERTO) {
                // Si ya lo abrió, entonces mostramos las minas cercanas
                int minasCercanas = obtenerMinasCercanas(i, j, tablero);
                if (minasCercanas == 0) {
                    verdaderaLetra = ESPACIO_DESCUBIERTO;
                } else {
                    verdaderaLetra = minasCercanas + '0';
                }
            }
            // Si DEBUG está en 1, o debería mostrar las minas (porque perdió o ganó)
            // mostramos la mina original
            if (letraActual == MINA && (DEBUG || deberiaMostrarMinas)) {
                verdaderaLetra = MINA;
            }
            printf("%c ", verdaderaLetra);
        }
        printf("\n");
    }
    printf("\n");
}

void iniciarJuego() {
    char matrix[6][6];
    int deberiaMostrarMinas = 0;
    // Alimentar rand
    srand(getpid());
    iniciarTablero(matrix);
    colocarMinasAleatoriamente(matrix);

    while (1) {
        imprimirTablero(matrix, deberiaMostrarMinas);
        if (deberiaMostrarMinas) {
            break;
        }
        int columna;
        char fila;
        printf("Ingresa la fila: ");
        scanf(" %d", &fila);
        printf("Ingresa la columna: ");
        scanf("%d", &columna);
        int status = abrirCasilla(fila, columna, matrix);
        if (elUsuarioHaGanado(matrix)) {
            printf("Has ganado\n");
            deberiaMostrarMinas = 1;
        } else if (status == ESPACIO_YA_DESCUBIERTO) {
            printf("Ya has abierto esta casilla\n");
        } else if (status == MINA_ENCONTRADA) {
            printf("Has perdido\n");
            deberiaMostrarMinas = 1;
        }
    }
}

int main() {
    printf("*** BUSCAMINAS 6X6 ***\n\n");
    iniciarJuego();
    return 0;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// para compilar
// gcc nombre.c -> archivo a.out ---- gcc nombre -o nombreEjecutable
// ejeutar -> ingresar a la ruta
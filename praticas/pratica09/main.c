#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cobertura.h"

#define MAX_VERTICES 15
#define MAX_ARESTAS (MAX_VERTICES * (MAX_VERTICES - 1) / 2)
#define INSTANCIAS 5
#define TEMPO_MINIMO_SEGUNDOS 0.02

typedef int (*FuncaoCobertura)(Aresta *a, int m, int n, int *cobertura);

static void imprimir_cobertura(const char *nome, int *cobertura, int n, int tamanho) {
    printf("%s: tamanho %d | vertices:", nome, tamanho);

    for (int v = 0; v < n; v++) {
        if (cobertura[v]) {
            printf(" %d", v);
        }
    }

    printf("\n");
}

static void analisar(Aresta *a, int m, int n, const char *nome) {
    int cobertura[MAX_VERTICES];

    printf("\n##### %s #####\n", nome);

    imprimir_cobertura("Backtracking", cobertura, n, cobertura_minima_bt(a, m, n, cobertura));
    printf("E cobertura? %s\n", eh_cobertura(a, cobertura, m) ? "sim" : "nao");

    imprimir_cobertura("Gulosa      ", cobertura, n, cobertura_gulosa(a, m, n, cobertura));
    printf("E cobertura? %s\n", eh_cobertura(a, cobertura, m) ? "sim" : "nao");
}

/* Cada par de vertices vira uma aresta com 50% de chance. */
static int gerar_grafo_aleatorio(Aresta *a, int n) {
    int m = 0;

    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            if (rand() % 2 == 0) {
                a[m].u = u;
                a[m].v = v;
                m++;
            }
        }
    }

    return m;
}

/* Tempo medio por execucao, em ms. Repete a chamada ate acumular um tempo que o
   clock() consiga medir, ja que uma unica execucao pode ser rapida demais. */
static double medir_ms(FuncaoCobertura f, Aresta *a, int m, int n, int *cobertura, int *tamanho) {
    clock_t inicio = clock();
    long repeticoes = 0;
    double segundos;

    do {
        *tamanho = f(a, m, n, cobertura);
        repeticoes++;
        segundos = (double)(clock() - inicio) / CLOCKS_PER_SEC;
    } while (segundos < TEMPO_MINIMO_SEGUNDOS);

    return segundos * 1000.0 / (double)repeticoes;
}

static int experimento(void) {
    const int tamanhos[] = { 5, 8, 10, 12, 15 };
    const int quantidade = (int)(sizeof(tamanhos) / sizeof(tamanhos[0]));

    printf("\n##### Experimento: forca bruta x heuristica #####\n");
    printf("Media de %d grafos aleatorios por n (cada aresta com 50%% de chance)\n\n", INSTANCIAS);
    printf(" n | arestas | otimo | gulosa | maior que otimo | BT (ms)    | gulosa (ms) | BT/gulosa\n");
    printf("---+---------+-------+--------+-----------------+------------+-------------+----------\n");

    /* Semente fixa: repete os mesmos grafos a cada execucao na mesma plataforma. */
    srand(42);

    for (int i = 0; i < quantidade; i++) {
        int n = tamanhos[i];
        double soma_m = 0.0, soma_bt = 0.0, soma_gulosa = 0.0;
        double ms_bt = 0.0, ms_gulosa = 0.0;
        int acima_do_otimo = 0;

        for (int j = 0; j < INSTANCIAS; j++) {
            Aresta a[MAX_ARESTAS];
            int cobertura_bt[MAX_VERTICES];
            int cobertura_gul[MAX_VERTICES];
            int tamanho_bt, tamanho_gul;
            int m = gerar_grafo_aleatorio(a, n);

            ms_bt += medir_ms(cobertura_minima_bt, a, m, n, cobertura_bt, &tamanho_bt);
            ms_gulosa += medir_ms(cobertura_gulosa, a, m, n, cobertura_gul, &tamanho_gul);

            if (tamanho_bt < 0 || tamanho_gul < 0 ||
                !eh_cobertura(a, cobertura_bt, m) ||
                !eh_cobertura(a, cobertura_gul, m)) {
                fprintf(stderr, "Erro: resultado invalido para n = %d.\n", n);
                return 0;
            }

            soma_m += m;
            soma_bt += tamanho_bt;
            soma_gulosa += tamanho_gul;
            acima_do_otimo += tamanho_gul > tamanho_bt;
        }

        printf("%2d | %7.1f | %5.1f | %6.1f | %9d de %d  | %10.5f | %11.5f | %8.1fx\n",
               n,
               soma_m / INSTANCIAS,
               soma_bt / INSTANCIAS,
               soma_gulosa / INSTANCIAS,
               acima_do_otimo,
               INSTANCIAS,
               ms_bt / INSTANCIAS,
               ms_gulosa / INSTANCIAS,
               ms_bt / ms_gulosa);
    }

    return 1;
}

int main(void) {
    printf("========================================\n");
    printf("    PRATICA 09 - COBERTURA DE VERTICES\n");
    printf("========================================\n");

    /* Grafo da aula: V = {A, B, C, D}, E = {{A, B}, {B, C}, {C, D}}. */
    Aresta aula[] = { {0, 1}, {1, 2}, {2, 3} };
    int c1[] = { 1, 1, 0, 1 }; /* {A, B, D} */
    int c2[] = { 0, 1, 1, 0 }; /* {B, C} */
    int c3[] = { 0, 0, 1, 0 }; /* {C} */

    printf("\n=== Verificacao com eh_cobertura ===\n");
    printf("C1 = {A, B, D} e cobertura? %s\n", eh_cobertura(aula, c1, 3) ? "sim" : "nao");
    printf("C2 = {B, C} e cobertura? %s\n", eh_cobertura(aula, c2, 3) ? "sim" : "nao");
    printf("C3 = {C} e cobertura? %s\n", eh_cobertura(aula, c3, 3) ? "sim" : "nao");

    analisar(aula, 3, 4, "Grafo 1: exemplo da aula (A=0, B=1, C=2, D=3)");

    /* Caminho 3-2-0-1-4: no empate, a gulosa pega o vertice 0 primeiro e usa 3
       vertices, enquanto {1, 2} ja cobre tudo. */
    Aresta caminho[] = { {0, 1}, {0, 2}, {1, 4}, {2, 3} };
    analisar(caminho, 4, 5, "Grafo 2: caminho em que a gulosa nao e otima");

    return experimento() ? 0 : 1;
}

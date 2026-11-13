#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "hamiltoniano.h"
#include "tps.h"

#define MAX_VERTICES 12
#define INSTANCIAS 3
#define PESO_MAXIMO 100
#define TEMPO_MINIMO_SEGUNDOS 0.02

typedef int (*FuncaoTSP)(const int *dist, int n, int *rota);

/* ---------- Impressao ---------- */

static void imprimir_sequencia(const int *seq, int n, int fecha) {
    for (int i = 0; i < n; i++) {
        printf("%s%d", i > 0 ? " -> " : "", seq[i]);
    }

    if (fecha) {
        printf(" -> %d", seq[0]);
    }
}

static void imprimir_rota(const char *nome, const int *rota, int n, int custo) {
    printf("%s: custo %3d | ", nome, custo);
    imprimir_sequencia(rota, n, 1);
    printf("\n");
}

/* ---------- Parte 1: Caminho e Ciclo Hamiltoniano ---------- */

static void montar_grafo(int *adj, int n, const int arestas[][2], int m) {
    for (int i = 0; i < n * n; i++) {
        adj[i] = 0;
    }

    for (int i = 0; i < m; i++) {
        int u = arestas[i][0];
        int v = arestas[i][1];
        adj[u * n + v] = 1;
        adj[v * n + u] = 1;
    }
}

static void testar_hamiltoniano(const int *adj, int n, const char *nome) {
    int caminho[MAX_VERTICES];
    int dist[MAX_VERTICES * MAX_VERTICES];
    int rota[MAX_VERTICES];

    printf("\n##### %s (n = %d) #####\n", nome, n);

    for (int ciclo = 0; ciclo <= 1; ciclo++) {
        const char *tipo = ciclo ? "Ciclo Hamiltoniano  " : "Caminho Hamiltoniano";
        int r = encontrar_caminho_hamiltoniano_bt(adj, n, ciclo, caminho);

        if (r == 1) {
            printf("%s: ", tipo);
            imprimir_sequencia(caminho, n, ciclo);
            printf(" (valido? %s)\n", eh_caminho_hamiltoniano(adj, n, ciclo, caminho) ? "sim" : "nao");
        } else if (r == 0) {
            printf("%s: nao existe\n", tipo);
        } else {
            printf("%s: erro\n", tipo);
        }
    }

    /* Reducao: existe ciclo hamiltoniano <=> TSP otimo com pesos 1/2 custa n. */
    grafo_para_tsp(adj, n, dist);
    int custo = tsp_forca_bruta(dist, n, rota);
    printf("Reducao para TSP (peso 1 = aresta, 2 = nao aresta): otimo %d -> %s\n",
           custo, custo == n ? "tem ciclo hamiltoniano" : "nao tem ciclo hamiltoniano");
}

static void parte_hamiltoniano(void) {
    int adj[MAX_VERTICES * MAX_VERTICES];

    printf("=============== PARTE 1: CAMINHO E CICLO HAMILTONIANO ===============\n");

    /* Pentagono 0-1-2-3-4-0 com a corda 0-2: tem caminho e ciclo. */
    const int pentagono[][2] = { {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2} };
    montar_grafo(adj, 5, pentagono, 6);
    testar_hamiltoniano(adj, 5, "Pentagono com corda");

    /* Vertice 0 tem grau 1, entao nao fecha ciclo, mas 0-1-2-3 e um caminho. */
    const int bandeira[][2] = { {0, 1}, {1, 2}, {2, 3}, {3, 1} };
    montar_grafo(adj, 4, bandeira, 4);
    testar_hamiltoniano(adj, 4, "Triangulo com cauda");

    /* Estrela: o centro 0 teria de ser visitado mais de uma vez. */
    const int estrela[][2] = { {0, 1}, {0, 2}, {0, 3}, {0, 4} };
    montar_grafo(adj, 5, estrela, 4);
    testar_hamiltoniano(adj, 5, "Estrela K1,4");

    /* Grafo de Petersen: classico grafo sem ciclo hamiltoniano, mas com caminho. */
    const int petersen[][2] = {
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0},      /* ciclo externo */
        {5, 7}, {7, 9}, {9, 6}, {6, 8}, {8, 5},      /* estrela interna */
        {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9}       /* raios */
    };
    montar_grafo(adj, 10, petersen, 15);
    testar_hamiltoniano(adj, 10, "Grafo de Petersen");
}

/* ---------- Parte 2: TSP ---------- */

static void parte_tsp(void) {
    /* 5 cidades com distancias simetricas. */
    const int n = 5;
    const int dist[] = {
         0, 12, 10, 19,  8,
        12,  0,  3,  7,  2,
        10,  3,  0,  6, 20,
        19,  7,  6,  0,  4,
         8,  2, 20,  4,  0
    };
    int rota[MAX_VERTICES];

    printf("\n=============== PARTE 2: CAIXEIRO VIAJANTE (TSP) ===============\n");
    printf("\nInstancia com %d cidades (origem 0)\n", n);

    imprimir_rota("Forca bruta       ", rota, n, tsp_forca_bruta(dist, n, rota));
    imprimir_rota("Vizinho + proximo ", rota, n, tsp_vizinho_mais_proximo(dist, n, rota));
    imprimir_rota("Insercao + proximo", rota, n, tsp_insercao(dist, n, rota));
}

/* ---------- Parte 3: Experimento ---------- */

/* Grafo completo com pesos simetricos aleatorios em [1, PESO_MAXIMO]. */
static void gerar_instancia(int *dist, int n) {
    for (int u = 0; u < n; u++) {
        dist[u * n + u] = 0;

        for (int v = u + 1; v < n; v++) {
            int w = 1 + rand() % PESO_MAXIMO;
            dist[u * n + v] = w;
            dist[v * n + u] = w;
        }
    }
}

/* Tempo medio por execucao, em ms. Repete a chamada ate acumular um tempo que o
   clock() consiga medir, ja que uma unica execucao pode ser rapida demais. */
static double medir_ms(FuncaoTSP f, const int *dist, int n, int *rota, int *custo) {
    clock_t inicio = clock();
    long repeticoes = 0;
    double segundos;

    do {
        *custo = f(dist, n, rota);
        repeticoes++;
        segundos = (double)(clock() - inicio) / CLOCKS_PER_SEC;
    } while (segundos < TEMPO_MINIMO_SEGUNDOS);

    return segundos * 1000.0 / (double)repeticoes;
}

static long fatorial(int k) {
    long f = 1;

    for (int i = 2; i <= k; i++) {
        f *= i;
    }

    return f;
}

static void experimento(void) {
    const int tamanhos[] = { 5, 8, 10, 12 };
    const int quantidade = (int)(sizeof(tamanhos) / sizeof(tamanhos[0]));
    const FuncaoTSP funcoes[] = { tsp_forca_bruta, tsp_vizinho_mais_proximo, tsp_insercao };
    enum { BF, VMP, INS, ALGORITMOS };

    int dist[MAX_VERTICES * MAX_VERTICES];
    int rota[MAX_VERTICES];

    printf("\n=============== PARTE 3: EXPERIMENTO ===============\n");
    printf("Media de %d instancias aleatorias por n (pesos de 1 a %d)\n\n", INSTANCIAS, PESO_MAXIMO);
    printf(" n |   (n-1)!  | otimo | VMP   | Ins.  | VMP acima | Ins. acima | BF (ms)      | VMP (ms) | Ins. (ms)\n");
    printf("---+-----------+-------+-------+-------+-----------+------------+--------------+----------+----------\n");

    /* Semente fixa: repete as mesmas instancias a cada execucao na mesma plataforma. */
    srand(10);

    for (int t = 0; t < quantidade; t++) {
        int n = tamanhos[t];
        double custo[ALGORITMOS] = { 0 };
        double tempo[ALGORITMOS] = { 0 };

        for (int inst = 0; inst < INSTANCIAS; inst++) {
            gerar_instancia(dist, n);

            for (int a = 0; a < ALGORITMOS; a++) {
                int c;
                tempo[a] += medir_ms(funcoes[a], dist, n, rota, &c);
                custo[a] += c;
            }
        }

        for (int a = 0; a < ALGORITMOS; a++) {
            custo[a] /= INSTANCIAS;
            tempo[a] /= INSTANCIAS;
        }

        printf("%2d | %9ld | %5.1f | %5.1f | %5.1f | %8.1f%% | %9.1f%% | %12.4f | %8.4f | %8.4f\n",
               n, fatorial(n - 1), custo[BF], custo[VMP], custo[INS],
               100.0 * (custo[VMP] - custo[BF]) / custo[BF],
               100.0 * (custo[INS] - custo[BF]) / custo[BF],
               tempo[BF], tempo[VMP], tempo[INS]);
        fflush(stdout);
    }

    printf("\nA forca bruta sempre acha o otimo, mas o tempo cresce com (n-1)!:\n");
    printf("cada vertice a mais multiplica o trabalho por n. As heuristicas rodam em\n");
    printf("tempo polinomial (VMP em O(n^2), insercao em O(n^3)) e respondem em\n");
    printf("microssegundos, mas sem garantia de otimo: as rotas podem sair bem mais caras.\n");
}

int main(void) {
    parte_hamiltoniano();
    parte_tsp();
    experimento();
    return 0;
}

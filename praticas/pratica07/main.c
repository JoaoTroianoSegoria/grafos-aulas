#include <stdio.h>
#include <stdlib.h>

#include "bellman_ford.h"

static void imprimir_caminho(int origem, int v, int *pred) {
    if (v != origem) {
        imprimir_caminho(origem, pred[v], pred);
        printf(" -> ");
    }

    printf("%d", v);
}

static void analisar(GrafoListaPonderado *g, const char *nome, int origem) {
    int n = g->num_vertices;
    int *dist = malloc((size_t)n * sizeof(int));
    int *pred = malloc((size_t)n * sizeof(int));

    if (dist == NULL || pred == NULL) {
        fprintf(stderr, "Erro ao alocar memoria.\n");
        free(dist);
        free(pred);
        return;
    }

    printf("\n##### %s #####\n", nome);
    imprimir_grafo(g);
    printf("Origem: %d\n", origem);

    if (bellman_ford(g, origem, dist, pred) == 0) {
        printf("O grafo possui ciclo negativo alcancavel a partir da origem.\n");
        reconstruir_ciclo_negativo(g, pred);
    } else {
        for (int v = 0; v < n; v++) {
            printf("Vertice %d | ", v);

            if (dist[v] == INFINITO) {
                printf("dist = INF | inalcancavel\n");
            } else {
                printf("dist = %d | caminho: ", dist[v]);
                imprimir_caminho(origem, v, pred);
                printf("\n");
            }
        }
    }

    free(dist);
    free(pred);
}

int main(void) {
    printf("========================================\n");
    printf("     PRATICA 07 - BELLMAN-FORD\n");
    printf("========================================\n");

    /* Grafo 1: arestas negativas, mas nenhum ciclo negativo. */
    GrafoListaPonderado *g1 = criar_grafo(5);

    /* Grafo 2: ciclo 1 -> 2 -> 3 -> 1 com custo -3, alcancavel a partir de 0. */
    GrafoListaPonderado *g2 = criar_grafo(5);

    /* Grafo 3: ciclo 2 -> 3 -> 2 com custo -1, inalcancavel a partir de 0. */
    GrafoListaPonderado *g3 = criar_grafo(4);

    if (g1 == NULL || g2 == NULL || g3 == NULL) {
        fprintf(stderr, "Erro ao criar os grafos.\n");
        liberar_grafo(g1);
        liberar_grafo(g2);
        liberar_grafo(g3);
        return 1;
    }

    adicionar_aresta(g1, 0, 1, 6);
    adicionar_aresta(g1, 0, 3, 7);
    adicionar_aresta(g1, 1, 2, 5);
    adicionar_aresta(g1, 1, 3, 8);
    adicionar_aresta(g1, 1, 4, -4);
    adicionar_aresta(g1, 2, 1, -2);
    adicionar_aresta(g1, 3, 2, -3);
    adicionar_aresta(g1, 3, 4, 9);
    adicionar_aresta(g1, 4, 0, 2);
    adicionar_aresta(g1, 4, 2, 7);

    adicionar_aresta(g2, 0, 1, 1);
    adicionar_aresta(g2, 1, 2, -1);
    adicionar_aresta(g2, 2, 3, -1);
    adicionar_aresta(g2, 3, 1, -1);
    adicionar_aresta(g2, 3, 4, 2);

    adicionar_aresta(g3, 0, 1, 3);
    adicionar_aresta(g3, 2, 3, 1);
    adicionar_aresta(g3, 3, 2, -2);

    analisar(g1, "Grafo 1: arestas negativas sem ciclo negativo", 0);
    analisar(g2, "Grafo 2: ciclo negativo alcancavel", 0);
    analisar(g3, "Grafo 3: ciclo negativo inalcancavel", 0);

    liberar_grafo(g1);
    liberar_grafo(g2);
    liberar_grafo(g3);

    return 0;
}

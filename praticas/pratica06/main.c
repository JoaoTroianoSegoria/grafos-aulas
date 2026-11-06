#include <stdio.h>
#include <stdlib.h>

#include "dijkstra.h"

static void linha(const char *titulo) {
    printf("\n=== %s ===\n", titulo);
}

static void testar_heap(void) {
    linha("Min-Heap: inserir, diminuir chave e extrair");

    MinHeap *h = criar_min_heap(5);
    if (h == NULL) {
        fprintf(stderr, "Erro ao criar o heap.\n");
        return;
    }

    inserir_heap(h, 0, 40);
    inserir_heap(h, 1, 10);
    inserir_heap(h, 2, 30);
    inserir_heap(h, 3, 50);
    inserir_heap(h, 4, 20);

    printf("Diminuindo a chave do vertice 3 de 50 para 5\n");
    diminuir_chave(h, 3, 5);

    printf("Ordem de extracao (vertice, chave):");
    while (!heap_vazio(h)) {
        NoHeap minimo = extrair_minimo(h);
        printf(" (%d, %d)", minimo.vertice, minimo.distancia);
    }
    printf("\n");

    liberar_min_heap(h);
}

static void analisar(GrafoListaPonderado *g, int origem) {
    int n = g->num_vertices;
    int *dist = malloc((size_t)n * sizeof(int));
    int *pred = malloc((size_t)n * sizeof(int));

    if (dist == NULL || pred == NULL) {
        fprintf(stderr, "Erro ao alocar memoria.\n");
        free(dist);
        free(pred);
        return;
    }

    linha("Lista de adjacencia");
    imprimir_grafo(g);

    linha("Dijkstra");
    printf("Origem: %d\n", origem);

    if (!dijkstra(g, origem, dist, pred)) {
        printf("Dijkstra nao pode ser executado: ha aresta de custo negativo.\n");
        free(dist);
        free(pred);
        return;
    }

    for (int v = 0; v < n; v++) {
        printf("Vertice %d | ", v);

        if (dist[v] == INFINITO) {
            printf("dist = INF | ");
        } else {
            printf("dist = %d | ", dist[v]);
        }

        printf("caminho: ");
        reconstruir_caminho(origem, v, pred);
    }

    free(dist);
    free(pred);
}

int main(void) {
    printf("========================================\n");
    printf("     PRATICA 06 - DIJKSTRA COM HEAP\n");
    printf("========================================\n");

    testar_heap();

    /* Grafo 1: o vertice 5 nao e alcancavel a partir da origem 0. */
    GrafoListaPonderado *g1 = criar_grafo(6);
    if (g1 == NULL) {
        fprintf(stderr, "Erro ao criar o grafo.\n");
        return 1;
    }

    adicionar_aresta(g1, 0, 1, 10);
    adicionar_aresta(g1, 0, 3, 5);
    adicionar_aresta(g1, 1, 2, 1);
    adicionar_aresta(g1, 1, 3, 2);
    adicionar_aresta(g1, 2, 4, 4);
    adicionar_aresta(g1, 3, 1, 3);
    adicionar_aresta(g1, 3, 2, 9);
    adicionar_aresta(g1, 3, 4, 2);
    adicionar_aresta(g1, 4, 0, 7);
    adicionar_aresta(g1, 4, 2, 6);
    adicionar_aresta(g1, 5, 0, 1);

    printf("\n##### Grafo 1: custos nao-negativos #####\n");
    analisar(g1, 0);
    liberar_grafo(g1);

    /* Grafo 2: Dijkstra nao garante o resultado com custo negativo. */
    GrafoListaPonderado *g2 = criar_grafo(3);
    if (g2 == NULL) {
        fprintf(stderr, "Erro ao criar o grafo.\n");
        return 1;
    }

    adicionar_aresta(g2, 0, 1, 4);
    adicionar_aresta(g2, 0, 2, 5);
    adicionar_aresta(g2, 2, 1, -3);

    printf("\n##### Grafo 2: aresta de custo negativo #####\n");
    analisar(g2, 0);
    liberar_grafo(g2);

    return 0;
}

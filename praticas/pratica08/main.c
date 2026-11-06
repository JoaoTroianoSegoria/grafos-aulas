#include <stdio.h>
#include <stdlib.h>

#include "agm.h"

static void imprimir_resultado(
    const char *nome,
    Aresta *agm,
    int quantidade,
    int custo_total
) {
    printf("%s\n", nome);

    for (int i = 0; i < quantidade; i++) {
        printf("  %d -- %d (peso %d)\n", agm[i].u, agm[i].v, agm[i].peso);
    }

    printf("  Arestas: %d | Custo total: %d\n", quantidade, custo_total);
}

static void analisar(GrafoListaPonderado *g, const char *nome) {
    int n = g->num_vertices;
    Aresta *agm = malloc((size_t)n * sizeof(Aresta));

    if (agm == NULL) {
        fprintf(stderr, "Erro ao alocar memoria.\n");
        return;
    }

    printf("\n##### %s #####\n", nome);
    imprimir_grafo(g);

    int custo_kruskal = 0;
    int custo_prim = 0;

    printf("\n");
    int arestas_kruskal = kruskal(g, agm, &custo_kruskal);
    imprimir_resultado("Kruskal", agm, arestas_kruskal, custo_kruskal);

    int arestas_prim = prim(g, 0, agm, &custo_prim);
    imprimir_resultado("Prim (origem 0)", agm, arestas_prim, custo_prim);

    if (arestas_kruskal == n - 1) {
        printf("Grafo conexo: os dois custos %s.\n",
               custo_kruskal == custo_prim ? "coincidem" : "DIVERGEM");
    } else {
        printf("Grafo desconexo: Kruskal gerou uma floresta e Prim cobriu "
               "apenas a componente da origem.\n");
    }

    free(agm);
}

int main(void) {
    printf("========================================\n");
    printf("   PRATICA 08 - ARVORE GERADORA MINIMA\n");
    printf("========================================\n");

    printf("\n=== Union-Find ===\n");

    DSU *d = criar_dsu(5);
    if (d == NULL) {
        fprintf(stderr, "Erro ao criar o Union-Find.\n");
        return 1;
    }

    unir(d, 0, 1);
    unir(d, 2, 3);
    unir(d, 1, 3);
    printf("0 e 3 no mesmo conjunto? %s\n", find(d, 0) == find(d, 3) ? "sim" : "nao");
    printf("0 e 4 no mesmo conjunto? %s\n", find(d, 0) == find(d, 4) ? "sim" : "nao");
    printf("Unir 0 e 3 de novo mudou algo? %s\n", unir(d, 0, 3) ? "sim" : "nao");
    liberar_dsu(d);

    /* Grafo 1: conexo, com AGM de custo 37. */
    GrafoListaPonderado *g1 = criar_grafo(9);

    /* Grafo 2: duas componentes, {0, 1, 2} e {3, 4}. */
    GrafoListaPonderado *g2 = criar_grafo(5);

    if (g1 == NULL || g2 == NULL) {
        fprintf(stderr, "Erro ao criar os grafos.\n");
        liberar_grafo(g1);
        liberar_grafo(g2);
        return 1;
    }

    adicionar_aresta(g1, 0, 1, 4);
    adicionar_aresta(g1, 0, 7, 8);
    adicionar_aresta(g1, 1, 2, 8);
    adicionar_aresta(g1, 1, 7, 11);
    adicionar_aresta(g1, 2, 3, 7);
    adicionar_aresta(g1, 2, 5, 4);
    adicionar_aresta(g1, 2, 8, 2);
    adicionar_aresta(g1, 3, 4, 9);
    adicionar_aresta(g1, 3, 5, 14);
    adicionar_aresta(g1, 4, 5, 10);
    adicionar_aresta(g1, 5, 6, 2);
    adicionar_aresta(g1, 6, 7, 1);
    adicionar_aresta(g1, 6, 8, 6);
    adicionar_aresta(g1, 7, 8, 7);

    adicionar_aresta(g2, 0, 1, 3);
    adicionar_aresta(g2, 1, 2, 1);
    adicionar_aresta(g2, 0, 2, 2);
    adicionar_aresta(g2, 3, 4, 5);

    analisar(g1, "Grafo 1: conexo");
    analisar(g2, "Grafo 2: desconexo");

    liberar_grafo(g1);
    liberar_grafo(g2);

    return 0;
}

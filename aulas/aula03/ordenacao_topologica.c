#include <stdlib.h>
#include "grafo_lista.h"
#include "ordenacao_topologica.h"

void ordenar_dfs(GrafoLista *g, int *visitado, int *ordenacao) {
    int idx = 0;

    // uma DFS por vertice ainda nao visitado, para cobrir o digrafo inteiro
    for (int i = 0; i < g->num_vertices; i++) {
        if (!visitado[i]) {
            buscar_dfs(g, i, visitado, &idx, ordenacao);
        }
    }

    // a pos-ordem sai invertida: quem termina por ultimo vem primeiro
    for (int i = 0; i < g->num_vertices / 2; i++) {
        int j = g->num_vertices - 1 - i;
        int troca = ordenacao[i];
        ordenacao[i] = ordenacao[j];
        ordenacao[j] = troca;
    }
}

void buscar_dfs(GrafoLista *g, int u, int *visitado, int *idx, int *ordenacao) {
    visitado[u] = 1;

    No *no = g->lista[u];
    while (no != NULL)
    {
        int v = no->vertice;
        if (!visitado[v])
            buscar_dfs(g, v, visitado, idx, ordenacao);
        no = no->proximo;
    }

    // u so entra na lista depois de todos os seus sucessores
    ordenacao[(*idx)++] = u;
}

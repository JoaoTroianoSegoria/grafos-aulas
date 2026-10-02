#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"


void dfs(GrafoLista *g, int u, int *visitado) {
    visitado[u] = 1;
    printf("Empilha %i, Visita %i\n", u+1, u+1);
    No *no = g->lista[u];
    while (no != NULL) {
        int v = no->vertice;
        if (!visitado[v]) dfs(g, v, visitado); // Pilha de recursividade
        no = no->proximo;
    }
    printf("Desempliha %i\n", u+1);
}

void bfs(GrafoLista *g, int u, int *visitado) {
    int fila[10];
    int inicio = 0;
    int final = 0;

    visitado[u] = 1;
    printf(" Visita %i, Enfileira %i\n", u+1, u+1);
    fila[final++] = u;
    while(inicio < final) {
        int i = fila[inicio++];
        printf("Desinfilera %i\n", i+1);
        No *no = g->lista[u];
        while(no != NULL) {
            int v = no->vertice;
            if (!visitado[v]) {
                visitado[v] = 1;
                printf(" Visita %i, Enfileira %i\n", v+1, v+1);
                fila[final++] = v;
            }
            no = no->proximo;
        }
    }

}

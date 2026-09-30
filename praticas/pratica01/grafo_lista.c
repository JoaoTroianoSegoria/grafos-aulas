#include <stdio.h>
#include <stdlib.h>
#include "grafo_lista.h"

GrafoLista* criar_grafo_lista(int n) {
    GrafoLista *grafo = malloc(sizeof(GrafoLista));
    grafo->n = n;
    grafo->adj = calloc(n, sizeof(No *));
    return grafo;
}

void liberar_grafo_lista(GrafoLista *grafo) {
    for (int i = 0; i < grafo->n; i++) {
        No *atual = grafo->adj[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp);
        }
    }
    free(grafo->adj);
    free(grafo);
}

void inserir_aresta_lista(GrafoLista *grafo, int u, int v) {
    No *novo = malloc(sizeof(No));
    novo->destino = v;
    novo->prox = grafo->adj[u];
    grafo->adj[u] = novo;

    novo = malloc(sizeof(No));
    novo->destino = u;
    novo->prox = grafo->adj[v];
    grafo->adj[v] = novo;
}

void remover_aresta_lista(GrafoLista *grafo, int u, int v) {
    No *atual = grafo->adj[u];
    No *ant = NULL;
    while (atual != NULL && atual->destino != v) {
        ant = atual;
        atual = atual->prox;
    }
    if (atual != NULL) {
        if (ant == NULL) grafo->adj[u] = atual->prox;
        else ant->prox = atual->prox;
        free(atual);
    }

    atual = grafo->adj[v];
    ant = NULL;
    while (atual != NULL && atual->destino != u) {
        ant = atual;
        atual = atual->prox;
    }
    if (atual != NULL) {
        if (ant == NULL) grafo->adj[v] = atual->prox;
        else ant->prox = atual->prox;
        free(atual);
    }
}

int grau_lista(GrafoLista *grafo, int v) {
    int g = 0;
    No *atual = grafo->adj[v];
    while (atual != NULL) {
        g++;
        atual = atual->prox;
    }
    return g;
}

int sao_adjacentes_lista(GrafoLista *grafo, int u, int v) {
    No *atual = grafo->adj[u];
    while (atual != NULL) {
        if (atual->destino == v) return 1;
        atual = atual->prox;
    }
    return 0;
}

void exibir_lista(GrafoLista *grafo) {
    for (int i = 0; i < grafo->n; i++) {
        printf("%d: ", i);
        No *atual = grafo->adj[i];
        while (atual != NULL) {
            printf("→ %d ", atual->destino);
            atual = atual->prox;
        }
        printf("→ NULL\n");
    }
}
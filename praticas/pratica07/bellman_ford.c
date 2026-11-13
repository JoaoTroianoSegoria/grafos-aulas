#include "bellman_ford.h"

#include <stdio.h>
#include <stdlib.h>

/* --- Grafo --- */

GrafoListaPonderado *criar_grafo(int n) {
    if (n <= 0) {
        return NULL;
    }

    GrafoListaPonderado *g = malloc(sizeof(GrafoListaPonderado));
    if (g == NULL) {
        return NULL;
    }

    g->num_vertices = n;
    g->lista = calloc((size_t)n, sizeof(No *));

    if (g->lista == NULL) {
        free(g);
        return NULL;
    }

    return g;
}

void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso) {
    if (g == NULL ||
        u < 0 || u >= g->num_vertices ||
        v < 0 || v >= g->num_vertices) {
        return;
    }

    No *novo = malloc(sizeof(No));
    if (novo == NULL) {
        return;
    }

    novo->vertice = v;
    novo->peso = peso;
    novo->proximo = g->lista[u];
    g->lista[u] = novo;
}

void liberar_grafo(GrafoListaPonderado *g) {
    if (g == NULL) {
        return;
    }

    for (int i = 0; i < g->num_vertices; i++) {
        No *atual = g->lista[i];

        while (atual != NULL) {
            No *temp = atual;
            atual = atual->proximo;
            free(temp);
        }
    }

    free(g->lista);
    free(g);
}

void imprimir_grafo(GrafoListaPonderado *g) {
    if (g == NULL) {
        return;
    }

    for (int u = 0; u < g->num_vertices; u++) {
        printf("  %d:", u);

        for (No *a = g->lista[u]; a != NULL; a = a->proximo) {
            printf(" -> %d (%d)", a->vertice, a->peso);
        }

        printf("\n");
    }
}

/* --- Bellman-Ford --- */

/* Relaxa todas as arestas uma vez e retorna 1 se alguma distancia mudou. */
static int relaxar_arestas(GrafoListaPonderado *g, int *dist, int *pred) {
    int mudou = 0;

    for (int u = 0; u < g->num_vertices; u++) {
        if (dist[u] == INFINITO) {
            continue;
        }

        for (No *a = g->lista[u]; a != NULL; a = a->proximo) {
            /* Evita estouro de inteiro na soma abaixo. */
            if (a->peso > 0 && dist[u] > 0 && a->peso >= INFINITO - dist[u]) {
                continue;
            }

            if (dist[u] + a->peso < dist[a->vertice]) {
                dist[a->vertice] = dist[u] + a->peso;
                pred[a->vertice] = u;
                mudou = 1;
            }
        }
    }

    return mudou;
}

int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    if (g == NULL || dist == NULL || pred == NULL ||
        origem < 0 || origem >= g->num_vertices) {
        return -1;
    }

    int n = g->num_vertices;

    for (int v = 0; v < n; v++) {
        dist[v] = INFINITO;
        pred[v] = -1;
    }

    dist[origem] = 0;

    for (int i = 1; i <= n - 1; i++) {
        if (!relaxar_arestas(g, dist, pred)) {
            return 1; /* nada mudou: as distancias ja sao definitivas */
        }
    }

    /* n-esima iteracao: qualquer melhora agora indica ciclo negativo. */
    return relaxar_arestas(g, dist, pred) ? 0 : 1;
}

/* Menor peso entre as arestas u -> v (pode haver arestas paralelas). */
static int menor_peso_aresta(GrafoListaPonderado *g, int u, int v) {
    int menor = INFINITO;

    for (No *a = g->lista[u]; a != NULL; a = a->proximo) {
        if (a->vertice == v && a->peso < menor) {
            menor = a->peso;
        }
    }

    return menor;
}

int reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *pred) {
    if (g == NULL || pred == NULL) {
        return 0;
    }

    int n = g->num_vertices;
    int no_ciclo = -1;

    /* Quem anda n passos sem chegar em -1 esta, com certeza, dentro de um ciclo:
       com n vertices, a parte do trajeto antes do ciclo tem no maximo n-1 passos. */
    for (int inicio = 0; inicio < n && no_ciclo == -1; inicio++) {
        int atual = inicio;

        for (int i = 0; i < n && atual != -1; i++) {
            atual = pred[atual];
        }

        no_ciclo = atual;
    }

    if (no_ciclo == -1) {
        printf("Nenhum ciclo encontrado.\n");
        return 0;
    }

    int *ciclo = malloc((size_t)n * sizeof(int));
    if (ciclo == NULL) {
        return 0;
    }

    int tamanho = 0;
    int atual = no_ciclo;

    do {
        ciclo[tamanho++] = atual;
        atual = pred[atual];
    } while (atual != no_ciclo);

    /* pred[] aponta para tras: o vetor invertido segue a direcao das arestas. */
    int custo = 0;

    printf("Ciclo negativo: ");
    for (int i = tamanho - 1; i >= 0; i--) {
        int proximo = (i > 0) ? ciclo[i - 1] : ciclo[tamanho - 1];

        custo += menor_peso_aresta(g, ciclo[i], proximo);
        printf("%d -> ", ciclo[i]);
    }
    printf("%d (custo total %d)\n", ciclo[tamanho - 1], custo);

    free(ciclo);

    return tamanho;
}

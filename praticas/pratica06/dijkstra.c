#include "dijkstra.h"

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

/* --- Min-Heap --- */

MinHeap *criar_min_heap(int capacidade) {
    if (capacidade <= 0) {
        return NULL;
    }

    MinHeap *h = malloc(sizeof(MinHeap));
    if (h == NULL) {
        return NULL;
    }

    h->dados = malloc((size_t)capacidade * sizeof(NoHeap));
    h->posicao = malloc((size_t)capacidade * sizeof(int));

    if (h->dados == NULL || h->posicao == NULL) {
        free(h->dados);
        free(h->posicao);
        free(h);
        return NULL;
    }

    h->capacidade = capacidade;
    h->tamanho = 0;

    for (int i = 0; i < capacidade; i++) {
        h->posicao[i] = -1;
    }

    return h;
}

void liberar_min_heap(MinHeap *h) {
    if (h == NULL) {
        return;
    }

    free(h->dados);
    free(h->posicao);
    free(h);
}

int heap_vazio(MinHeap *h) {
    return h == NULL || h->tamanho == 0;
}

static void trocar(MinHeap *h, int i, int j) {
    NoHeap temp = h->dados[i];
    h->dados[i] = h->dados[j];
    h->dados[j] = temp;

    h->posicao[h->dados[i].vertice] = i;
    h->posicao[h->dados[j].vertice] = j;
}

static void subir(MinHeap *h, int i) {
    while (i > 0) {
        int pai = (i - 1) / 2;

        if (h->dados[pai].distancia <= h->dados[i].distancia) {
            break;
        }

        trocar(h, i, pai);
        i = pai;
    }
}

static void descer(MinHeap *h, int i) {
    while (1) {
        int esquerdo = 2 * i + 1;
        int direito = 2 * i + 2;
        int menor = i;

        if (esquerdo < h->tamanho &&
            h->dados[esquerdo].distancia < h->dados[menor].distancia) {
            menor = esquerdo;
        }

        if (direito < h->tamanho &&
            h->dados[direito].distancia < h->dados[menor].distancia) {
            menor = direito;
        }

        if (menor == i) {
            break;
        }

        trocar(h, i, menor);
        i = menor;
    }
}

int inserir_heap(MinHeap *h, int vertice, int distancia) {
    if (h == NULL || h->tamanho == h->capacidade ||
        vertice < 0 || vertice >= h->capacidade ||
        h->posicao[vertice] != -1) {
        return 0;
    }

    int i = h->tamanho++;

    h->dados[i].vertice = vertice;
    h->dados[i].distancia = distancia;
    h->posicao[vertice] = i;

    subir(h, i);

    return 1;
}

NoHeap extrair_minimo(MinHeap *h) {
    NoHeap minimo = { -1, INFINITO };

    if (heap_vazio(h)) {
        return minimo;
    }

    minimo = h->dados[0];
    h->tamanho--;

    if (h->tamanho > 0) {
        h->dados[0] = h->dados[h->tamanho];
        h->posicao[h->dados[0].vertice] = 0;
    }

    h->posicao[minimo.vertice] = -1;

    descer(h, 0);

    return minimo;
}

void diminuir_chave(MinHeap *h, int vertice, int nova_distancia) {
    if (h == NULL || vertice < 0 || vertice >= h->capacidade) {
        return;
    }

    int i = h->posicao[vertice];

    if (i == -1 || nova_distancia >= h->dados[i].distancia) {
        return;
    }

    h->dados[i].distancia = nova_distancia;
    subir(h, i);
}

/* --- Dijkstra --- */

static int possui_custo_negativo(GrafoListaPonderado *g) {
    for (int u = 0; u < g->num_vertices; u++) {
        for (No *a = g->lista[u]; a != NULL; a = a->proximo) {
            if (a->peso < 0) {
                return 1;
            }
        }
    }

    return 0;
}

int dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred) {
    if (g == NULL || dist == NULL || pred == NULL ||
        origem < 0 || origem >= g->num_vertices ||
        possui_custo_negativo(g)) {
        return 0;
    }

    MinHeap *h = criar_min_heap(g->num_vertices);
    if (h == NULL) {
        return 0;
    }

    for (int v = 0; v < g->num_vertices; v++) {
        dist[v] = INFINITO;
        pred[v] = -1;
    }

    dist[origem] = 0;
    inserir_heap(h, origem, 0);

    while (!heap_vazio(h)) {
        int u = extrair_minimo(h).vertice;

        for (No *a = g->lista[u]; a != NULL; a = a->proximo) {
            int v = a->vertice;

            /* Evita estouro de inteiro na soma abaixo. */
            if (a->peso >= INFINITO - dist[u]) {
                continue;
            }

            int nova_distancia = dist[u] + a->peso;

            /* Com custos nao-negativos, um vertice ja extraido nunca melhora,
               entao posicao[v] == -1 aqui significa "ainda nao descoberto". */
            if (nova_distancia < dist[v]) {
                dist[v] = nova_distancia;
                pred[v] = u;

                if (h->posicao[v] == -1) {
                    inserir_heap(h, v, nova_distancia);
                } else {
                    diminuir_chave(h, v, nova_distancia);
                }
            }
        }
    }

    liberar_min_heap(h);

    return 1;
}

static void imprimir_caminho(int origem, int v, int *pred) {
    if (v != origem) {
        imprimir_caminho(origem, pred[v], pred);
        printf(" -> ");
    }

    printf("%d", v);
}

int reconstruir_caminho(int origem, int destino, int *pred) {
    if (pred == NULL) {
        return 0;
    }

    if (destino != origem && pred[destino] == -1) {
        printf("Nao ha caminho de %d ate %d\n", origem, destino);
        return 0;
    }

    imprimir_caminho(origem, destino, pred);
    printf("\n");

    return 1;
}

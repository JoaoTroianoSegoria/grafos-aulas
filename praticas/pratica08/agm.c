#include "agm.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define INFINITO INT_MAX

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

static void inserir_na_lista(GrafoListaPonderado *g, int u, int v, int peso) {
    No *novo = malloc(sizeof(No));
    if (novo == NULL) {
        return;
    }

    novo->vertice = v;
    novo->peso = peso;
    novo->proximo = g->lista[u];
    g->lista[u] = novo;
}

void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso) {
    if (g == NULL || u == v ||
        u < 0 || u >= g->num_vertices ||
        v < 0 || v >= g->num_vertices) {
        return;
    }

    inserir_na_lista(g, u, v, peso);
    inserir_na_lista(g, v, u, peso);
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
            printf(" %d (%d)", a->vertice, a->peso);
        }

        printf("\n");
    }
}

/* --- Union-Find --- */

DSU *criar_dsu(int n) {
    if (n <= 0) {
        return NULL;
    }

    DSU *d = malloc(sizeof(DSU));
    if (d == NULL) {
        return NULL;
    }

    d->pai = malloc((size_t)n * sizeof(int));
    d->rank = calloc((size_t)n, sizeof(int));

    if (d->pai == NULL || d->rank == NULL) {
        free(d->pai);
        free(d->rank);
        free(d);
        return NULL;
    }

    d->n = n;

    for (int i = 0; i < n; i++) {
        d->pai[i] = i;
    }

    return d;
}

void liberar_dsu(DSU *d) {
    if (d == NULL) {
        return;
    }

    free(d->pai);
    free(d->rank);
    free(d);
}

int find(DSU *d, int x) {
    if (d->pai[x] != x) {
        d->pai[x] = find(d, d->pai[x]);
    }

    return d->pai[x];
}

int unir(DSU *d, int x, int y) {
    int raiz_x = find(d, x);
    int raiz_y = find(d, y);

    if (raiz_x == raiz_y) {
        return 0;
    }

    if (d->rank[raiz_x] < d->rank[raiz_y]) {
        d->pai[raiz_x] = raiz_y;
    } else if (d->rank[raiz_x] > d->rank[raiz_y]) {
        d->pai[raiz_y] = raiz_x;
    } else {
        d->pai[raiz_y] = raiz_x;
        d->rank[raiz_x]++;
    }

    return 1;
}

/* --- Kruskal --- */

static int comparar_arestas(const void *a, const void *b) {
    const Aresta *x = a;
    const Aresta *y = b;

    if (x->peso != y->peso) {
        return (x->peso > y->peso) - (x->peso < y->peso);
    }

    if (x->u != y->u) {
        return x->u - y->u;
    }

    return x->v - y->v;
}

int kruskal(GrafoListaPonderado *g, Aresta *agm, int *custo_total) {
    if (g == NULL || agm == NULL || custo_total == NULL) {
        return -1;
    }

    int n = g->num_vertices;
    int m = 0;

    /* Cada aresta aparece duas vezes nas listas: conta so a que tem u < v. */
    for (int u = 0; u < n; u++) {
        for (No *a = g->lista[u]; a != NULL; a = a->proximo) {
            if (u < a->vertice) {
                m++;
            }
        }
    }

    Aresta *arestas = malloc((size_t)(m > 0 ? m : 1) * sizeof(Aresta));
    DSU *d = criar_dsu(n);

    if (arestas == NULL || d == NULL) {
        free(arestas);
        liberar_dsu(d);
        return -1;
    }

    int i = 0;

    for (int u = 0; u < n; u++) {
        for (No *a = g->lista[u]; a != NULL; a = a->proximo) {
            if (u < a->vertice) {
                arestas[i].u = u;
                arestas[i].v = a->vertice;
                arestas[i].peso = a->peso;
                i++;
            }
        }
    }

    qsort(arestas, (size_t)m, sizeof(Aresta), comparar_arestas);

    int escolhidas = 0;

    *custo_total = 0;

    for (i = 0; i < m && escolhidas < n - 1; i++) {
        if (unir(d, arestas[i].u, arestas[i].v)) {
            agm[escolhidas++] = arestas[i];
            *custo_total += arestas[i].peso;
        }
    }

    free(arestas);
    liberar_dsu(d);

    return escolhidas;
}

/* --- Min-Heap usado pelo Prim --- */

typedef struct {
    int vertice;
    int chave;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; /* mapeia vertice -> indice no heap (-1 se estiver fora) */
} MinHeap;

static MinHeap *criar_min_heap(int capacidade) {
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

static void liberar_min_heap(MinHeap *h) {
    if (h == NULL) {
        return;
    }

    free(h->dados);
    free(h->posicao);
    free(h);
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

        if (h->dados[pai].chave <= h->dados[i].chave) {
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
            h->dados[esquerdo].chave < h->dados[menor].chave) {
            menor = esquerdo;
        }

        if (direito < h->tamanho &&
            h->dados[direito].chave < h->dados[menor].chave) {
            menor = direito;
        }

        if (menor == i) {
            break;
        }

        trocar(h, i, menor);
        i = menor;
    }
}

static void inserir_heap(MinHeap *h, int vertice, int chave) {
    int i = h->tamanho++;

    h->dados[i].vertice = vertice;
    h->dados[i].chave = chave;
    h->posicao[vertice] = i;

    subir(h, i);
}

static int extrair_minimo(MinHeap *h) {
    int vertice = h->dados[0].vertice;

    h->tamanho--;

    if (h->tamanho > 0) {
        h->dados[0] = h->dados[h->tamanho];
        h->posicao[h->dados[0].vertice] = 0;
    }

    h->posicao[vertice] = -1;

    descer(h, 0);

    return vertice;
}

static void diminuir_chave(MinHeap *h, int vertice, int nova_chave) {
    int i = h->posicao[vertice];

    h->dados[i].chave = nova_chave;
    subir(h, i);
}

/* --- Prim --- */

int prim(GrafoListaPonderado *g, int origem, Aresta *agm, int *custo_total) {
    if (g == NULL || agm == NULL || custo_total == NULL ||
        origem < 0 || origem >= g->num_vertices) {
        return -1;
    }

    int n = g->num_vertices;
    int *chave = malloc((size_t)n * sizeof(int));
    int *pai = malloc((size_t)n * sizeof(int));
    int *na_agm = calloc((size_t)n, sizeof(int));
    MinHeap *h = criar_min_heap(n);

    if (chave == NULL || pai == NULL || na_agm == NULL || h == NULL) {
        free(chave);
        free(pai);
        free(na_agm);
        liberar_min_heap(h);
        return -1;
    }

    for (int v = 0; v < n; v++) {
        chave[v] = INFINITO;
        pai[v] = -1;
    }

    chave[origem] = 0;
    inserir_heap(h, origem, 0);

    int escolhidas = 0;

    *custo_total = 0;

    while (h->tamanho > 0) {
        int u = extrair_minimo(h);

        na_agm[u] = 1;

        if (pai[u] != -1) {
            agm[escolhidas].u = pai[u];
            agm[escolhidas].v = u;
            agm[escolhidas].peso = chave[u];
            *custo_total += chave[u];
            escolhidas++;
        }

        for (No *a = g->lista[u]; a != NULL; a = a->proximo) {
            int v = a->vertice;

            if (!na_agm[v] && a->peso < chave[v]) {
                chave[v] = a->peso;
                pai[v] = u;

                if (h->posicao[v] == -1) {
                    inserir_heap(h, v, a->peso);
                } else {
                    diminuir_chave(h, v, a->peso);
                }
            }
        }
    }

    free(chave);
    free(pai);
    free(na_agm);
    liberar_min_heap(h);

    return escolhidas;
}

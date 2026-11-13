#include "cobertura.h"

#include <stdlib.h>
#include <string.h>

int eh_cobertura(Aresta *a, int *cobertura, int m) {
    for (int i = 0; i < m; i++) {
        if (!cobertura[a[i].u] && !cobertura[a[i].v]) {
            return 0;
        }
    }

    return 1;
}

/* Dados que permanecem iguais durante a recursao do backtracking. */
typedef struct {
    Aresta *arestas;
    int m;
    int n;
    int *atual;
    int *melhor;
    int tamanho_melhor;
} Busca;

static void backtracking(Busca *b, int v, int tamanho) {
    /* Poda por limite: acrescentar vertices nunca reduz o tamanho, entao este
       ramo nao tem como superar a melhor cobertura ja encontrada. */
    if (tamanho >= b->tamanho_melhor) {
        return;
    }

    if (v == b->n) {
        if (eh_cobertura(b->arestas, b->atual, b->m)) {
            b->tamanho_melhor = tamanho;
            memcpy(b->melhor, b->atual, (size_t)b->n * sizeof(int));
        }
        return;
    }

    b->atual[v] = 1;
    backtracking(b, v + 1, tamanho + 1);

    b->atual[v] = 0;
    backtracking(b, v + 1, tamanho);
}

int cobertura_minima_bt(Aresta *a, int m, int n, int *cobertura) {
    if (a == NULL || cobertura == NULL || m < 0 || n <= 0) {
        return -1;
    }

    int *atual = malloc((size_t)n * sizeof(int));
    if (atual == NULL) {
        return -1;
    }

    Busca busca = { a, m, n, atual, cobertura, n };

    /* Todos os vertices juntos sempre formam uma cobertura: e o limite inicial. */
    for (int v = 0; v < n; v++) {
        cobertura[v] = 1;
    }

    backtracking(&busca, 0, 0);

    free(atual);

    return busca.tamanho_melhor;
}

int cobertura_gulosa(Aresta *a, int m, int n, int *cobertura) {
    if (a == NULL || cobertura == NULL || m < 0 || n <= 0) {
        return -1;
    }

    int *grau = malloc((size_t)n * sizeof(int));
    if (grau == NULL) {
        return -1;
    }

    int tamanho = 0;

    memset(cobertura, 0, (size_t)n * sizeof(int));

    while (1) {
        memset(grau, 0, (size_t)n * sizeof(int));

        /* O grau restante considera apenas as arestas ainda descobertas. */
        for (int i = 0; i < m; i++) {
            int u = a[i].u;
            int v = a[i].v;

            if (!cobertura[u] && !cobertura[v]) {
                grau[u]++;
                if (v != u) {
                    grau[v]++;
                }
            }
        }

        int escolhido = 0;

        for (int v = 1; v < n; v++) {
            if (grau[v] > grau[escolhido]) {
                escolhido = v;
            }
        }

        if (grau[escolhido] == 0) {
            break; /* nenhuma aresta descoberta */
        }

        cobertura[escolhido] = 1;
        tamanho++;
    }

    free(grau);

    return tamanho;
}

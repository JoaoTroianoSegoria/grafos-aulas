#include <stdlib.h>

#include "hamiltoniano.h"

/* Estado compartilhado pela recursao, para nao repetir os mesmos parametros. */
typedef struct {
    const int *adj;
    int n;
    int ciclo;
    int *caminho;
    int *usado;
} Busca;

/* Tenta preencher caminho[pos..n-1]. caminho[0..pos-1] ja e um caminho valido. */
static int estender(Busca *b, int pos) {
    int n = b->n;

    if (pos == n) {
        /* Todos os vertices foram usados; no ciclo falta a aresta de volta. */
        return !b->ciclo || b->adj[b->caminho[n - 1] * n + b->caminho[0]];
    }

    int anterior = b->caminho[pos - 1];

    for (int v = 0; v < n; v++) {
        if (!b->usado[v] && b->adj[anterior * n + v]) {
            b->usado[v] = 1;
            b->caminho[pos] = v;

            if (estender(b, pos + 1)) {
                return 1;
            }

            b->usado[v] = 0; /* desfaz a escolha (backtracking) */
        }
    }

    return 0;
}

int encontrar_caminho_hamiltoniano_bt(const int *adj, int n, int ciclo, int *caminho) {
    if (adj == NULL || caminho == NULL || n <= 0) {
        return -1;
    }

    /* Em grafo simples, um ciclo precisa de pelo menos 3 vertices. */
    if (ciclo && n < 3) {
        return 0;
    }

    int *usado = calloc((size_t)n, sizeof(int));

    if (usado == NULL) {
        return -1;
    }

    Busca b = { adj, n, ciclo, caminho, usado };
    int encontrou = 0;
    int inicios = ciclo ? 1 : n;

    for (int s = 0; s < inicios && !encontrou; s++) {
        usado[s] = 1;
        caminho[0] = s;
        encontrou = estender(&b, 1);
        usado[s] = 0;
    }

    free(usado);
    return encontrou;
}

int eh_caminho_hamiltoniano(const int *adj, int n, int ciclo, const int *caminho) {
    if (adj == NULL || caminho == NULL || n <= 0 || (ciclo && n < 3)) {
        return 0;
    }

    int *visto = calloc((size_t)n, sizeof(int));

    if (visto == NULL) {
        return 0;
    }

    int valido = 1;

    for (int i = 0; i < n && valido; i++) {
        int v = caminho[i];

        if (v < 0 || v >= n || visto[v]) {
            valido = 0;
        } else {
            visto[v] = 1;

            if (i > 0 && !adj[caminho[i - 1] * n + v]) {
                valido = 0;
            }
        }
    }

    if (valido && ciclo && !adj[caminho[n - 1] * n + caminho[0]]) {
        valido = 0;
    }

    free(visto);
    return valido;
}

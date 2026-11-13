#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "tps.h"

int tsp_custo_rota(const int *dist, int n, const int *rota) {
    int custo = 0;

    for (int i = 0; i < n; i++) {
        custo += dist[rota[i] * n + rota[(i + 1) % n]];
    }

    return custo;
}

/* ---------- Forca bruta ---------- */

typedef struct {
    const int *dist;
    int n;
    int *atual;      /* permutacao sendo montada; atual[0] = 0 fica fixo */
    int *melhor;
    int melhor_custo;
} Permutacao;

/* Gera todas as permutacoes de atual[pos..n-1] por trocas sucessivas.
   custo e o custo acumulado do trecho atual[0..pos-1]. Nao ha poda: as
   (n - 1)! rotas sao todas avaliadas, como pede a forca bruta. */
static void permutar(Permutacao *p, int pos, int custo) {
    int n = p->n;
    int *a = p->atual;

    if (pos == n) {
        int total = custo + p->dist[a[n - 1] * n + a[0]];

        if (total < p->melhor_custo) {
            p->melhor_custo = total;
            memcpy(p->melhor, a, (size_t)n * sizeof(int));
        }
        return;
    }

    for (int i = pos; i < n; i++) {
        int tmp = a[pos];
        a[pos] = a[i];
        a[i] = tmp;

        permutar(p, pos + 1, custo + p->dist[a[pos - 1] * n + a[pos]]);

        tmp = a[pos];
        a[pos] = a[i];
        a[i] = tmp;
    }
}

int tsp_forca_bruta(const int *dist, int n, int *rota) {
    if (dist == NULL || rota == NULL || n <= 0) {
        return -1;
    }

    int *atual = malloc((size_t)n * sizeof(int));

    if (atual == NULL) {
        return -1;
    }

    for (int i = 0; i < n; i++) {
        atual[i] = i;
    }

    Permutacao p = { dist, n, atual, rota, INT_MAX };
    permutar(&p, 1, 0);

    free(atual);
    return p.melhor_custo;
}

/* ---------- Vizinho Mais Proximo ---------- */

int tsp_vizinho_mais_proximo(const int *dist, int n, int *rota) {
    if (dist == NULL || rota == NULL || n <= 0) {
        return -1;
    }

    int *visitado = calloc((size_t)n, sizeof(int));

    if (visitado == NULL) {
        return -1;
    }

    rota[0] = 0;
    visitado[0] = 1;

    for (int i = 1; i < n; i++) {
        int atual = rota[i - 1];
        int proximo = -1;

        for (int v = 0; v < n; v++) {
            if (!visitado[v] && (proximo == -1 || dist[atual * n + v] < dist[atual * n + proximo])) {
                proximo = v;
            }
        }

        rota[i] = proximo;
        visitado[proximo] = 1;
    }

    free(visitado);
    return tsp_custo_rota(dist, n, rota);
}

/* ---------- Insercao do Mais Proximo ---------- */

int tsp_insercao(const int *dist, int n, int *rota) {
    if (dist == NULL || rota == NULL || n <= 0) {
        return -1;
    }

    rota[0] = 0;

    if (n == 1) {
        return 0;
    }

    int *na_rota = calloc((size_t)n, sizeof(int));
    /* dmin[v]: menor distancia de v (fora da rota) ate alguma cidade da rota. */
    int *dmin = malloc((size_t)n * sizeof(int));

    if (na_rota == NULL || dmin == NULL) {
        free(na_rota);
        free(dmin);
        return -1;
    }

    na_rota[0] = 1;

    for (int v = 0; v < n; v++) {
        dmin[v] = dist[0 * n + v];
    }

    int tamanho = 1;

    while (tamanho < n) {
        /* Selecao: cidade fora da rota mais proxima da rota. */
        int k = -1;

        for (int v = 0; v < n; v++) {
            if (!na_rota[v] && (k == -1 || dmin[v] < dmin[k])) {
                k = v;
            }
        }

        /* Insercao: entre rota[i] e rota[i + 1] onde o aumento de custo e menor.
           Com uma so cidade na rota, so existe a posicao 1 (ciclo 0 -> k -> 0). */
        int melhor_pos = tamanho;
        int melhor_aumento = INT_MAX;

        if (tamanho > 1) {
            for (int i = 0; i < tamanho; i++) {
                int a = rota[i];
                int b = rota[(i + 1) % tamanho];
                int aumento = dist[a * n + k] + dist[k * n + b] - dist[a * n + b];

                if (aumento < melhor_aumento) {
                    melhor_aumento = aumento;
                    melhor_pos = i + 1;
                }
            }
        }

        /* Abre espaco e insere k na posicao escolhida. */
        for (int j = tamanho; j > melhor_pos; j--) {
            rota[j] = rota[j - 1];
        }
        rota[melhor_pos] = k;
        tamanho++;
        na_rota[k] = 1;

        for (int v = 0; v < n; v++) {
            if (!na_rota[v] && dist[k * n + v] < dmin[v]) {
                dmin[v] = dist[k * n + v];
            }
        }
    }

    free(na_rota);
    free(dmin);
    return tsp_custo_rota(dist, n, rota);
}

/* ---------- Reducao Ciclo Hamiltoniano -> TSP ---------- */

int grafo_para_tsp(const int *adj, int n, int *dist) {
    if (adj == NULL || dist == NULL || n <= 0) {
        return -1;
    }

    for (int u = 0; u < n; u++) {
        for (int v = 0; v < n; v++) {
            if (u == v) {
                dist[u * n + v] = 0;
            } else {
                dist[u * n + v] = adj[u * n + v] ? 1 : 2;
            }
        }
    }

    return 0;
}

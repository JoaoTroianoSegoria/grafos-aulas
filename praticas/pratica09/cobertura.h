#ifndef COBERTURA_H
#define COBERTURA_H

/* Pratica 09 - Cobertura de Vertices.
   O grafo e nao direcionado e representado pela lista das suas m arestas.
   cobertura[v] vale 1 se o vertice v pertence a cobertura e 0 caso contrario. */

typedef struct {
    int u;
    int v;
} Aresta;

/* Retorna 1 se todas as arestas tem ao menos uma ponta na cobertura. */
int eh_cobertura(Aresta *a, int *cobertura, int m);

/* Forca bruta com backtracking: decide, vertice a vertice, se ele entra ou nao.
   A poda por limite abandona o ramo que nao consegue superar a melhor cobertura
   ja encontrada. Tempo exponencial, O(2^n * m) no pior caso.
   Preenche cobertura[] (n posicoes) com uma cobertura minima e retorna o tamanho
   dela, ou -1 em caso de erro. */
int cobertura_minima_bt(Aresta *a, int m, int n, int *cobertura);

/* Heuristica gulosa: escolhe o vertice de maior grau restante ate cobrir todas as
   arestas. Tempo polinomial, O(n * (n + m)), mas o resultado pode nao ser minimo.
   Preenche cobertura[] (n posicoes) e retorna o tamanho dela, ou -1 em caso de erro. */
int cobertura_gulosa(Aresta *a, int m, int n, int *cobertura);

#endif

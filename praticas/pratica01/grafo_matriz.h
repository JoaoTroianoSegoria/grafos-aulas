#ifndef GRAFO_MATRIZ_H
#define GRAFO_MATRIZ_H

typedef struct {
    int n;
    int **adj;
} GrafoMatriz;

GrafoMatriz* criar_grafo_matriz(int n);
void liberar_grafo_matriz(GrafoMatriz *grafo);
void inserir_aresta_matriz(GrafoMatriz *grafo, int u, int v);
void remover_aresta_matriz(GrafoMatriz *grafo, int u, int v);
int grau_matriz(GrafoMatriz *grafo, int v);
int sao_adjacentes_matriz(GrafoMatriz *grafo, int u, int v);
void exibir_matriz(GrafoMatriz *grafo);

#endif
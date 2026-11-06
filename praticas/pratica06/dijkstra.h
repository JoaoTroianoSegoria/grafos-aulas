#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include <limits.h>

/* Pratica 06 - Menor caminho com Dijkstra e Min-Heap.
   O grafo e DIRECIONADO: a aresta (u, v) sai de u e chega em v. */

#define INFINITO INT_MAX

typedef struct No {
    int vertice;
    int peso;
    struct No *proximo;
} No;

typedef struct {
    No **lista;
    int num_vertices;
} GrafoListaPonderado;

typedef struct {
    int vertice;
    int distancia;
} NoHeap;

typedef struct {
    NoHeap *dados;
    int capacidade, tamanho;
    int *posicao; /* mapeia vertice -> indice no heap (-1 se estiver fora) */
} MinHeap;

/* --- Grafo --- */
GrafoListaPonderado *criar_grafo(int n);
void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo(GrafoListaPonderado *g);
void imprimir_grafo(GrafoListaPonderado *g);

/* --- Min-Heap ---
   capacidade e o numero de vertices: cada vertice entra no heap uma vez. */
MinHeap *criar_min_heap(int capacidade);
void liberar_min_heap(MinHeap *h);
int heap_vazio(MinHeap *h);

/* O(log V). Retorna 1 se inseriu e 0 se o vertice e invalido, ja esta no heap
   ou o heap esta cheio. */
int inserir_heap(MinHeap *h, int vertice, int distancia);

/* O(log V). Com o heap vazio, retorna vertice = -1 e distancia = INFINITO. */
NoHeap extrair_minimo(MinHeap *h);

/* O(log V), pois posicao[vertice] localiza o no sem percorrer o heap.
   Ignora vertices fora do heap e chaves que nao diminuem. */
void diminuir_chave(MinHeap *h, int vertice, int nova_distancia);

/* --- Dijkstra ---
   Preenche dist[] (INFINITO se inalcancavel) e pred[] (-1 se nao ha predecessor);
   os dois vetores precisam ter num_vertices posicoes.
   Retorna 1 em caso de sucesso e 0 se os argumentos forem invalidos, se houver
   aresta de custo negativo ou se faltar memoria. */
int dijkstra(GrafoListaPonderado *g, int origem, int *dist, int *pred);

/* Imprime o caminho origem -> ... -> destino usando pred[].
   Retorna 1 se o caminho existe e 0 caso contrario. */
int reconstruir_caminho(int origem, int destino, int *pred);

#endif

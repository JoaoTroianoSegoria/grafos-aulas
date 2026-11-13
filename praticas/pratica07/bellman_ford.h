#ifndef BELLMAN_FORD_H
#define BELLMAN_FORD_H

#include <limits.h>

/* Pratica 07 - Menor caminho com Bellman-Ford e ciclos de custo negativo.
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

/* --- Grafo --- */
GrafoListaPonderado *criar_grafo(int n);
void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso);
void liberar_grafo(GrafoListaPonderado *g);
void imprimir_grafo(GrafoListaPonderado *g);

/* --- Bellman-Ford ---
   Faz ate n-1 iteracoes de relaxacao e usa a n-esima para detectar ciclos.
   Preenche dist[] (INFINITO se inalcancavel) e pred[] (-1 se nao ha predecessor);
   os dois vetores precisam ter num_vertices posicoes.
   Retorna 1 em caso de sucesso, 0 se houver ciclo negativo alcancavel a partir
   da origem e -1 se os argumentos forem invalidos.
   Com retorno 0, dist[] nao representa menores caminhos, que deixam de existir. */
int bellman_ford(GrafoListaPonderado *g, int origem, int *dist, int *pred);

/* Retrocede n vezes pelo pred[] para garantir que caiu dentro do ciclo e imprime
   o ciclo na direcao das arestas. Deve receber o pred[] de um bellman_ford que
   retornou 0. Retorna o numero de vertices do ciclo, ou 0 se nao ha ciclo. */
int reconstruir_ciclo_negativo(GrafoListaPonderado *g, int *pred);

#endif

#ifndef AGM_H
#define AGM_H

/* Pratica 08 - Arvore Geradora Minima com Kruskal e Prim.
   O grafo e NAO DIRECIONADO: cada aresta aparece nas listas de u e de v. */

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
    int *pai, *rank;
    int n;
} DSU;

typedef struct {
    int u, v, peso;
} Aresta;

/* --- Grafo --- */
GrafoListaPonderado *criar_grafo(int n);
void adicionar_aresta(GrafoListaPonderado *g, int u, int v, int peso); /* lacos sao ignorados */
void liberar_grafo(GrafoListaPonderado *g);
void imprimir_grafo(GrafoListaPonderado *g);

/* --- Union-Find --- */
DSU *criar_dsu(int n);
void liberar_dsu(DSU *d);

/* Retorna o representante do conjunto de x, com compressao de caminho. */
int find(DSU *d, int x);

/* Une os conjuntos de x e y por rank. Retorna 1 se uniu e 0 se ja estavam juntos. */
int unir(DSU *d, int x, int y);

/* --- Arvore Geradora Minima ---
   agm[] precisa ter espaco para num_vertices - 1 arestas e *custo_total recebe
   a soma dos pesos escolhidos. As duas funcoes retornam o numero de arestas
   escolhidas, ou -1 em caso de erro. O grafo e conexo se o retorno for n - 1. */

/* O(E log E). Em grafo desconexo, retorna a floresta geradora minima. */
int kruskal(GrafoListaPonderado *g, Aresta *agm, int *custo_total);

/* O((V + E) log V). Em grafo desconexo, retorna a AGM da componente da origem. */
int prim(GrafoListaPonderado *g, int origem, Aresta *agm, int *custo_total);

#endif

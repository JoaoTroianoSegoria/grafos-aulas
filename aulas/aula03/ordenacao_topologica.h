#include "grafo_lista.h"

#ifndef ORDENACAO_TOPOLOGICA
#define ORDENACAO_TOPOLOGICA

// Preenche "ordenacao" com uma ordem topologica do digrafo g.
void ordenar_dfs(GrafoLista *g, int *visitado, int *ordenacao);

// DFS que grava o vertice em ordenacao[*idx] ao terminar de visita-lo.
void buscar_dfs(GrafoLista *g, int u, int *visitado, int *idx, int *ordenacao);



#endif
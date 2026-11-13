#ifndef HAMILTONIANO_H
#define HAMILTONIANO_H

/* Pratica 10 - Caminho e Ciclo Hamiltoniano.
   O grafo e nao direcionado e representado por matriz de adjacencia "achatada":
   adj[u * n + v] vale 1 se existe a aresta {u, v} e 0 caso contrario. */

/* Forca bruta com backtracking: monta a permutacao dos vertices posicao a posicao,
   so avancando para um vertice ainda nao usado e adjacente ao anterior.
   - ciclo = 0: procura um Caminho Hamiltoniano (testa todos os vertices iniciais);
   - ciclo = 1: procura um Ciclo Hamiltoniano (fixa o vertice 0 como inicio, ja que
     todo ciclo passa por ele, e exige a aresta de volta do ultimo para o 0).
   Tempo exponencial, O(n!) no pior caso.
   Se encontrar, preenche caminho[] (n posicoes) e retorna 1; senao retorna 0.
   Retorna -1 em caso de erro (parametros invalidos ou falta de memoria). */
int encontrar_caminho_hamiltoniano_bt(const int *adj, int n, int ciclo, int *caminho);

/* Confere se caminho[] (n posicoes) e uma permutacao dos vertices em que vertices
   consecutivos sao adjacentes. Com ciclo = 1 exige tambem a aresta de volta.
   Retorna 1 se for valido e 0 caso contrario. */
int eh_caminho_hamiltoniano(const int *adj, int n, int ciclo, const int *caminho);

#endif

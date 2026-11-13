#ifndef TPS_H
#define TPS_H

/* Pratica 10 - Problema do Caixeiro Viajante (TSP).
   As distancias sao simetricas e ficam numa matriz "achatada":
   dist[u * n + v] e o custo de ir de u para v (dist[v * n + v] = 0).
   Uma rota e uma permutacao dos n vertices que comeca na origem 0 e,
   implicitamente, volta para ela no final. */

/* Custo total da rota (n posicoes), incluindo a volta para rota[0]. */
int tsp_custo_rota(const int *dist, int n, const int *rota);

/* Forca bruta: fixa a origem 0 e gera as (n - 1)! permutacoes dos demais
   vertices, guardando a de menor custo. Resultado otimo, tempo O(n!).
   Preenche rota[] (n posicoes) e retorna o custo, ou -1 em caso de erro. */
int tsp_forca_bruta(const int *dist, int n, int *rota);

/* Heuristica do Vizinho Mais Proximo: parte da origem 0 e sempre vai para a
   cidade nao visitada mais proxima da atual. Tempo O(n^2), sem garantia de otimo.
   Preenche rota[] (n posicoes) e retorna o custo, ou -1 em caso de erro. */
int tsp_vizinho_mais_proximo(const int *dist, int n, int *rota);

/* Heuristica de Insercao do Mais Proximo: comeca com o ciclo 0 -> c -> 0, sendo c
   a cidade mais proxima de 0; a cada passo escolhe a cidade fora da rota mais
   proxima de alguma cidade da rota e a insere na posicao que menos aumenta o
   custo. Tempo O(n^3) nesta implementacao, sem garantia de otimo.
   Preenche rota[] (n posicoes) e retorna o custo, ou -1 em caso de erro. */
int tsp_insercao(const int *dist, int n, int *rota);

/* Reducao Ciclo Hamiltoniano -> TSP: transforma o grafo (matriz de adjacencia
   adj, n vertices) num grafo completo com peso 1 nas arestas existentes e 2 nas
   inexistentes. O grafo tem Ciclo Hamiltoniano se, e somente se, o TSP otimo
   nesse grafo completo custa exatamente n.
   Preenche dist[] (n * n posicoes). Retorna 0 em sucesso e -1 em caso de erro. */
int grafo_para_tsp(const int *adj, int n, int *dist);

#endif

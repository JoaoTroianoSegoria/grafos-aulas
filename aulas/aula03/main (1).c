#include <stdio.h>
#include <string.h>
#include "grafo_lista.h"
#include "busca_grafo.h"
#include "ordenacao_topologica.h"
#include "coloracao_grafo.h"

int main()
{
    GrafoLista *grafo = criar_grafo(5);

    adicionar_aresta(grafo, 0, 1);
    adicionar_aresta(grafo, 0, 2);
    adicionar_aresta(grafo, 1, 3);
    adicionar_aresta(grafo, 2, 3);
    adicionar_aresta(grafo, 3, 4);
 
    printf("Grafo nao orientado\n");
    imprimir_grafo(grafo);

    int visitado[10];
    memset(visitado, 0, sizeof(visitado));
    printf("Inicia dfs pelo vertice 1\n");
    dfs(grafo, 0, visitado);
    printf("\n");

    memset(visitado, 0, sizeof(visitado));
    printf("Inicia bfs pelo vertice 1\n");
    bfs(grafo, 0, visitado);
    printf("\n");

    GrafoLista *digrafo = criar_grafo(4);

    adicionar_arco(digrafo, 0, 1);
    adicionar_arco(digrafo, 0, 2);
    adicionar_arco(digrafo, 1, 3);
    adicionar_arco(digrafo, 2, 3);

   printf("Grafo orientado (Digrafo)\n");
   imprimir_grafo(digrafo);
    
   int ordem[MAX_VERTICES];
   memset(visitado, 0, sizeof(visitado));
   printf("Ordena com dfs\n");
   ordenar_dfs(digrafo, visitado, ordem);

   printf("Ordem topologica do digrafo\n");
   for (int i = 0; i < digrafo->num_vertices; i++) {
        printf("%i ", ordem[i] + 1);
   }
   printf("\n");

   // componente conexo
   printf("Componentes conexos do digrafo\n");
   memset(visitado, 0, sizeof(visitado));
   int num_componentes = 0;
   for (int i = 0; i < digrafo->num_vertices; i++) {
    if (!visitado[i]) {
        num_componentes++;
        dfs(digrafo, i, visitado);
    }
   }

   printf("Total de componentes conexos = %i\n", num_componentes);

   printf("Coloração de grafos\n");
   int cor[MAX_VERTICES];
   int cor_usada[MAX_VERTICES];
   coloracao_gulosa(grafo, cor, cor_usada);

   int total_cores = 0;
   for (int i = 0; i < grafo->num_vertices; i++) {
        printf("vertice %i: cor %i\n", i + 1, cor[i] + 1);
        if (cor[i] + 1 > total_cores) total_cores = cor[i] + 1;
   }
   printf("Total de cores usadas = %i\n", total_cores);

    return 0;
}
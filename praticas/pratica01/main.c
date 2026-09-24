#include <stdio.h>
#include "grafo_matriz.h"
#include "grafo_lista.h"

int main() {
    printf("==== Grafo com Matriz ====\n");
    GrafoMatriz *gm = criar_grafo_matriz(5);
    
    inserir_aresta_matriz(gm, 0, 1);
    inserir_aresta_matriz(gm, 0, 2);
    inserir_aresta_matriz(gm, 1, 3);
    
    exibir_matriz(gm);
    printf("Grau do vertice 0: %d\n", grau_matriz(gm, 0));
    printf("Os vertices 0 e 1 sao adjacentes? %s\n", sao_adjacentes_matriz(gm, 0, 1) ? "Sim (1)" : "Nao (0)");
    
    remover_aresta_matriz(gm, 0, 1);
    printf("Os vertices 0 e 1 sao adjacentes apos a remocao? %s\n", sao_adjacentes_matriz(gm, 0, 1) ? "Sim (1)" : "Nao (0)");
    
    liberar_grafo_matriz(gm);

    printf("\n==== Grafo com Lista ====\n");
    GrafoLista *gl = criar_grafo_lista(5);
    
    inserir_aresta_lista(gl, 0, 1);
    inserir_aresta_lista(gl, 0, 2);
    inserir_aresta_lista(gl, 1, 3);
    
    exibir_lista(gl);
    printf("Grau do vertice 0: %d\n", grau_lista(gl, 0));
    printf("Os vertices 0 e 1 sao adjacentes? %s\n", sao_adjacentes_lista(gl, 0, 1) ? "Sim (1)" : "Nao (0)");
    
    remover_aresta_lista(gl, 0, 1);
    printf("Os vertices 0 e 1 sao adjacentes apos a remocao? %s\n", sao_adjacentes_lista(gl, 0, 1) ? "Sim (1)" : "Nao (0)");
    
    liberar_grafo_lista(gl);

    return 0;
}
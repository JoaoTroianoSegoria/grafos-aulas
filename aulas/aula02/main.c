#include <stdio.h>
#include "grafo_matriz.h"

int main() {
    // matriz de adjacencia usa vertices com verticer (posição com posição) 
    GrafoMatriz grafo;

    inicializar(&grafo, 8);

    inserir_aresta(&grafo, 0, 1);
    inserir_aresta(&grafo, 0, 2);
    inserir_aresta(&grafo, 0, 3);
    inserir_aresta(&grafo, 1, 4);
    inserir_aresta(&grafo, 1, 5);
    inserir_aresta(&grafo, 2, 3);
    inserir_aresta(&grafo, 2, 6);
    inserir_aresta(&grafo, 3, 6);
    inserir_aresta(&grafo, 7, 4);
    inserir_aresta(&grafo, 7, 6);

    printf("Matriz de Adjacencia\n"); 
    exibir_matriz(&grafo);

    // Exemplo de grafo com arcos (direcionado)
    inicializar(&grafo, 3);
    /* as lihas e colunas começam em zero. ent o tamanho 3x3 serão:

            coluna coluna coluna
               0     1     1
    linha 0 | 0,0 | 0,1 | 0,2
    linha 1 | 1,0 | 1,1 | 1,2
    linha 2 | 2,0 | 2,1 | 2,2

    */

    inserir_arco(&grafo, 0, 1); 
    inserir_arco(&grafo, 1, 2);
    inserir_arco(&grafo, 2, 0);


    printf("\nMatriz de Adjacencia (grafo direcionado)\n");

    exibir_matriz(&grafo);

    return 0;
}
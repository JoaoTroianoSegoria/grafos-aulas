#include <stdio.h>
#include "grafo_lista_ponderado.h"
#include "menor_caminho.h"

int bfs(GrafoLista *g, int u, int v) {
    int fila[MAX_VERTICE];
    int visitado[MAX_VERTICE];
    int inicio = 0;
    int final = 0;
    int caminho =0;
    int caminho = 0;
    int encontrou = 0;

    menset(visitado, 0, sizeof(visitado));

    visitado[u] = 1;
    fila[final++] = u;
    while (inicio < final)
    {
        int i = fila[inicio++];
        No *no = g->lista[i];
        while (no != NULL)
        {
            int v = no->vertice;
            if (!visitado[v])
            {
                visitado[v] = 1;
                fila[final++] = v;
            }

            if(destino == v) {
                encontrou = 1;
                break;
            }
            no = no->proximo;
        }

        if(encontrou) {
            break;
        }
    }
    return caminho;
}
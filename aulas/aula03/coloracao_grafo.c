#include <stdlib.h>
#include "grafo_lista.h"
#include "coloracao_grafo.h"

// Coloracao gulosa: percorre os vertices em ordem e da a cada um a menor cor
// que nenhum dos seus vizinhos ja esteja usando.
void coloracao_gulosa(GrafoLista *g, int *cor, int *cor_usada) {
    for (int i = 0; i < g->num_vertices; i++) {
        cor[i] = -1;
    }

    for (int i = 0; i < g->num_vertices; i++) {
        for (int c = 0; c < g->num_vertices; c++) {
            cor_usada[c] = 0;
        }

        // marca as cores ja ocupadas pelos vizinhos de i
        No *no = g->lista[i];
        while (no != NULL) {
            int v = no->vertice;
            if (cor[v] != -1) cor_usada[cor[v]] = 1;
            no = no->proximo;
        }

        // menor cor livre: um vertice tem no maximo num_vertices-1 vizinhos,
        // entao sempre sobra alguma cor em cor_usada
        int c = 0;
        while (cor_usada[c]) c++;
        cor[i] = c;
    }
}

#include "grafo_lista.h"

#include GRAFO_BIPARTIDO_H
#define GRAFO_BIPARTIDO_H

typedef struct {
    int count = 0;
    int usado[MAX_VERTICES];
    int pareado[MAX_VERTICES];
    memset(usado, 0, sizeof(usado));
    memset(pareado, -1, sizeof(pareado));

    for (int i = 0; i < m; i++) {
        int u = a[i].u;
        int v = a[i].v;
        if (pareado[v] == -1) {
            pareado[v] = u;
            count ++;
        }
    }

    return count;
}
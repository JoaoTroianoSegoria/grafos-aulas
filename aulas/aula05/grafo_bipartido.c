#include <stdio.h>
#include <stdlib.h>

int emparelhamento_guloso(Aresta *a, int m) {
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
            count++;
        }
    }

    for (int i=0; i<n; i++) {
        printf("{%i, %i} ", i+1, parado[i]+1);
    }
    printf("\n");

    return count;
}
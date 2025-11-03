#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "bellman.h"
#include "print.h"
#include "generate.h"
#include "test_utils.h"

int main()
{
    int source_vertex = 0;
    int NUM_THREADS = 2;
    srand(time(NULL));

    printf("\n=== CASOS DE TESTE ESPECIAIS ===\n");

    // CASO 1: Grafo sem ciclo negativo
    {
        int V = 5;
        int E = 9;
        Edge edges[] = {
            {0, 1, 4}, {0, 2, 2}, {1, 2, 3}, {1, 3, 2}, {1, 4, 3},
            {2, 1, 1}, {2, 3, 4}, {2, 4, 5}, {4, 3, 3}
        };
        printar_caso(1, "Grafo sem ciclo negativo", edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 2: Grafo com pesos negativos e sem ciclo negativo
    {
        int V = 5;
        int E = 7;
        Edge edges[] = {
            {0, 1, -1}, {0, 2, 4}, {1, 2, 3}, {1, 3, 2}, {1, 4, 2},
            {3, 2, 5}, {3, 1, 1}, {4, 3, -2}
        };
        printar_caso(2, "Grafo com pesos negativos e sem ciclo negativo", edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 3: Grafo com ciclo negativo
    {
        int V = 4;
        int E = 5;
        Edge edges[] = {
            {0, 1, 1}, {1, 2, -1}, {2, 3, -1}, {3, 1, -1}, {0, 3, 4}
        };
        printar_caso(3, "Grafo com ciclo negativo", edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 4: Grafo desconexo
    {
        int V = 6;
        int E = 4;
        Edge edges[] = {
            {0, 1, 2}, {1, 2, 3}, {3, 4, 1}, {4, 5, 2}
        };
        printar_caso(4, "Grafo desconexo", edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 5: Grafo pequeno completo e sem ciclo negativo
    {
        int V = 8;
        int E = V * (V - 1);
        Edge edges[] = {
            {0,1,2}, {0,2,3}, {0,3,4}, {0,4,5}, {0,5,6}, {0,6,7}, {0,7,8},
            {1,0,2}, {1,2,4}, {1,3,5}, {1,4,6}, {1,5,7}, {1,6,8}, {1,7,9},
            {2,0,3}, {2,1,4}, {2,3,6}, {2,4,7}, {2,5,8}, {2,6,9}, {2,7,1},
            {3,0,4}, {3,1,5}, {3,2,6}, {3,4,8}, {3,5,9}, {3,6,1}, {3,7,2},
            {4,0,5}, {4,1,6}, {4,2,7}, {4,3,8}, {4,5,1}, {4,6,2}, {4,7,3},
            {5,0,6}, {5,1,7}, {5,2,8}, {5,3,9}, {5,4,1}, {5,6,3}, {5,7,4},
            {6,0,7}, {6,1,8}, {6,2,9}, {6,3,1}, {6,4,2}, {6,5,3}, {6,7,5},
            {7,0,8}, {7,1,9}, {7,2,1}, {7,3,2}, {7,4,3}, {7,5,4}, {7,6,5}
        };
        printar_caso(5, "Grafo pequeno completo e sem ciclo negativo", edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 6: Grafo pequeno imcompleto e sem ciclo negativo
    {
        int V = 8;
        int E = 10;
        Edge edges[] = {
            {0, 1, 3}, {0, 2, 5}, {1, 3, 2}, {2, 4, 4}, {3, 5, 6},
            {4, 6, 2}, {5, 7, 3}, {6, 0, 7}, {2, 5, 1}, {3, 6, 8}
        };
        printar_caso(6, "Grafo pequeno imcompleto e sem ciclo negativo", edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 7: Grafo pequeno completo com pesos negativos e sem ciclo negativo
    {
        int V = 8;
        int E = 56;
        Edge edges[] = {
            {0,1,-1}, {0,2,2}, {0,3,3}, {0,4,2}, {0,5,3}, {0,6,1}, {0,7,2},
            {1,0,2}, {1,2,1}, {1,3,3}, {1,4,2}, {1,5,3}, {1,6,2}, {1,7,2},
            {2,0,2}, {2,1,3}, {2,3,1}, {2,4,2}, {2,5,3}, {2,6,2}, {2,7,1},
            {3,0,3}, {3,1,2}, {3,2,2}, {3,4,1}, {3,5,2}, {3,6,3}, {3,7,2},
            {4,0,2}, {4,1,3}, {4,2,2}, {4,3,2}, {4,5,1}, {4,6,2}, {4,7,3},
            {5,0,3}, {5,1,2}, {5,2,1}, {5,3,3}, {5,4,2}, {5,6,1}, {5,7,2},
            {6,0,2}, {6,1,3}, {6,2,2}, {6,3,1}, {6,4,2}, {6,5,3}, {6,7,1},
            {7,0,2}, {7,1,2}, {7,2,3}, {7,3,2}, {7,4,1}, {7,5,3}, {7,6,2}
        };
        printar_caso(7, "Grafo pequeno completo com pesos negativos e sem ciclo", edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 8: Grafo pequeno imcompleto com pesos negativos e sem ciclo negativo
    {
        int V = 8;
        int E = 10;
        Edge edges[] = {
            {0, 1, -1}, {0, 2, 2}, {0, 3, 3},
            {1, 4, 2}, {1, 5, 3},
            {2, 5, 1}, {2, 6, 2},
            {3, 6, 3}, {3, 7, 2},
            {4, 7, 3},
            {5, 6, 2},
            {6, 7, 1}
        };
        printar_caso(8, "Grafo pequeno imcompleto com pesos negativos e sem ciclo", edges, V, E, source_vertex, NUM_THREADS);
    }

    return 0;
}
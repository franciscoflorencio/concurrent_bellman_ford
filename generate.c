#include "generate.h"

// Função auxiliar para criar arestas de teste
void generate_complete_graph_edges(Edge *edges, int V, int *E, int with_negative)
{
    *E = 0;
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i != j) {
                edges[*E].src = i;
                edges[*E].dest = j;
                if (with_negative && (*E % 7 == 0)) {
                    edges[*E].weight = -(rand() % 5 + 1);
                } else {
                    edges[*E].weight = rand() % 10 + 1;
                }
                (*E)++;
            }
        }
    }
}

void generate_sparse_connected_edges(Edge *edges, int V, int *E, int with_negative)
{
    *E = 0;
    // Assegura conectividade básica
    for (int i = 0; i < V-1; i++) {
        edges[*E].src = i;
        edges[*E].dest = i + 1;
        edges[*E].weight = rand() % 10 + 1;
        (*E)++;
    }
    
    // Adiciona alguns vértices aleatórios para manter o grafo conexto, mas ainda esparso
    int extra_edges = V + (rand() % V);
    for (int i = 0; i < extra_edges; i++) {
        int src = rand() % V;
        int dest = rand() % V;
        if (src != dest) {
            edges[*E].src = src;
            edges[*E].dest = dest;
            if (with_negative && (*E % 5 == 0)) {
                edges[*E].weight = -(rand() % 5 + 1);
            } else {
                edges[*E].weight = rand() % 10 + 1;
            }
            (*E)++;
        }
    }
}
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "bellman.h"

void executar_caso(int caso, const char* descricao, Edge edges[], int V, int E, int source_vertex, int NUM_THREADS)
{
    printf("\n===== CASO %d: %s =====\n", caso, descricao);
    printf("Vertices: %d, Arestas: %d\n", V, E);

    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge *)malloc(E * sizeof(Edge));

    for (int i = 0; i < E; i++) {
        graph->edge[i] = edges[i];
    }

    if (NUM_THREADS > E) {
        NUM_THREADS = E;
    }

    parallel_bellman_ford(graph, source_vertex, NUM_THREADS);

    free(graph->edge);
    free(graph);
}

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

int main()
{
    int source_vertex = 0;
    int NUM_THREADS = 4;  // Aumentado para melhor paralelismo
    srand(42);  // Para resultados consistentes

    // Definição dos tamanhos dos grafos
    const int SMALL_V = 8;
    const int MEDIUM_V = 25;
    const int LARGE_V = 75;

    // Arrays para armazenar as arestas (tamanho máximo possível)
    Edge edges[LARGE_V * LARGE_V];
    int E;

    printf("\n=== TESTES COM GRAFOS PEQUENOS (V=%d) ===\n", SMALL_V);
    
    // Grafo pequeno completo sem ciclo negativo
    generate_complete_graph_edges(edges, SMALL_V, &E, 0);
    executar_caso(1, "Grafo pequeno completo sem ciclo negativo", edges, SMALL_V, E, source_vertex, NUM_THREADS);

    // Grafo pequeno incompleto sem ciclo negativo
    generate_sparse_connected_edges(edges, SMALL_V, &E, 0);
    executar_caso(2, "Grafo pequeno incompleto sem ciclo negativo", edges, SMALL_V, E, source_vertex, NUM_THREADS);

    // Grafo pequeno completo com pesos negativos (sem ciclo)
    generate_complete_graph_edges(edges, SMALL_V, &E, 1);
    executar_caso(3, "Grafo pequeno completo com pesos negativos", edges, SMALL_V, E, source_vertex, NUM_THREADS);

    // Grafo pequeno incompleto com pesos negativos (sem ciclo)
    generate_sparse_connected_edges(edges, SMALL_V, &E, 1);
    executar_caso(4, "Grafo pequeno incompleto com pesos negativos", edges, SMALL_V, E, source_vertex, NUM_THREADS);

    printf("\n=== TESTES COM GRAFOS MÉDIOS (V=%d) ===\n", MEDIUM_V);
    
    // Grafo médio completo sem ciclo negativo
    generate_complete_graph_edges(edges, MEDIUM_V, &E, 0);
    executar_caso(5, "Grafo médio completo sem ciclo negativo", edges, MEDIUM_V, E, source_vertex, NUM_THREADS);

    // Grafo médio incompleto sem ciclo negativo
    generate_sparse_connected_edges(edges, MEDIUM_V, &E, 0);
    executar_caso(6, "Grafo médio incompleto sem ciclo negativo", edges, MEDIUM_V, E, source_vertex, NUM_THREADS);

    // Grafo médio completo com pesos negativos (sem ciclo)
    generate_complete_graph_edges(edges, MEDIUM_V, &E, 1);
    executar_caso(7, "Grafo médio completo com pesos negativos", edges, MEDIUM_V, E, source_vertex, NUM_THREADS);

    // Grafo médio incompleto com pesos negativos (sem ciclo)
    generate_sparse_connected_edges(edges, MEDIUM_V, &E, 1);
    executar_caso(8, "Grafo médio incompleto com pesos negativos", edges, MEDIUM_V, E, source_vertex, NUM_THREADS);

    printf("\n=== TESTES COM GRAFOS GRANDES (V=%d) ===\n", LARGE_V);
    
    // Grafo grande completo sem ciclo negativo
    generate_complete_graph_edges(edges, LARGE_V, &E, 0);
    executar_caso(9, "Grafo grande completo sem ciclo negativo", edges, LARGE_V, E, source_vertex, NUM_THREADS);

    // Grafo grande incompleto sem ciclo negativo
    generate_sparse_connected_edges(edges, LARGE_V, &E, 0);
    executar_caso(10, "Grafo grande incompleto sem ciclo negativo", edges, LARGE_V, E, source_vertex, NUM_THREADS);

    // Grafo grande completo com pesos negativos (sem ciclo)
    generate_complete_graph_edges(edges, LARGE_V, &E, 1);
    executar_caso(11, "Grafo grande completo com pesos negativos", edges, LARGE_V, E, source_vertex, NUM_THREADS);

    // Grafo grande incompleto com pesos negativos (sem ciclo)
    generate_sparse_connected_edges(edges, LARGE_V, &E, 1);
    executar_caso(12, "Grafo grande incompleto com pesos negativos", edges, LARGE_V, E, source_vertex, NUM_THREADS);

    // Casos especiais com ciclos negativos
    {
        // Grafo pequeno com ciclo negativo
        int V = 4;
        E = 5;
        Edge special_edges[] = {
            {0, 1, 1}, {1, 2, -1}, {2, 3, -1}, {3, 1, -1}, {0, 3, 4}
        };
        executar_caso(13, "Grafo pequeno com ciclo negativo", special_edges, V, E, source_vertex, NUM_THREADS);
    }

    {
        // Grafo médio com ciclo negativo
        int V = 6;
        E = 8;
        Edge special_edges[] = {
            {0, 1, 2}, {1, 2, -3}, {2, 3, 4}, {3, 4, 2},
            {4, 2, -6}, {1, 5, 3}, {5, 0, 1}, {2, 0, 2}
        };
        executar_caso(14, "Grafo médio com ciclo negativo", special_edges, V, E, source_vertex, NUM_THREADS);
    }

    {
        // Grafo grande com ciclo negativo (mais complexo)
        int V = 8;
        E = 12;
        Edge special_edges[] = {
            {0, 1, 2}, {1, 2, 3}, {2, 3, -2}, {3, 4, 1},
            {4, 5, -1}, {5, 6, 2}, {6, 7, 3}, {7, 4, -5},
            {1, 7, 4}, {3, 0, 2}, {5, 2, 1}, {6, 3, -2}
        };
        executar_caso(15, "Grafo grande com ciclo negativo", special_edges, V, E, source_vertex, NUM_THREADS);
    }

    return 0;
}
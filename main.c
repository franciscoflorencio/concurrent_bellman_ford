#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "bellman.h"

void print_graph(const Graph* graph, const char* filename)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) 
    {
        printf("Erro ao abrir o arquivo %s para escrita.\n", filename);
        return;
    }

    // Bellman-Ford usa vértices direcionados, portanto chamamos de "digraph"
    fprintf(fp, "digraph G {\n");
    fprintf(fp, " rankdir=LR;\n");

    // escreve as vértices
    for(int i = 0; i < graph->V; i++)
    {
        fprintf(fp, " %d [label=\"%d\"];\n", i, i);
    }

    // escreve as arestas
    for(int i = 0; i < graph->E; i++)
    {
        Edge edge = graph->edge[i];
        fprintf(fp, " %d -> %d [label=\"%d\"];\n", edge.src, edge.dest, edge.weight);
    }

    fprintf(fp, "}\n");
    fclose(fp);
    printf("Grafo escrito em %s\n", filename);
}

void print_result(const Graph* graph, const char* filename, int dist[], int parent[], int source_vertex)
{
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) 
    {
        printf("Erro ao abrir o arquivo %s para escrita.\n", filename);
        return;
    }

    fprintf(fp, "digraph G {\n");
    fprintf(fp, " rankdir=LR;\n");

    // escreve as vértices
    for(int i = 0; i < graph->V; i++)
    {
        char label_text[100];
        if(dist[i] == INFINITY) {
            sprintf(label_text, "%d (INF)", i);
        } else {
            sprintf(label_text, "%d (%d)", i, dist[i]);
        }

        // START node attributes
        fprintf(fp, " %d [label=\"%s\"", i, label_text);

        // Append style attributes
        if(i == source_vertex) {
            fprintf(fp, ", style=filled, fillcolor=lightblue");
        } else if (dist[i] == NEG_INF) { // ciclo negativo
            fprintf(fp, ", style=filled, fillcolor=red");
        }
        
        // END node attributes
        fprintf(fp, "];\n");
    }

    // escreve as arestas do caminho mínimo
    for(int i = 0; i < graph->E; i++)
    {
        int src = graph->edge[i].src;
        int dest = graph->edge[i].dest;
        int weight = graph->edge[i].weight;

        // Start edge attributes (always include label)
        fprintf(fp, " %d -> %d [label=\"%d\"", src, dest, weight);

        // Append shortest path style
        if (parent[dest] == src)
        {
            fprintf(fp, ", color=blue, penwidth=2.0");
        }
        
        // End edge attributes
        fprintf(fp, "];\n");
    }

    fprintf(fp, "}\n");
    fclose(fp);
    printf("Resultado escrito em %s\n", filename);
}

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

    int dist[V];
    int parent[V];

    char filename[50];
    sprintf(filename, "graphs/caso_%d.dot", caso);
    print_graph(graph, filename);

    if (NUM_THREADS > E) {
        NUM_THREADS = E;
    }

    parallel_bellman_ford(graph, source_vertex, NUM_THREADS, dist, parent);

    char filename_res[100];
    sprintf(filename_res, "correctness/caso_%d_threads_%d.dot", caso, NUM_THREADS);
    print_result(graph, filename_res, dist, parent, source_vertex);

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
    int NUM_THREADS = 1;
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
        executar_caso(1, "Grafo sem ciclo negativo", edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 2: Grafo com pesos negativos e sem ciclo negativo
    {
        int V = 5;
        int E = 7;
        Edge edges[] = {
            {0, 1, -1}, {0, 2, 4}, {1, 2, 3}, {1, 3, 2}, {1, 4, 2},
            {3, 2, 5}, {3, 1, 1}, {4, 3, -2}
        };
        executar_caso(2, "Grafo com pesos negativos e sem ciclo negativo", edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 3: Grafo com ciclo negativo
    {
        int V = 4;
        int E = 5;
        Edge edges[] = {
            {0, 1, 1}, {1, 2, -1}, {2, 3, -1}, {3, 1, -1}, {0, 3, 4}
        };
        executar_caso(3, "Grafo com ciclo negativo", edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 4: Grafo desconexo
    {
        int V = 6;
        int E = 4;
        Edge edges[] = {
            {0, 1, 2}, {1, 2, 3}, {3, 4, 1}, {4, 5, 2}
        };
        executar_caso(4, "Grafo desconexo", edges, V, E, source_vertex, NUM_THREADS);
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
        executar_caso(5, "Grafo pequeno completo e sem ciclo negativo", edges, V, E, source_vertex, NUM_THREADS);
    }

    // CASO 6: Grafo pequeno imcompleto e sem ciclo negativo
    {
        int V = 8;
        int E = 10;
        Edge edges[] = {
            {0, 1, 3}, {0, 2, 5}, {1, 3, 2}, {2, 4, 4}, {3, 5, 6},
            {4, 6, 2}, {5, 7, 3}, {6, 0, 7}, {2, 5, 1}, {3, 6, 8}
        };
        executar_caso(6, "Grafo pequeno imcompleto e sem ciclo negativo", edges, V, E, source_vertex, NUM_THREADS);
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
        executar_caso(7, "Grafo pequeno completo com pesos negativos e sem ciclo", edges, V, E, source_vertex, NUM_THREADS);
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
        executar_caso(8, "Grafo pequeno imcompleto com pesos negativos e sem ciclo", edges, V, E, source_vertex, NUM_THREADS);
    }

    return 0;
}
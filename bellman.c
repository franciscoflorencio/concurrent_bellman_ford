#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "bellman.h"

void relax(int u, int v, int weight, int *distances, int *parents)
{
    if (distances[u] != INFINITY && distances[u] + weight < distances[v])
    {
        distances[v] = distances[u] + weight;
        parents[v] = u;
    }
}

void bellman_ford(Graph* graph, int src, int *dist, int *parent)
{
    int V = graph->V;
    int E = graph->E;

    // Incializa distancias e pais
    for (int i = 0; i < V; i++)
    {
        dist[i] = INFINITY;
        parent[i] = -1;
    }
    dist[src] = 0;

    // loop principal do Bellman-Ford
    for (int i = 1; i <= V - 1; i++)
    {
        for (int j = 0; j < E; j++)
        {
            int u = graph->edge[j].src;
            int v = graph->edge[j].dest;
            int weight = graph->edge[j].weight;

            relax(u, v, weight, dist, parent);
        }
    }

    // checa por ciclos negativos
    for (int i = 0; i < E; i++)
    {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        int weight = graph->edge[i].weight;

        if (dist[u] != INFINITY && dist[u] + weight < dist[v])
        {
            dist[v] = NEG_INF; // Indica ciclo negativo
        }
    }
}

// nome auto-explicativo :)
void *bellman_ford_thread(void *arg)
{
    t_args *args = (t_args *)arg;
    Graph *graph = args->graph;
    int V = graph->V;

    int start_edge = args->start_edge;
    int end_edge = start_edge + args->num_edges_for_thread;

    // loop principal do Bellman-Ford, executado em V-1 iterações
    for (int i = 0; i < V - 1; i++)
    {
        // "relaxa" os vértices atribuídos a essa thread
        for (int j = start_edge; j < end_edge; j++)
        {
            int u = graph->edge[j].src;
            int v = graph->edge[j].dest;
            int weight = graph->edge[j].weight;

            relax(u, v, weight, args->distances, args->parents);
        }
        // espera todas as threads terminarem essa iteração antes de prosseguir
        barrier_wait(args->barrier);
    }
    pthread_exit(NULL);
}

// aqui a mágica acontece, CORAÇÃO DO PROGRAMA
void parallel_bellman_ford(Graph *graph, int src, int num_threads, int *dist, int *parent)
{
    int V = graph->V;
    int E = graph->E;
    int *distances = dist;

    // inicializa as distâncias e pais
    for (int i = 0; i < V; i++)
    {
        distances[i] = INFINITY;
        parent[i] = -1;
    }
    distances[src] = 0;
    
    // variáveis de "gerenciamento" das threads
    pthread_t threads[num_threads];
    t_args thread_args[num_threads];
    Barrier barrier;

    // inicializa a barreira
    barrier_init(&barrier, num_threads);

    // cria threads e distribui o trabalho
    int edges_per_thread = E / num_threads;
    for (int i = 0; i < num_threads; i++)
    {
        thread_args[i].thread_id = i;
        thread_args[i].start_edge = i * edges_per_thread;
        thread_args[i].num_edges_for_thread = (i == num_threads - 1)
                                                ? (E - thread_args[i].start_edge) // última thread pega o resto
                                                : edges_per_thread;
        thread_args[i].distances = distances;
        thread_args[i].parents = parent;
        thread_args[i].graph = graph;
        thread_args[i].barrier = &barrier;

        pthread_create(&threads[i], NULL, bellman_ford_thread, (void*)&thread_args[i]);
    }

    // espera todas as threads terminarem
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // "destrói" a barreira
    barrier_destroy(&barrier);

    // checa por ciclos negativos
    for (int i = 0; i < E; i++)
    {
        int u = graph->edge[i].src;
        int v = graph->edge[i].dest;
        int weight = graph->edge[i].weight;
        if (distances[u] != INFINITY && distances[u] + weight < distances[v])
        {
            distances[v] = NEG_INF; // Indica ciclo negativo
        }
    }
}

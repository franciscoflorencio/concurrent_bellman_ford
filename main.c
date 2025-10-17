#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <limits.h>

// definição do infinity que será utilizado no algoritmo
#define INFINITY INT_MAX

// estrutura representante de uma aresta com peso de um grafo
typedef struct
{
    int src, dest, weight;
} Edge;

// estrutura representante de um grafo
typedef struct
{
    int V, E;
    Edge *edge;
} Graph;

// estrutura representante de uma barreira
typedef struct
{
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int count;
    int total_threads;
} Barrier;

// estrutura representante dos argumentos que serão passados para cada thread
typedef struct
{
    int thread_id;
    int start_edge;
    int num_edges_for_thread;
    int *distances;
    Graph *graph;
    Barrier *barrier;
} t_args;

// função para inicializar a barreira
void barrier_init(Barrier *barrier, int num_threads)
{
    pthread_mutex_init(&barrier->mutex, NULL);
    pthread_cond_init(&barrier->cond, NULL);
    barrier->count = 0;
    barrier->total_threads = num_threads;
}

// função de wait da barreira
void barrier_wait(Barrier *barrier)
{
    pthread_mutex_lock(&barrier->mutex);
    barrier->count++;
    if (barrier->count == barrier -> total_threads)
    {
        // isso significa que estamos na última thread a chegar na barreira
        barrier->count = 0; // reset para a próxima utilização
        pthread_cond_broadcast(&barrier->cond);
    }
    else
    {
        // aguarda até que todas as threads cheguem na barreira
        pthread_cond_wait(&barrier->cond, &barrier->mutex);
    }
    pthread_mutex_unlock(&barrier->mutex);
}

// função para "destruir" o mutex e o cond dentro da barreira
void barrier_destroy(Barrier *barrier)
{
    pthread_mutex_destroy(&barrier->mutex);
    pthread_cond_destroy(&barrier->cond);
}

void relax(int u, int v, int weight, int *distances)
{
    if (distances[u] != INFINITY && distances[u] + weight < distances[v])
    {
        distances[v] = distances[u] + weight;
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

            relax(u, v, weight, args->distances);
        }
        // espera todas as threads terminarem essa iteração antes de prosseguir
        barrier_wait(args->barrier);
    }
    pthread_exit(NULL);
}

// aqui a mágica acontece, CORAÇÃO DO PROGRAMA
void parallel_bellman_ford(Graph *graph, int src, int num_threads)
{
    int V = graph->V;
    int E = graph->E;
    int *distances = (int *)malloc(V * sizeof(int));

    // inicializa as distâncias
    for (int i = 0; i < V; i++)
    {
        distances[i] = INFINITY;
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
            printf("Graph contains negative weight cycle\n");
            free(distances);
            return;
        }
    }

    //imprime o resultado
    printf("Distancias do vértice fonte %d:\n", src);
    for (int i = 0; i < V; i++)
    {
        if (distances[i] == INFINITY)
        {
            printf("%d -> %d: INFINITY\n", src, i);
        }
        else
        {
            printf("%d -> %d: %d\n", src, i, distances[i]);
        }
    }

    free(distances);
}

int main(int argc, char *argv[])
{
    // A = 0, B = 1, C = 2, D = 3, E = 4
    int V = 5; // número de vértices
    int E = 9; // número de arestas
    int NUM_THREADS = 2; // número de threads

    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->V = V;
    graph->E = E;
    graph->edge = (Edge *)malloc(E * sizeof(Edge));

    graph->edge[0] = (Edge){0, 1, 4};  // A -> B
    graph->edge[1] = (Edge){0, 2, 2};  // A -> C
    graph->edge[2] = (Edge){1, 2, 3};  // B -> C
    graph->edge[3] = (Edge){1, 3, 2};  // B -> D
    graph->edge[4] = (Edge){1, 4, 3};  // B -> E
    graph->edge[5] = (Edge){2, 1, 1};  // C -> B
    graph->edge[6] = (Edge){2, 3, 4};  // C -> D
    graph->edge[7] = (Edge){2, 4, 5};  // C -> E
    graph->edge[8] = (Edge){4, 3, -5}; // E -> D

    int source_vertex = 0; // vértice fonte (A)

    if (NUM_THREADS > E) {
        NUM_THREADS = E; // não faz sentido ter mais threads do que arestas
    }

    parallel_bellman_ford(graph, source_vertex, NUM_THREADS);

    free(graph->edge);
    free(graph);

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define INF 999999999

typedef struct {
    int first_node;
    int second_node;
} pair_t;

int** floyd_warshall(pair_t** adjacency, int cities) {
    int** distance = (int**)malloc((cities + 1) * sizeof(int*));
    for (int i = 0; i <= cities; i++) {
        distance[i] = (int*)malloc((cities + 1) * sizeof(int));
        for (int j = 0; j <= cities; j++) {
            distance[i][j] = INF;
        }
    }

    for (int x = 1; x <= cities; x++)
        distance[x][x] = 0;

    for (int x = 1; x <= cities; x++)
        for (int i = 0; i < cities; i++) {
            int y = adjacency[x][i].first_node;
            int h = adjacency[x][i].second_node;
            if (y != -1)
                distance[x][y] = h;
        }

    for (int k = 1; k <= cities; k++)
        for (int x = 1; x <= cities; x++)
            for (int y = 1; y <= cities; y++)
                if (distance[x][k] < INF && distance[k][y] < INF)
                    distance[x][y] = (distance[x][y] < distance[x][k] + distance[k][y]) ? distance[x][y] : distance[x][k] + distance[k][y];

    return distance;
}

int main() {
    int cities;
    int agreements;

    while (scanf("%d %d", &cities, &agreements), cities != 0 || agreements != 0) {
        pair_t** graph = (pair_t**)malloc((cities + 1) * sizeof(pair_t*));
        for (int i = 0; i <= cities; i++) {
            graph[i] = (pair_t*)malloc(cities * sizeof(pair_t));
            for (int j = 0; j < cities; j++) {
                graph[i][j].first_node = -1;
                graph[i][j].second_node = -1;
            }
        }

        int X, Y, H;

        while (agreements--) {
            scanf("%d %d %d", &X, &Y, &H);

            bool found = false;
            for (int i = 0; i < cities && !found; i++) {
                if (graph[Y][i].first_node == X) {
                        found = true;
                        graph[Y][i].second_node = 0;
                        graph[X][i].first_node = Y;
                        graph[X][i].second_node = 0;
                    }
                }
                if (!found) {
                    for (int i = 0; i < cities; i++) {
                        if (graph[X][i].first_node == -1) {
                            graph[X][i].first_node = Y;
                            graph[X][i].second_node = H;
                            break;
                        }
                    }
                }
            }

            int** dist = floyd_warshall(graph, cities);

            int queries;
            int origin, destination;

            scanf("%d", &queries);
            while (queries--) {
                scanf("%d %d", &origin, &destination);

                if (dist[origin][destination] != INF)
                    printf("%d\n", dist[origin][destination]);
                else
                    printf("Nao e possivel entregar a carta\n");
            }

            printf("\n");

            // Libera a memória alocada para o grafo e a matriz de distâncias
            for (int i = 0; i <= cities; i++) {
                free(graph[i]);
                free(dist[i]);
            }
            free(graph);
            free(dist);
        }

        return 0;
    }

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define oo 999999999

typedef struct {
    int first;
    int second;
} ii;

/*
 * Computa a distância mínima entre todos os pares de vértices do grafo.
 * Função adaptada de Edson Alves (https://github.com/edsomjr/TEP/blob/master/Grafos/slides/floyd-warshall/floyd-warshall.pdf)
 */
int** floyd_warshall(ii** adj, int N) {
    int** dist = (int**)malloc((N + 1) * sizeof(int*));
    for (int i = 0; i <= N; i++) {
        dist[i] = (int*)malloc((N + 1) * sizeof(int));
        for (int j = 0; j <= N; j++) {
            dist[i][j] = oo;
        }
    }

    // A distância (horas gastas) entre um nó e ele mesmo é 0
    for (int x = 1; x <= N; x++)
        dist[x][x] = 0;

    // Atualiza a distância entre as agências com acordos de envio de mensagens (arestas diretas)
    for (int x = 1; x <= N; x++)
        for (int i = 0; i < N; i++) {
            int y = adj[x][i].first;
            int h = adj[x][i].second;
            if (y != -1)
                dist[x][y] = h;
        }

    // Calcula a menor distância entre todos os pares de vértices e salva em dist
    for (int k = 1; k <= N; k++)
        for (int x = 1; x <= N; x++)
            for (int y = 1; y <= N; y++)
                if (dist[x][k] < oo && dist[k][y] < oo)
                    dist[x][y] = (dist[x][y] < dist[x][k] + dist[k][y]) ? dist[x][y] : dist[x][k] + dist[k][y];

    return dist;
}

int main() {
    int N;  // Número de cidades (Nós)
    int E;  // Número de acordos de envios de mensagens (Arestas)

    // Loop termina quando N = E = 0
    while (scanf("%d %d", &N, &E), N != 0 || E != 0) {
        ii** graph = (ii**)malloc((N + 1) * sizeof(ii*));
        for (int i = 0; i <= N; i++) {
            graph[i] = (ii*)malloc(N * sizeof(ii));
            for (int j = 0; j < N; j++) {
                graph[i][j].first = -1;
                graph[i][j].second = -1;
            }
        }

        int X, Y, H;

        while (E--) {
            scanf("%d %d %d", &X, &Y, &H);

            // Checa se as agências estão na mesma cidade (Existe caminho de X -> Y e Y -> X)
            bool found = false;
            for (int i = 0; i < N && !found; i++) {
                if (graph[Y][i].first == X) {
                        found = true;
                        // Se as agências estão na mesma cidade, o tempo gasto (custo) para envio da carta é 0
                        graph[Y][i].second = 0;
                        graph[X][i].first = Y;
                        graph[X][i].second = 0;
                    }
                }
                if (!found) {
                    for (int i = 0; i < N; i++) {
                        if (graph[X][i].first == -1) {
                            graph[X][i].first = Y;
                            graph[X][i].second = H;
                            break;
                        }
                    }
                }
            }

            int** dist = floyd_warshall(graph, N);

            int K;     // Número de consultas
            int O, D;  // Origem e destino das consultas

            scanf("%d", &K);
            while (K--) {
                scanf("%d %d", &O, &D);

                if (dist[O][D] != oo)
                    printf("%d\n", dist[O][D]);
                else
                    printf("Nao e possivel entregar a carta\n");
            }

            printf("\n");

            // Libera a memória alocada para o grafo e a matriz de distâncias
            for (int i = 0; i <= N; i++) {
                free(graph[i]);
                free(dist[i]);
            }
            free(graph);
            free(dist);
        }

        return 0;
    }

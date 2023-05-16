#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#define infinity INT_MAX
typedef struct celula
{
    int vertice;
    int peso;
    struct celula *prox;
} celula;

typedef struct graph
{
    int quantidadeVertices, quantidadeArestas;
    celula **listaDeAdjacencias;

} graph;

typedef struct aresta
{
    int vertice1;
    int vertice2;
    int peso;

} aresta;

aresta inicializa_aresta(int v1,int v2, int pesoDaAresta)
{
    aresta a;

    a.vertice1 = v1;
    a.vertice2 = v2;
    a.peso = pesoDaAresta;


    return a;
}

celula *cria_no_cabeca(int vertice)
{
    celula *novaCelula = malloc(sizeof(celula));
    novaCelula->vertice = vertice;
    novaCelula->prox = NULL;

    return novaCelula;
}

void cria_adjacencia(int valorDoVertice, celula *noCabeca, int peso)
{
    celula *novaCelula = malloc(sizeof(celula));
    novaCelula->vertice = valorDoVertice;
    novaCelula->peso = peso;
    novaCelula->prox = noCabeca->prox;
    noCabeca->prox = novaCelula;
}

graph *graphInit(int qtdVertices)
{
    int i;
    graph *g = malloc(sizeof(graph));
    g->quantidadeVertices = qtdVertices;
    g->quantidadeArestas = 0;
    g->listaDeAdjacencias = malloc(qtdVertices*sizeof(celula));

    for(i = 0; i< qtdVertices; i++)
        g->listaDeAdjacencias[i] = cria_no_cabeca(i);

    return g;
}
void removeAresta(graph *g,aresta a)
{
    int v1 = a.vertice1;
    int v2 = a.vertice2;

    celula *c1 = g->listaDeAdjacencias[v1];

    while(c1->prox->vertice != v2 && c1!= NULL)
        c1 = c1->prox;

    if(c1 == NULL) return; ///nao existe conexao

    celula *novo1 = c1->prox;
    c1->prox = novo1->prox;
    free(novo1);
}

void insere_aresta_DIRECAO_UNICA(graph *g,aresta a)
{
    int peso = a.peso;
    int v1 = a.vertice1;
    int v2 = a.vertice2;

    cria_adjacencia(v2, g->listaDeAdjacencias[v1], peso);

}
int dijkstra(graph *g, int source) {
    int mature[(g->quantidadeVertices )];
    int distances[(g->quantidadeVertices )];
    int parent[(g->quantidadeVertices)];

    for(int v = 0; v < g->quantidadeVertices; v++){
        parent[v] = -1;
        distances[v] = infinity;
        mature[v] = false;
    }

    parent[source] = source;
    distances[source] = 0;

    while(true) {
        int min = infinity;
        int y;

        for(int z = 0; z < g->quantidadeVertices; z++)
        {
            if(mature[z]) continue;
            if(distances[z] < min) {
                min = distances[z];
                y = z;
            }
        }

        if(min == infinity) break;

        for(celula *t = g->listaDeAdjacencias[y]->prox; t != NULL; t = t->prox)
        {
            if(mature[t->vertice] == true) continue;
            if(distances[y] + t->peso < distances[t->vertice]) {
                distances[t->vertice] = distances[y] + t->peso;
                parent[t->vertice] = y;
            }
        }
        mature[y] = true;
    }
    int maior = -infinity; 
    for(int i = 0; i < g->quantidadeVertices; i++){
        if(distances[i] > maior) maior = distances[i];
    }
    return maior == infinity ? -1 : maior;
}

int networkDelayTime(int** times, int timesSize, int* timesColSize, int n, int k){
    graph *g = graphInit(n);
    k--;
    
    for(int i = 0; i < timesSize; i++){
        int v1 = times[i][0] - 1;
        int v2 = times[i][1] - 1;
        int peso = times[i][2];
        aresta a = inicializa_aresta(v1, v2, peso);
        insere_aresta_DIRECAO_UNICA(g, a);
    }
    return dijkstra(g, k);
}

